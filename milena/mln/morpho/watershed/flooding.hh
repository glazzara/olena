// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_MORPHO_WATERSHED_FLOODING_HH
# define MLN_MORPHO_WATERSHED_FLOODING_HH

/// \file
///
/// Meyer's Watershed Transform (WST) algorithm.
///
///    The Watershed Transform algorithm from Meyer using a hierarchical
///    queue.
///
///    Reference:
///      Fernand Meyer.  Un algorithme optimal de ligne de partage des
///      eaux. In: Actes du 8ème Congrès AFCET, Lyon-Villeurbanne, France
///      (1991), pages 847--859.

# include <mln/trait/ch_value.hh>

# include <mln/morpho/includes.hh>
# include <mln/literal/zero.hh>
# include <mln/labeling/regional_minima.hh>

# include <mln/core/site_set/p_queue_fast.hh>
# include <mln/core/site_set/p_priority.hh>

# include <mln/extension/adjust_fill.hh>


namespace mln
{

  namespace morpho
  {

    namespace watershed
    {

      /*!  \brief Meyer's Watershed Transform (WST) algorithm.
       *
       *  \param[in]  input    The input image.
       *  \param[in]  nbh      The connexity of markers.
       *  \param[out] n_basins The number of basins.
       *
       *  \li \p L is the type of labels, used to number the watershed
       *  itself (with the minimal value), and the basins.
       *  \li \p I is the exact type of the input image.
       *  \li \p N is the exact type of the neighborhood used to express
       *  \a input's connexity.
       *
       * \ingroup mlnmorphowst
       */
      template <typename L, typename I, typename N>
      mln_ch_value(I, L)
      flooding(const Image<I>& input, const Neighborhood<N>& nbh,
	       L& n_basins);

      /*! \brief Meyer's Watershed Transform (WST) algorithm, with no
       *  count of basins.
       *
       *  \param[in]  input   The input image.
       *  \param[in]  nbh     The connexity of markers.
       *
       *  \li \p L is the type of labels, used to number the watershed
       *  itself (with the minimal value), and the basins.
       *  \li \p I is the exact type of the input image.
       *  \li \p N is the exact type of the neighborhood used to express
       *  \a input's connexity.
       *
       *  Note that the first parameter, \p L, is not automatically
       *  valued from the type of the actual argument during implicit
       *  instantiation: you have to explicitly pass this parameter at
       *  call sites.
       *
       *  \ingroup mlnmorphowst
       */
    template <typename L, typename I, typename N>
    mln_ch_value(I, L)
    flooding(const Image<I>& input, const Neighborhood<N>& nbh);



# ifndef MLN_INCLUDE_ONLY


      // Implementations.

      namespace impl
      {

	namespace generic
	{

	  template <typename L, typename I, typename N>
	  mln_ch_value(I, L)
	  flooding(const Image<I>& input_, const Neighborhood<N>& nbh_,
		   L& n_basins)
	  {
	    mln_trace("morpho::watershed::impl::generic::flooding");
	    /* FIXME: Ensure the input image has scalar values.  */

	    const I input = exact(input_);
	    const N nbh = exact(nbh_);

	    typedef L marker;
	    const marker unmarked = literal::zero;

	    typedef mln_value(I) V;
	    const V max = mln_max(V);

	    // Initialize the output with the markers (minima components).
	    mln_ch_value(I, marker) output =
	      labeling::regional_minima (input, nbh, n_basins);

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
	    for_all(p)
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

	} // end of namespace mln::morpho::watershed::impl::generic



	// Fastest version.

