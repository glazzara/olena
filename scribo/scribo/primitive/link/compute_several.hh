
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

#ifndef SCRIBO_PRIMITIVE_LINK_COMPUTE_SEVERAL_HH
# define SCRIBO_PRIMITIVE_LINK_COMPUTE_SEVERAL_HH

/// \file
///
/// Compute_Several links between objects according a given functor.


# include <scribo/core/macros.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/concept/link_functor.hh>
# include <scribo/primitive/link/internal/find_several_links.hh>

namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      /*! \brief Compute_Several links between objects according a given functor.

	  \param[in,out] functor Linking policy.

	  \return Object links.


	  Functors must implement the following interface :


	  bool verify_link_criterion_(unsigned current_object,
				      const P& start_point, const P& p) const;

	  void start_processing_object_(unsigned current_object);


	  mln_site(L) start_point_(unsigned current_object, unsigned anchor);


	  void validate_link_(unsigned current_object, const P& start_point,
			      const P& p, unsigned anchor);
	  void invalidate_link_(unsigned current_object, const P& start_point,
				const P& p, unsigned anchor);

	  void initialize_link_(unsigned current_object);
	  void finalize_link_(unsigned current_object);

	  bool is_potential_link(unsigned current_object,
	                         const P& start_point, const P& p) const

	  void compute_next_site(P& p)

      */
      template <typename F>
      object_links<scribo_support(F)>
      compute_several(Link_Functor<F>& functor);


# ifndef MLN_INCLUDE_ONLY


      template <typename F>
      object_links<scribo_support(F)>
      compute_several(Link_Functor<F>& functor_)
      {
	mln_trace("scribo::primitive::link::compute_several");

	F& functor = exact(functor_);

	for_all_comps(current_object, functor.components())
	{
	  functor.start_processing_object(current_object); //<-- start_processing_object
	  internal::find_several_links(functor, current_object);
	}

	return functor.links();
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_LINK_COMPUTE_SEVERAL_HH
