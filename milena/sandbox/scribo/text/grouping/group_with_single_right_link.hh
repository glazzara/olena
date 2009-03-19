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


#ifndef SCRIBO_TEXT_GROUPING_GROUP_WITH_SINGLE_RIGHT_LINK_HH
# define SCRIBO_TEXT_GROUPING_GROUP_WITH_SINGLE_RIGHT_LINK_HH

/// \file scribo/text/grouping/group_with_single_right_link.hh
///
/// Link text bounding boxes with their right neighbor.
///
/// Merge code with text::grouping::group_with_single_right_link.hh

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/math/abs.hh>

# include <mln/util/array.hh>

# include <scribo/core/macros.hh>
# include <scribo/text/grouping/internal/init_link_array.hh>
# include <scribo/text/grouping/internal/update_link_array.hh>
# include <scribo/text/grouping/internal/find_root.hh>
# include <scribo/util/text.hh>

//FIXME: not generic.
# include <mln/core/alias/dpoint2d.hh>

namespace scribo
{

  namespace text
  {

    namespace grouping
    {

      /// Map each character bounding box to its right bounding box neighbor
      /// if possible.
      /// Iterate to the right but link boxes to the right.
      ///
      /// \return an mln::util::array. Map a bounding box to its right neighbor.
      template <typename L>
      inline
      mln::util::array<unsigned>
      group_with_single_right_link(const scribo::util::text<L>& text,
				   unsigned neighb_max_distance);

# ifndef MLN_INCLUDE_ONLY

      template <typename L>
      inline
      mln::util::array<unsigned>
      group_with_single_right_link(const scribo::util::text<L>& text,
				   unsigned neighb_max_distance)
      {
	trace::entering("scribo::text::grouping::group_with_single_right_link");

	mln_precondition(text.is_valid());

	mln::util::array<unsigned> right_link(text.nbboxes().next());
	internal::init_link_array(right_link);

	for_all_ncomponents(i, text.nbboxes())
	{
	  unsigned midcol = (text.bbox(i).pmax().col()
				- text.bbox(i).pmin().col()) / 2;
	  int dmax = midcol + neighb_max_distance;
	  mln_site(L) c = text.bbox(i).center();

	  ///
	  /// Find a neighbor on the left
	  ///

	  ///FIXME: the following code is not generic...
	  /// First site on the left of the central site
	  mln_site(L) p = c + left;

	  const L& lbl = text.label_image();
	  while (lbl.domain().has(p) && (lbl(p) == literal::zero
		    || lbl(p) == i || right_link[i] == lbl(p))
		 && math::abs(p.col() - c.col()) < dmax)
	    --p.col();

	  internal::update_link_array(lbl, right_link, p, c, i, dmax);
	}

	trace::exiting("scribo::text::grouping::group_with_single_right_link");
	return right_link;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::text::grouping

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_GROUPING_GROUP_WITH_SINGLE_RIGHT_LINK_HH
