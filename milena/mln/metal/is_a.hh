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

#ifndef MLN_METAL_IS_A_HH
# define MLN_METAL_IS_A_HH

/// \file
///
/// \brief Definition of a type that means "is_a".

# include <mln/metal/bool.hh>


/*!
  \brief Expand to a "metalic" boolean expression stating whether \a
  T is a subclass of \a M or not.

  In the current implementation, \a M must be template class with
  exactly one, non template parameter.

  This macro is the recommended user interface of the "is_a"
  facility.
*/
# define mlc_is_a(T, M) mln::metal::is_a<T, M>


// Allows to write somthing like "mlc_is_a(T<P1,P2>, M)" without
// getting trouble with the preprocessor due to the 1st comma.
# define mlc_is_a__1comma(Tleft, Tright, M) mln::metal::is_a< Tleft,Tright , M >



namespace mln
{

  namespace metal
  {

    namespace internal
    {

      typedef char yes_;
      struct no_ { char dummy[2]; };

      template <typename T>
      struct make_
      {
	static T* ptr();
      };

      template <typename T>
      struct make_< T& >
      {
	static T* ptr();
      };

      template <typename T, template <class> class M>
      struct helper_is_a_
      {

	template<class V>
	static yes_ selector(M<V>*);
	static no_  selector(...);
      };

    } // end of namespace mln::metal::internal



    /*!
      \internal
      \brief "is_a" check.

      Check whether T inherits from _CONCEPT_ M.
    */
    template <typename T, template <class> class M>
    struct is_a : bool_<( sizeof( internal::helper_is_a_< T, M >::selector(internal::make_< T >::ptr()) )
			  ==
			  sizeof( internal::yes_ )  )>
    {};

    template <typename T, template <class> class M>
    struct is_a< const T, M > : is_a< T, M >::eval
    {};

    template <typename T, template <class> class M>
    struct is_a< T&, M > : is_a< T, M >::eval
    {};

    template <typename T, template <class> class M>
    struct is_a< const T&, M > : is_a< T, M >::eval
    {};


  } // end of namespace mln::metal

} // end of namespace mln


#endif // ! MLN_METAL_IS_A_HH