	template <typename I, typename N, typename L>
	mln_ch_value(I, L)
	flooding_fastest(const Image<I>& input_, const Neighborhood<N>& nbh_,
			 L& n_basins)
	{
	  mln_trace("morpho::watershed::impl::flooding_fastest");
	  /* FIXME: Ensure the input image has scalar values.  */

	  const I input = exact(input_);
	  const N nbh = exact(nbh_);

	  typedef L marker;
	  const marker unmarked = literal::zero;

	  typedef mln_value(I) V;
	  const V max = mln_max(V);

	  extension::adjust_fill(input, nbh, max);

	  // Initialize the output with the markers (minima components).
	  typedef mln_ch_value(I, L) O;
	  O output = labeling::regional_minima(input, nbh, n_basins);
	  extension::fill(output, unmarked);

	  // Ordered queue.
	  typedef p_queue_fast<unsigned> Q;
	  p_priority<V, Q> queue;

	  // FIXME:  With  typedef std::pair<V, unsigned> VU;
	  //               std::priority_queue<VU> queue;
	  // we do not get the same results!!!

	  // In_queue structure to avoid processing sites several times.
	  mln_ch_value(I, bool) in_queue;
	  initialize(in_queue, input);
	  data::fill(in_queue, false);
	  extension::fill(in_queue, true);

	  // Insert every neighbor P of every marked area in a
	  // hierarchical queue, with a priority level corresponding to
	  // the grey level input(P).
	  mln_pixter(const O)    p_out(output);
	  mln_nixter(const O, N) n_out(p_out, nbh);
	  for_all(p_out)
	    if (p_out.val() == unmarked)
	      for_all(n_out)
		if (n_out.val() != unmarked)
		  {
		    unsigned po = p_out.offset();
		    queue.push(max - input.element(po), po);
		    in_queue.element(po) = true;
		    break;
		  }

	  /* Until the queue is empty, extract a psite P from the
	     hierarchical queue, at the highest priority level, that is,
	     the lowest level.  */
	  util::array<int> dp = offsets_wrt(input, nbh);
	  const unsigned n_nbhs = dp.nelements();
	  while (! queue.is_empty())
	    {
	      unsigned p = queue.front();
	      queue.pop();

	      // Last seen marker adjacent to P.
	      marker adjacent_marker = unmarked;
	      // Has P a single adjacent marker?
	      bool single_adjacent_marker_p = true;
	      for (unsigned i = 0; i < n_nbhs; ++i)
		{
		  unsigned n = p + dp[i];
		  // In the border, output is unmarked so N is ignored.
		  if (output.element(n) != unmarked)
		    {
		      if (adjacent_marker == unmarked)
			{
			  adjacent_marker = output.element(n);
			  single_adjacent_marker_p = true;
			}
		      else
			if (adjacent_marker != output.element(n))
			  {
			    single_adjacent_marker_p = false;
			    break;
			  }
		    }
		}
	      /* If the neighborhood of P contains only psites with the
		 same label, then P is marked with this label, and its
		 neighbors that are not yet marked are put into the
		 hierarchical queue.  */
	      if (single_adjacent_marker_p)
		{
		  output.element(p) = adjacent_marker;
		  for (unsigned i = 0; i < n_nbhs; ++i)
		    {
		      unsigned n = p + dp[i];
		      if (output.element(n) == unmarked
			  // In the border, in_queue is true so N is ignored.
			  && ! in_queue.element(n))
			{
			  queue.push(max - input.element(n), n);
			  in_queue.element(n) = true;
			}
		    }
		}
	    }

	  return output;
	}


      } // end of namespace mln::morpho::watershed::impl



      // Dispatch.

      namespace internal
      {

	template <typename I, typename N, typename L>
	inline
	mln_ch_value(I, L)
	flooding_dispatch(metal::false_,
			  const Image<I>& input, const Neighborhood<N>& nbh,
			  L& n_basins)
	{
	  return impl::generic::flooding(input, nbh, n_basins);
	}


	template <typename I, typename N, typename L>
	inline
	mln_ch_value(I, L)
	flooding_dispatch(metal::true_,
			  const Image<I>& input, const Neighborhood<N>& nbh,
			  L& n_basins)
	{
 	  return impl::flooding_fastest(input, nbh, n_basins);
	}

	template <typename I, typename N, typename L>
	inline
	mln_ch_value(I, L)
	flooding_dispatch(const Image<I>& input, const Neighborhood<N>& nbh,
			  L& n_basins)
	{
	  enum {
	    test = mlc_equal(mln_trait_image_speed(I),
			     trait::image::speed::fastest)::value
	    &&
	    mln_is_simple_neighborhood(N)::value
	  };
	  return flooding_dispatch(metal::bool_<test>(),
				   input, nbh, n_basins);
	}

      } // end of namespace mln::morpho::watershed::internal


      // Facades.

      template <typename L, typename I, typename N>
      inline
      mln_ch_value(I, L)
      flooding(const Image<I>& input, const Neighborhood<N>& nbh, L& n_basins)
      {
	mln_trace("morpho::watershed::flooding");

	// FIXME: internal::flooding_tests(input, nbh, n_basins);

	mln_ch_value(I, L) output =
	  internal::flooding_dispatch(input, nbh, n_basins);

	return output;
      }

    template <typename L, typename I, typename N>
    mln_ch_value(I, L)
    flooding(const Image<I>& input, const Neighborhood<N>& nbh)
    {
      L nbasins;
      return flooding<L>(input, nbh, nbasins);
    }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::watershed

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_WATERSHED_FLOODING_HH
