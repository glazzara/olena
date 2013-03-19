// Copyright (C) 2008, 2009, 2011, 2012 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_MORPHO_MEYER_WST_HH
# define MLN_MORPHO_MEYER_WST_HH

/** \file
    \brief Meyer's Watershed Transform (WST) algorithm.

    The Watershed Transform algorithm from Meyer using a hierarchical
    queue.

    Reference:
      Fernand Meyer.  Un algorithme optimal de ligne de partage des
      eaux. In: Actes du 8ème Congrès AFCET, Lyon-Villeurbanne, France
      (1991), pages 847--859.  */


/* FIXME: This file is outdated.  Compare this file with
   mln/morpho/watershed/flooding.hh, and remove the former when

   1. it is entirely covered by the latter;
   2. clients (including tests and Milena) are updated to use
      mln::morpho::watershed::flooding.  */


# include <mln/trait/ch_value.hh>

// FIXME: See below.
# include <mln/util/greater_psite.hh>
# include <mln/morpho/includes.hh>
# include <mln/literal/zero.hh>
# include <mln/labeling/regional_minima.hh>

# include <mln/core/site_set/p_queue_fast.hh>
# include <mln/core/site_set/p_priority.hh>


namespace mln
{

  namespace morpho
  {
    /*! \brief Meyer's Watershed Transform (WST) algorithm.

	\param[in]  input   The input image.
	\param[in]  nbh     The connexity of markers.
	\param[out] nbasins The number of basins.

	\li \p L is the type of labels, used to number the watershed
	itself (with the minimal value), and the basins.
	\li \p I is the exact type of the input image.
	\li \p N is the exact type of the neighborhood used to express
	\a input's connexity.

	\ingroup mlnmorphowst
    */
    template <typename L, typename I, typename N>
    mln_ch_value(I, L)
    meyer_wst(const Image<I>& input, const Neighborhood<N>& nbh,
	      L& nbasins);

    /*! \brief Meyer's Watershed Transform (WST) algorithm, with no
        count of basins.

	\param[in]  input   The input image.
	\param[in]  nbh     The connexity of markers.

	\li \p L is the type of labels, used to number the watershed
	itself (with the minimal value), and the basins.
	\li \p I is the exact type of the input image.
	\li \p N is the exact type of the neighborhood used to express
	\a input's connexity.

	Note that the first parameter, \p L, is not automatically
	valued from the type of the actual argument during implicit
	instantiation: you have to explicitly pass this parameter at
	call sites.

	\ingroup mlnmorphowst
    */
    template <typename L, typename I, typename N>
    mln_ch_value(I, L)
    meyer_wst(const Image<I>& input, const Neighborhood<N>& nbh);



# ifndef MLN_INCLUDE_ONLY

    template <typename L, typename I, typename N>
    mln_ch_value(I, L)
    meyer_wst(const Image<I>& input_, const Neighborhood<N>& nbh_,
	      L& nbasins)
    {
      mln_trace("morpho::meyer_wst");
      /* FIXME: Ensure the input image has scalar values.  */

      const I input = exact(input_);
      const N nbh = exact(nbh_);

      typedef L marker;
      const marker unmarked = literal::zero;

      typedef mln_value(I) V;
      const V max = mln_max(V);

      // Initialize the output with the markers (minima components).
      mln_ch_value(I, marker) output =
	labeling::regional_minima (input, nbh, nbasins);

      typedef mln_psite(I) psite;

      // Ordered queue.
      typedef p_queue_fast<psite> Q;
      p_priority<V, Q> queue;

      // In_queue structure to avoid processing sites several times.
      mln_ch_value(I, bool) in_queue;
      initialize(in_queue, input);
      data::fill(in_queue, false);

      // Insert every neighbor P of every marked area in a
      // hierarchical queue, with a priority level corresponding to
      // the grey level input(P).
      mln_piter(I) p(output.domain());
      mln_niter(N) n(nbh, p);
      for_all (p)
	if (output(p) == unmarked)
	  for_all(n)
	    if (output.domain().has(n) && output(n) != unmarked)
	      {
		queue.push(max - input(p), p);
		in_queue(p) = true;
		break;
	      }

      /* Until the queue is empty, extract a psite P from the
         hierarchical queue, at the highest priority level, that is,
         the lowest level.  */
      while (! queue.is_empty())
	{
	  psite p = queue.front();
	  queue.pop();
	  // Last seen marker adjacent to P.
	  marker adjacent_marker = unmarked;
	  // Has P a single adjacent marker?
	  bool single_adjacent_marker_p = true;
	  mln_niter(N) n(nbh, p);
	  for_all(n)
	    if (output.domain().has(n) && output(n) != unmarked)
	      {
		if (adjacent_marker == unmarked)
		  {
		    adjacent_marker = output(n);
		    single_adjacent_marker_p = true;
		  }
		else
		  if (adjacent_marker != output(n))
		    {
		      single_adjacent_marker_p = false;
		      break;
		    }
	      }
	  /* If the neighborhood of P contains only psites with the
	     same label, then P is marked with this label, and its
	     neighbors that are not yet marked are put into the
	     hierarchical queue.  */
	  if (single_adjacent_marker_p)
	    {
	      output(p) = adjacent_marker;
	      for_all(n)
		if (output.domain().has(n) && output(n) == unmarked
		    && ! in_queue(n))
		  {
		    queue.push(max - input(n), n);
		    in_queue(n) = true;
		  }
	    }
	}
      return output;
    }

    template <typename L, typename I, typename N>
    mln_ch_value(I, L)
    meyer_wst(const Image<I>& input, const Neighborhood<N>& nbh)
    {
      L nbasins;
      return meyer_wst<L>(input, nbh, nbasins);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_MEYER_WST_HH
