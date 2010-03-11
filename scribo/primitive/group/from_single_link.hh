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

#ifndef SCRIBO_PRIMITIVE_GROUP_FROM_SINGLE_LINK_HH
# define SCRIBO_PRIMITIVE_GROUP_FROM_SINGLE_LINK_HH

/// \file
///
/// Link text objects with their neighbor line if they have one.

# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>
# include <mln/fun/i2v/array.hh>

# include <mln/accu/shape/bbox.hh>

# include <mln/util/array.hh>

# include <scribo/primitive/internal/find_root.hh>

# include <scribo/core/object_groups.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/macros.hh>


namespace scribo
{

  namespace primitive
  {

    namespace group
    {

      using namespace mln;

      /*!
	 \brief Link text components with their neighbor line if they
	 have one.

	 \param[in] components A component set.
	 \param[in] link_array The neighbor line of each line.

	 \return Object groups information.
      */
      template <typename L>
      object_groups<L>
      from_single_link(const component_set<L>& components,
		       const object_links<L>& link_array);


# ifndef MLN_INCLUDE_ONLY

      template <typename L>
      inline
      object_groups<L>
      from_single_link(const component_set<L>& components,
		       const object_links<L>& link_array)
      {
	trace::entering("scribo::primitive::group::from_single_link");

	mln_precondition(components.is_valid());
	mln_precondition(link_array.nelements() == components.nelements().next());
	mln_precondition(link_array.components_id_() == components.id_());

	object_groups<L> parent_array(components);
	parent_array.init_(link_array);

	for_all_components(i, parent_array)
	  primitive::internal::find_root(parent_array, i);

	trace::exiting("scribo::primitive::group::from_single_link");
	return parent_array;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::group

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_GROUP_FROM_SINGLE_LINK_HH
