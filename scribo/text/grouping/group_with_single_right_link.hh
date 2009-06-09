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

#ifndef SCRIBO_TEXT_GROUPING_GROUP_WITH_SINGLE_RIGHT_LINK_HH
# define SCRIBO_TEXT_GROUPING_GROUP_WITH_SINGLE_RIGHT_LINK_HH

/// \file scribo/text/grouping/group_with_single_right_link.hh
///
/// Link text objects with their right neighbor.
///
/// \todo Merge code with text::grouping::group_with_single_right_link.hh

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/accu/center.hh>

# include <mln/labeling/compute.hh>

# include <mln/math/abs.hh>

# include <mln/util/array.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/object_image.hh>
# include <scribo/text/grouping/internal/init_link_array.hh>
# include <scribo/text/grouping/internal/find_right_link.hh>

//FIXME: not generic.
# include <mln/core/alias/dpoint2d.hh>

namespace scribo
{

  namespace text
  {

    namespace grouping
    {

      /// Map each text object to its right bounding box neighbor
      /// if possible.
      /// Iterate to the right but link boxes to the right.
      ///
      /// \param[in] objects An object image.
      /// \param[in] The maximum distance allowed to seach a neighbor object.
      ///
      /// \return an mln::util::array. Map a bounding box to its right neighbor.
      template <typename L>
      inline
      mln::util::array<unsigned>
      group_with_single_right_link(const object_image(L)& objects,
				   unsigned neighb_max_distance);

# ifndef MLN_INCLUDE_ONLY

      template <typename L>
      inline
      mln::util::array<unsigned>
      group_with_single_right_link(const object_image(L)& objects,
				   unsigned neighb_max_distance)
      {
	trace::entering("scribo::text::grouping::group_with_single_right_link");

	mln_precondition(objects.is_valid());

	mln::util::array<unsigned> right_link(objects.nlabels().next());
	internal::init_link_array(right_link);

	mln::util::array<mln_result(accu::center<mln_psite(L)>)>
	    mass_centers = labeling::compute(accu::meta::center(),
					     objects, objects.nlabels());

	for_all_ncomponents(i, objects.nlabels())
	{
	  unsigned midcol = (objects.bbox(i).pmax().col()
				- objects.bbox(i).pmin().col()) / 2;
	  int dmax = midcol + neighb_max_distance;
	  mln_site(L) c = mass_centers(i);

	  /// Find a neighbor on the right
	  internal::find_right_link(objects, right_link, i, dmax, c);
	}

	trace::exiting("scribo::text::grouping::group_with_single_right_link");
	return right_link;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::text::grouping

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_GROUPING_GROUP_WITH_SINGLE_RIGHT_LINK_HH
