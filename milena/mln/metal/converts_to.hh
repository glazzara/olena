// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_METAL_CONVERTS_TO_HH
# define MLN_METAL_CONVERTS_TO_HH

/// \file
///
/// \brief Definition of a type that checks if a type "converts to"
/// another one.

# include <mln/metal/is_a.hh>
# include <mln/metal/const.hh>
# include <mln/core/def/coord.hh>


# define mlc_converts_to(T, U) mln::metal::converts_to< T, U >



namespace mln
{

  // Forward declarations.
  namespace value {
    template <unsigned n> struct int_u;
    template <unsigned n> struct int_s;
  }


  namespace metal
  {

    namespace internal
    {

      struct eat // Required by g++-2.95 for selector to work.
      {
	template <typename T> eat(const volatile T&);
	template <typename T> eat(T&);
      };

      template <typename T, typename U>
      struct helper_converts_to_
      {
	static yes_ selector(U, int);
 	static no_  selector(eat, ...);
      };

    } // end of namespace mln::metal::internal



    /*!
      \internal
      \brief "converts-to" check.
    */
    template <typename T, typename U>
    struct converts_to : bool_<( sizeof(internal::helper_converts_to_<T, U>
					::selector(*internal::make_<mlc_const(T)>::ptr(),
						   0) )
				 ==
				 sizeof(internal::yes_) )>
    {};


    template <typename T, typename U>
    struct converts_to<T*, U*> : converts_to<T, U>
    {};

    template <typename T, typename U>
    struct converts_to<T**, U**> : false_
    {};



    // Below, a few hacks to quiet g++-3.4 warnings:

    template <> struct converts_to< float, int        > : true_ {};
    template <> struct converts_to< float, def::coord > : true_ {};
    template <unsigned n> struct converts_to< float, value::int_u<n> > : true_ {};
    template <unsigned n> struct converts_to< float, value::int_s<n> > : true_ {};

    template <> struct converts_to< double, int        > : true_ {};
    template <> struct converts_to< double, def::coord > : true_ {};
    template <unsigned n> struct converts_to< double, value::int_u<n> > : true_ {};
    template <unsigned n> struct converts_to< double, value::int_s<n> > : true_ {};

    // End of hacks.

  } // end of namespace mln::metal



} // end of namespace mln


#endif // ! MLN_METAL_CONVERTS_TO_HH
