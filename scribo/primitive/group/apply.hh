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

#ifndef SCRIBO_PRIMITIVE_GROUP_APPLY_HH
# define SCRIBO_PRIMITIVE_GROUP_APPLY_HH

/// \file
///
/// Apply grouping in an object image.

# include <scribo/core/object_links.hh>
# include <scribo/core/object_groups.hh>
# include <scribo/core/object_image.hh>

# include <scribo/primitive/group/from_single_link.hh>

namespace scribo
{

  namespace primitive
  {

    namespace group
    {

      using namespace mln;

      /*! \brief Apply grouping in an object image.

	  \param objects An object image.
	  \param groups  An object group structure.

	  \return A copy of \p objects with grouped objects.
      */
      template <typename L>
      object_image(L)
      apply(const object_image(L)& objects,
	    const object_groups<L>& groups);

      /// \overload
      template <typename L>
      object_image(L)
      apply(const object_image(L)& objects,
	    const object_links<L>& links);



# ifndef MLN_INCLUDE_ONLY


      template <typename L>
      object_image(L)
      apply(const object_image(L)& objects,
	    const object_groups<L>& groups)

      {
	trace::entering("scribo::primitive::group::apply");

	mln_precondition(objects.is_valid());
	mln_precondition(groups.nelements() == objects.nlabels().next());
	mln_precondition(groups.nelements() == objects.bboxes().nelements());
	mln_precondition(groups.objects_id_() == objects.id_());

	object_image(L) output;
	output.init_from_(objects);
	output.relabel(groups);

	trace::exiting("scribo::primitive::group::apply");
	return output;
      }


      template <typename L>
      object_image(L)
      apply(const object_image(L)& objects,
	    const object_links<L>& links)

      {
	trace::entering("scribo::primitive::group::apply");

	mln_precondition(objects.is_valid());
	mln_precondition(links.nelements() == objects.nlabels().next());
	mln_precondition(links.nelements() == objects.bboxes().nelements());
	mln_precondition(links.objects_id_() == objects.id_());


	object_groups<L> group = group::from_single_link(objects, links);

	object_image(L) output = apply(objects, links);

	trace::exiting("scribo::primitive::group::apply");
	return output;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::group

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_GROUP_APPLY_HH
