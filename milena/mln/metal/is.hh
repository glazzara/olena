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

#ifndef MLN_METAL_IS_HH
# define MLN_METAL_IS_HH

/// \file
///
/// \brief Definition of a type that means "is".

# include <mln/metal/is_a.hh>


# define mlc_is(T, U) mln::metal::is< T, U >



namespace mln
{

  namespace metal
  {

    namespace internal
    {

      template <typename T, typename U>
      struct helper_is_
      {
	static yes_ selector(U*const);
	static no_  selector(...);
      };

    } // end of namespace mln::metal::internal



    /*!
      \internal
      \brief "is" check.
      Check whether T inherits from U.
    */
    template <typename T, typename U>
    struct is : bool_<( sizeof(internal::helper_is_<T, U>::selector(internal::make_<T>::ptr()))
			==
			sizeof(internal::yes_) )>
    {
    };

    template <typename T, typename U>
    struct is< const T, const U > : is<T, U>::eval
    {};

    template <typename T, typename U>
    struct is< T*, U* > : is<T, U>::eval
    {};

    template <typename T, typename U>
    struct is< T&, U& > : is<T, U>::eval
    {};

    template <typename T, typename U>
    struct is< T**, U** > : false_
    {};

  } // end of namespace mln::metal

} // end of namespace mln


#endif // ! MLN_METAL_IS_HH
