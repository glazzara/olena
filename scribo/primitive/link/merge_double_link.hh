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

#ifndef SCRIBO_PRIMITIVE_LINK_MERGE_DOUBLE_LINK_HH
# define SCRIBO_PRIMITIVE_LINK_MERGE_DOUBLE_LINK_HH


/// \file
///
/// Validate and merge double link information. A link must exist in
/// both ways to be validated.


# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>

# include <mln/accu/shape/bbox.hh>

# include <mln/data/compare.hh>

# include <mln/util/array.hh>

# include <scribo/core/macros.hh>

# include <scribo/core/object_links.hh>
# include <scribo/core/component_set.hh>
# include <scribo/primitive/internal/find_root.hh>
# include <scribo/primitive/internal/is_link_valid.hh>
# include <scribo/primitive/internal/init_link_array.hh>


namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      /*!
	 \brief Validate and merge double link information. A link
	 must exist in both ways to be validated.

	  \param[in] components     The Lines of text.
	  \param[in] left_link   The left neighbor of each line of text.
	  \param[in] right_link  The right neighbor of each line of text.

	  \return The merge of left_link and right_link.
      */
      template <typename L>
      object_links<L>
      merge_double_link(const component_set<L>& components,
			const object_links<L>& left_link,
			const object_links<L>& right_link);



# ifndef MLN_INCLUDE_ONLY


      template <typename L>
      inline
      object_links<L>
      merge_double_link(const component_set<L>& components,
			const object_links<L>& left_link,
			const object_links<L>& right_link)
      {
	trace::entering("scribo::primitive::link::merge_double_link");

	mln_precondition(components.is_valid());
	mln_precondition(left_link.nelements() == right_link.nelements());
	mln_precondition(left_link.components_id_() == components.id_());
	mln_precondition(right_link.components_id_() == components.id_());


	object_links<L> merge(left_link);

	for_all_ncomponents(i, components.nelements())
	{
	  if (components(i).tag() == component::Ignored)
	    merge[i] = 0;
	  else
	  {
	    mln::util::couple<bool, unsigned>
	      nbh = primitive::internal::is_link_valid(left_link,
						       right_link, i);
	    if (!nbh.first())
	      merge[i] = i;
	  }
	}

	trace::exiting("scribo::primitive::link::merge_double_link");
	return merge;
      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_LINK_MERGE_DOUBLE_LINK_HH
