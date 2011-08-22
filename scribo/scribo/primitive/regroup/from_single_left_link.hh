// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_PRIMITIVE_RELINK_FROM_SINGLE_LEFT_LINK_HH
# define SCRIBO_PRIMITIVE_RELINK_FROM_SINGLE_LEFT_LINK_HH

/// \file
///
/// \brief Regroup groups with their left neighbor.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/accu/center.hh>

# include <mln/labeling/compute.hh>

# include <mln/math/abs.hh>

# include <mln/util/array.hh>


# include <scribo/core/macros.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/object_groups.hh>

# include <scribo/primitive/group/apply.hh>
# include <scribo/primitive/group/from_single_link.hh>
# include <scribo/primitive/link/with_single_left_link.hh>
# include <scribo/primitive/link/internal/find_link.hh>
# include <scribo/primitive/link/internal/link_single_dmax_base.hh>

# include <scribo/primitive/link/compute.hh>


namespace scribo
{

  namespace primitive
  {

    namespace regroup
    {

      /// \brief Regroup groups with their left neighbor.
      ///
      /// \param[in] groups Groups data.
      /// \param[in] The maximum distance allowed to seach a neighbor object.
      ///
      /// \return Object groups data.
      //
      template <typename L>
      inline
      object_groups<L>
      from_single_left_link(const object_groups<L>& groups,
			    unsigned neighb_max_distance);


      /// \overload
      /// Max distance is set to mln_max(unsigned).
      template <typename L>
      inline
      object_groups<L>
      from_single_left_link(const object_groups<L>& groups);


# ifndef MLN_INCLUDE_ONLY


      // Facades

      template <typename L>
      inline
      object_groups<L>
      from_single_left_link(const object_groups<L>& groups,
			    unsigned neighb_max_distance)
      {
	trace::entering("scribo::primitive::regroup::from_single_left_link");

	mln_precondition(groups.is_valid());


	object_groups<L> output = groups.duplicate();

	fun::i2v::array<mln_value(L)> relabel_fun;
	component_set<L>
	  components = primitive::group::apply(groups, relabel_fun);

	object_links<L>
	  links = primitive::link::with_single_left_link(components, neighb_max_distance);

	object_groups<L>
	  new_groups = primitive::group::from_single_link(links);

	// Preserve the original groups ids but merge newly regrouped
	// groups.
	for_all_groups(g, new_groups)
	  if (new_groups(g).card() > 1)
	    for (unsigned i = 1; i < new_groups(g).component_ids().nelements(); ++i)
	      output(relabel_fun(new_groups(g).component_ids()(0)))
		.merge(output(relabel_fun(new_groups(g).component_ids()(i))));

	trace::exiting("scribo::primitive::regroup::from_single_left_link");
	return output;
      }


      template <typename L>
      inline
      object_groups<L>
      from_single_left_link(const object_groups<L>& groups)
      {
	return from_single_left_link(groups, mln_max(unsigned));
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::regroup

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_LINK_FROM_SINGLE_LEFT_LINK_HH
