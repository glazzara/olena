// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_MORPHO_MEYER_WST_HH
# define MLN_MORPHO_MEYER_WST_HH

/** \file mln/morpho/meyer_wst.hh
    \brief Meyer's Watershed Transform (WST) algorithm.

    The Watershed Transform algorithm from Meyer using a hierarchical
    queue.

    Reference:
      Fernand Meyer.  Un algorithme optimal de ligne de partage des
      eaux. In: Actes du 8ème Congrès AFCET, Lyon-Villeurbanne, France
      (1991), pages 847--859.  */

# include <queue>

# include <mln/trait/ch_value.hh>

# include <mln/util/greater_point.hh>
# include <mln/morpho/includes.hh>
# include <mln/morpho/extrema_components.hh>


namespace mln
{

  namespace morpho
  {
    /* FIXME: Provide also a version of the algorithm taking an image
       of minima as input.  */

    /* FIXME: See also the interface of the Shortest-Path Watershed
       Transform, which proposes to lower-complete the image before
       processing it.  Then, add a reference to
       mln/morpho/lower_completion.hh.  */

    /* FIXME: More doc.  */

    /// Meyer's Watershed Transform (WST) algorithm.
    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    meyer_wst(const Image<I>& input, const Window<W>& win, unsigned& nbasins);

    /// Meyer's Watershed Transform (WST) algorithm, with no count of
    /// basins.
    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    meyer_wst(const Image<I>& input, const Window<W>& win);


# ifndef MLN_INCLUDE_ONLY

    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    meyer_wst(const Image<I>& input, const Window<W>& win, unsigned& nbasins)
    {
      /* FIXME: Ensure the input image has scalar values.  */

      typedef DestValue marker;
      const marker unmarked = mln_min(marker);

      // Initialize the output with the markers (minima components).
      mln_ch_value(I, marker) markers =
	minima_components<marker>(input, win, nbasins);

      // Ordered queue.
      typedef mln_point(I) point;
      typedef
	std::priority_queue< point, std::vector<point>, util::greater_point<I> >
	ordered_queue_type;
      ordered_queue_type queue(util::make_greater_point(input));

      // Insert every neighbor P of every marked area in a
      // hierarchical queue, with a priority level corresponding to
      // the grey level input(P).
      mln_piter(I) p(markers.domain());
      mln_qiter(W) q(win, p);
      for_all (p)
	if (markers(p) == unmarked)
	  for_all(q)
	    if (markers.has(q) && markers(q) != unmarked)
	      {
		queue.push(p);
		break;
	      }

      /* Until the queue is empty, extract a point P from the
         hierarchical queue, at the highest priority level, that is,
         the lowest level.  */
      while (!queue.empty())
	{
	  point p = queue.top();
	  queue.pop();
	  // Last seen marker adjacent to P.
	  marker adjacent_marker = unmarked;
	  // Has P a single adjacent marker?
	  bool single_adjacent_marker_p = true;
	  mln_qiter(W) q(win, p);
	  for_all(q)
	    if (markers.has(q) && markers(q) != unmarked)
	      if (adjacent_marker == unmarked)
		{
		  adjacent_marker = markers(q);
		  single_adjacent_marker_p = true;
		}
	      else
		if (adjacent_marker != markers(q))
		  {
		    single_adjacent_marker_p = false;
		    break;
		  }
	  /* If the neighborhood of P contains only points with the
	     same label, then P is marked with this label, and its
	     neighbors that are not yet marked are put into the
	     hierarchical queue.  */
	  if (single_adjacent_marker_p)
	    {
	      markers(p) = adjacent_marker;
	      for_all(q)
		if (markers.has(q) && markers(q) == unmarked)
		  queue.push(q);
	    }
	}
      return markers;
    }

    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    meyer_wst(const Image<I>& input, const Window<W>& win)
    {
      unsigned nbasins;
      return meyer_wst<DestValue>(input, nbasins);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_MEYER_WST_HH
