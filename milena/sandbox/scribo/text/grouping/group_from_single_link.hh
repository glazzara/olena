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


#ifndef SCRIBO_TEXT_GROUPING_GROUP_FROM_SINGLE_LINK_HH
# define SCRIBO_TEXT_GROUPING_GROUP_FROM_SINGLE_LINK_HH

/// \file scribo/text/grouping/group_from_single_link.hh
///
/// Link text bounding boxes with their left neighbor.

# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>

# include <mln/accu/bbox.hh>

# include <mln/util/array.hh>

# include <scribo/text/grouping/internal/find_root.hh>

# include <scribo/core/macros.hh>


namespace scribo
{

  namespace text
  {

    namespace grouping
    {

      /// FIXME: Add much more doc!
      template <typename P>
      util::array< box<P> >
      group_from_single_link(const util::array< box<P> >& textbboxes,
			     util::array<unsigned>& link_array);

# ifndef MLN_INCLUDE_ONLY

      template <typename P>
      inline
      util::array< box<P> >
      group_from_single_link(const util::array< box<P> >& textbboxes,
			     util::array<unsigned>& link_array)
      {
	trace::entering("scribo::text::grouping::group_from_single_link");

	for (unsigned i = 0; i < link_array.nelements(); ++i)
	  link_array[i] = internal::find_root(link_array, i);

	util::array< accu::bbox<P> > tboxes;
	tboxes.resize(textbboxes.nelements());
	for_all_components(i, textbboxes)
	  tboxes[link_array[i]].take(textbboxes[i]);

	util::array< box<P> > result;
	// component 0, the background, has an invalid box.
	result.append(box<P>());
	for_all_components(i, tboxes)
	  if (tboxes[i].is_valid())
	    result.append(tboxes[i]);

	trace::exiting("scribo::text::grouping::group_from_single_link");
	return result;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::text::grouping

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_GROUPING_GROUP_FROM_SINGLE_LINK_HH
