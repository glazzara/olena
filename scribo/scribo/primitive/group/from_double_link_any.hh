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

#ifndef SCRIBO_PRIMITIVE_GROUP_FROM_DOUBLE_LINK_ANY_HH
# define SCRIBO_PRIMITIVE_GROUP_FROM_DOUBLE_LINK_ANY_HH


/// \file
///
/// Group objects from left and right links information and validate
/// These links. A link must exist in both ways to be validated.


# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>

# include <mln/accu/shape/bbox.hh>

# include <mln/util/array.hh>

# include <scribo/core/macros.hh>

# include <scribo/core/object_links.hh>
# include <scribo/core/object_groups.hh>
# include <scribo/core/component_set.hh>
# include <scribo/primitive/internal/find_root.hh>


namespace scribo
{

  namespace primitive
  {

    namespace group
    {

      /*! Group components from left and right links information and
	  validate These links.

	  \param[in] left_link   The left neighbor of each line of text.
	  \param[in] right_link  The right neighbor of each line of text.

	  \return Return object groups information.
      */
      template <typename L>
      object_groups<L>
      from_double_link_any(const object_links<L>& left_link,
			   const object_links<L>& right_link);



# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename L>
	util::array<unsigned> make_parent(const object_links<L>& link)
	{
	  mln::util::array<unsigned> parent = link.comp_to_link();
	  for_all_links(l, link)
	    parent(l) = scribo::internal::find_root(parent, l);

	  return parent;
	}

      } // end of namespace scribo::primitive::group::internal


      template <typename L>
      inline
      object_groups<L>
      from_double_link_any(const object_links<L>& left_link,
			   const object_links<L>& right_link)
      {
	trace::entering("scribo::primitive::group::from_double_link_any");

	object_links<L> merged_link(left_link.components());
	merged_link.init();

	util::array<unsigned> lparent = internal::make_parent(left_link);
	util::array<unsigned> rparent = internal::make_parent(right_link);

	for_all_links(i, merged_link)
	{
	  // Looking for new left link
	  unsigned
	    pi = scribo::internal::find_root(lparent, i),
	    pli = scribo::internal::find_root(lparent, left_link(i));

	  if (pi != pli)
	  {
	    merged_link.update(i, left_link(i));
	    if (pi < pli)
	      lparent(pli) = pi;
	    else
	      lparent(pi) = pli;
	  }

	  // Looking for new right link
	  pi = scribo::internal::find_root(lparent, i);
	  unsigned pri = scribo::internal::find_root(rparent, right_link(i));

	  if (pi != pri)
	  {
	    merged_link.update(i, right_link(i));
	    if (pi < pli)
	      lparent(pli) = pi;
	    else
	      lparent(pi) = pli;
	  }
	}

	object_groups<L> output(merged_link);

	trace::exiting("scribo::primitive::group::from_double_link_any");
	return output;
      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::primitive::group

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_GROUP_FROM_DOUBLE_LINK_ANY_HH
