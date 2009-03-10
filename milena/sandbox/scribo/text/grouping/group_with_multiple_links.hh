// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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


#ifndef SCRIBO_TEXT_GROUPING_GROUP_WITH_MULTIPLE_LINKS_HH
# define SCRIBO_TEXT_GROUPING_GROUP_WITH_MULTIPLE_LINKS_HH

/// \file scribo/text/grouping/group_with_multiple_links.hh
///
/// Group character bounding boxes with multiple links.

# include <mln/core/concept/image.hh>

# include <mln/math/abs.hh>

# include <mln/util/array.hh>

# include <scribo/core/macros.hh>
# include <scribo/text/grouping/internal/init_link_array.hh>
# include <scribo/text/grouping/internal/update_link_graph.hh>

namespace scribo
{

  namespace text
  {

    namespace grouping
    {

      /// Group character bounding boxes with multiple links.
      /// Look up for neighbors on the left of each box.
      template <typename I, typename N, typename V>
      util::graph
      group_with_multiple_links(const Image<I>& input_,
				const Neighborhood<N>& nbh, V& nbboxes,
				const util::array<box<mln_site(I)> >& textbboxes,
				unsigned neighb_max_distance);

# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename N, typename V>
      util::graph
      group_with_multiple_links(const Image<I>& input_,
				const Neighborhood<N>& nbh, V& nbboxes,
				const util::array<box<mln_site(I)> >& textbboxes,
				unsigned neighb_max_distance)
      {
	trace::entering("scribo::text::grouping::group_with_multiple_links");

	const I& input = exact(input_);

	mlc_equal(mln_value(I), bool)::check();
	mln_assertion(input.is_valid());

	typedef mln_ch_value(I,V) lbl_t;
	lbl_t lbl = labeling::blobs(input, nbh, nbboxes);

	util::graph g(nbboxes.next());

	for_all_ncomponents(i, nbboxes)
	{
	  unsigned midcol = (textbboxes[i].pmax().col()
				- textbboxes[i].pmin().col()) / 2;
	  int dmax = midcol + neighb_max_distance;
	  mln_site(I) c = textbboxes[i].center();

	  /// First site on the right of the central site
	  mln_site(I) p = c + right;

	  while (lbl.domain().has(p) && (lbl(p) == literal::zero || lbl(p) == i)
	      && math::abs(p.col() - c.col()) < dmax)
	    ++p.col();

	  internal::update_link_graph(lbl, g, p, c, i, dmax);
	}

	trace::exiting("scribo::text::grouping::group_with_multiple_links");
	return g;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::text::grouping

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_GROUPING_GROUP_WITH_MULTIPLE_LINKS_HH
