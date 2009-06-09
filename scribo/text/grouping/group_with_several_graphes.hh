// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_TEXT_GROUPING_GROUP_WITH_SEVERAL_GRAPHES_HH
# define SCRIBO_TEXT_GROUPING_GROUP_WITH_SEVERAL_GRAPHES_HH

/// \file scribo/text/grouping/group_with_several_graphes.hh
///
/// Group character bounding boxes with several graphes.

# include <mln/core/concept/image.hh>
# include <mln/labeling/compute.hh>
# include <mln/accu/center.hh>

# include <mln/math/abs.hh>

# include <mln/util/array.hh>

# include <scribo/core/macros.hh>
# include <scribo/text/grouping/internal/init_link_array.hh>
# include <scribo/text/grouping/internal/find_graph_link.hh>
# include <scribo/util/text.hh>

namespace scribo
{

  namespace text
  {

    namespace grouping
    {

      /// Group character bounding boxes with several graphes.
      /// Look up for neighbors on the left of each box.
      template <typename L>
      mln::util::graph
      group_with_several_graphes(const scribo::util::text<L>& text,
				unsigned neighb_max_distance);

# ifndef MLN_INCLUDE_ONLY

      template <typename L>
      inline
      mln::util::graph
      group_with_several_graphes(const scribo::util::text<L>& text,
				unsigned neighb_max_distance)
      {
	trace::entering("scribo::text::grouping::group_with_several_graphes");

	mln::util::graph g(text.nbboxes().next());

	for_all_ncomponents(i, text.nbboxes())
	{
	  unsigned midcol = (text.bbox(i).pmax().col()
				- text.bbox(i).pmin().col()) / 2;
	  int dmax = midcol + neighb_max_distance;
	  mln_site(L) c = text.mass_center(i);

	  //  -------
	  //  |	 X------->
	  //  |	    |
	  //  |	    |
	  //  |	 X------->
	  //  |	    |
	  //  |	    |
	  //  |	 X------->
	  //  -------

	  // FIXME: may create several times the same edge.
	  // We should use an adjacency matrix when available.

	  /// Left link from the top anchor.
	  mln_site(L) a1 = c;
	  a1.row() = text.bbox(i).pmin().row() + (c.row() - text.bbox(i).pmin().row()) / 4;
	  internal::find_graph_link(g, text, i, dmax, a1);

	  /// First site on the right of the central site
	  internal::find_graph_link(g, text, i, dmax, c);

	  /// Left link from the bottom anchor.
	  mln_site(L) a2 = c;
	  a2.row() = text.bbox(i).pmax().row() - (c.row() - text.bbox(i).pmin().row()) / 4;
	  internal::find_graph_link(g, text, i, dmax, a2);

	}

	trace::exiting("scribo::text::grouping::group_with_several_graphes");
	return g;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::text::grouping

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_GROUPING_GROUP_WITH_SEVERAL_GRAPHES_HH
