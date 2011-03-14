// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef SCRIBO_PRIMITIVE_INTERNAL_FIND_SEVERAL_LINKS_HH
# define SCRIBO_PRIMITIVE_INTERNAL_FIND_SEVERAL_LINKS_HH

/// \file
///
/// Find the neighbor of a line of text if exists.
///
/// \fixme do not iterate over the number of anchor types but use a
/// set of anchors specified in the functor.

# include <mln/core/concept/image.hh>

# include <mln/math/abs.hh>

# include <mln/util/array.hh>
# include <mln/util/couple.hh>

# include <scribo/core/concept/link_functor.hh>
# include <scribo/core/tag/anchor.hh>
# include <scribo/core/object_links.hh>


namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      namespace internal
      {

	/*! Find the neighbor of a line of text if exists.

	  \param[in,out] functor Functor used to compute the
	  links. Stores the results.
	  \param current_object Current object id.

	  \return A couple. The first argument tells whether a valid
	  link has been found, the second one is link anchor if exists.
	*/
	template <typename F>
	mln::util::couple<bool,
			  mln::util::couple<anchor::Type,
					    mln_site(scribo_support_(F))> >
	find_several_links(Link_Functor<F>& functor,
			   unsigned current_object);


# ifndef MLN_INCLUDE_ONLY

	template <typename F>
	mln::util::couple<bool,
			  mln::util::couple<anchor::Type,
					    mln_site(scribo_support_(F))> >
	find_several_links(Link_Functor<F>& functor_,
			   unsigned current_object)
	{
	  F& functor = exact(functor_);

	  typedef mln_site(scribo_support_(F)) P;
	  typedef mln::util::couple<anchor::Type, P> link_t;

	  functor.initialize_link(current_object); // <-- initialize_link

	  for_all_anchors(a, functor.anchors()) // <-- nanchor && anchors
	  {
	    // FIXME : See fixme at the beginning of this file.
	    anchor::Type anchor = functor.anchors()[a];

	    mln_site(scribo_support_(F))
	      start_point = functor.start_point(current_object, anchor), // <-- start_point
	      p = start_point;

	    mln_postcondition(p == start_point);

	    while (functor.labeled_image().domain().has(p)
		   && ! functor.is_potential_link(current_object,
						  start_point, p) // <-- is_potential_link
		   && functor.verify_link_criterion(current_object,
						    start_point, p, anchor)) // <-- verify_link_criterion
	      functor.compute_next_site(p); // <-- compute_next_site

	    if (functor.valid_link(current_object, start_point, p, anchor)) // <-- valid_link
	      functor.validate_link(current_object, start_point, p, anchor); // <-- validate_link
	    else
	      functor.invalidate_link(current_object, start_point, p, anchor); // <-- invalidate_link

	  }

	  link_t l = functor.finalize_link(current_object);

	  bool b = (functor.link(current_object) != current_object); // <-- link
	  return mln::make::couple(b, l);
	}

# endif // MLN_INCLUDE_ONLY


      } // end of namespace scribo::primitive::link::internal

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_INTERNAL_FIND_SEVERAL_LINKS_HH
