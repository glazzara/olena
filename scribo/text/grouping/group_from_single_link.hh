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

#ifndef SCRIBO_TEXT_GROUPING_GROUP_FROM_SINGLE_LINK_HH
# define SCRIBO_TEXT_GROUPING_GROUP_FROM_SINGLE_LINK_HH

/// \file scribo/text/grouping/group_from_single_link.hh
///
/// Link text objects with their neighbor line if they have one.

# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>
# include <mln/fun/i2v/array.hh>

# include <mln/accu/bbox.hh>

# include <mln/util/array.hh>

# include <scribo/text/grouping/internal/find_root.hh>

# include <scribo/core/object_image.hh>
# include <scribo/core/macros.hh>


namespace scribo
{

  namespace text
  {

    namespace grouping
    {

      using namespace mln;

      /// Link text objects with their neighbor line if they have one.
      /*!
      ** \param[in] objects    An object image.
      ** \param[in] link_array The neighbor line of each line.
      **
      ** \return An object image with grouped objects.
      */
      template <typename L>
      object_image(L)
      group_from_single_link(const object_image(L)& objects,
			     const mln::util::array<unsigned>& link_array);

# ifndef MLN_INCLUDE_ONLY

      template <typename L>
      inline
      object_image(L)
      group_from_single_link(const object_image(L)& objects,
			     const mln::util::array<unsigned>& link_array)
      {
	trace::entering("scribo::text::grouping::group_from_single_link");

	mln_precondition(objects.is_valid());
	mln_precondition(link_array.nelements() == objects.nlabels().next());
	mln_precondition(link_array.nelements() == objects.bboxes().nelements());

	mln::util::array<unsigned> parent_array = link_array;
	for_all_components(i, parent_array)
	  internal::find_root(parent_array, i);

	object_image(L) output;
	output.init_from_(objects);
	output.relabel(parent_array);

	trace::exiting("scribo::text::grouping::group_from_single_link");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::text::grouping

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_GROUPING_GROUP_FROM_SINGLE_LINK_HH
