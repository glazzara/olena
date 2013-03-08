// Copyright (C) 2007, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_METAL_BEXPR_HH
# define MLN_METAL_BEXPR_HH

/*! \file
 *
 * \brief Definition of types for static Boolean expressions.
 */

# include <mln/metal/bool.hh>


# define mlc_not( B )      mln::metal::not_< B >
# define mlc_and( B1, B2 ) mln::metal::and_< B1, B2 >
# define  mlc_or( B1, B2 ) mln::metal::or_ < B1, B2 >
# define mlc_xor( B1, B2 ) mln::metal::xor_< B1, B2 >


namespace mln
{

  namespace metal
  {

    /*!
      \internal
      \brief Negate type.
    */
    template <typename B>
    struct not_ : bool_<( ! B::value )>
    {};


    /*!
      \internal
      \brief And type.
    */
    template <typename L, typename R>
    struct and_ : bool_<( L::value && R::value )>
    {};


    /*!
      \internal
      \brief Or type.
    */
    template <typename L, typename R>
    struct or_ : bool_<( L::value || R::value )>
    {};


    /*!
      \internal
      \brief Xor type.
    */
    template <typename L, typename R>
    struct xor_ : bool_<( L::value ^ R::value )>
    {};


  } // end of namespace mln::metal

} // end of namespace mln


#endif // ! MLN_METAL_BEXPR_HH
