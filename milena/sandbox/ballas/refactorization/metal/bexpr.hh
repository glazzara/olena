// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_METAL_BEXPR_HH
# define MLN_METAL_BEXPR_HH

/*! \file mln/metal/bexpr.hh
 *
 * \brief Definition of types for static Boolean expressions.
 */

# include <metal/bool.hh>


# define mlc_not( B )      mln::metal::not_< B >
# define mlc_and( B1, B2 ) mln::metal::and_< B1, B2 >
# define  mlc_or( B1, B2 ) mln::metal::or_ < B1, B2 >
# define mlc_xor( B1, B2 ) mln::metal::xor_< B1, B2 >


namespace mln
{

  namespace metal
  {

    /// Negate type.
    template <typename B>
    struct not_ : bool_<( ! B::value )>
    {};


    /// And type.
    template <typename L, typename R>
    struct and_ : bool_<( L::value && R::value )>
    {};


    /// Or type.
    template <typename L, typename R>
    struct or_ : bool_<( L::value || R::value )>
    {};


    /// Xor type.
    template <typename L, typename R>
    struct xor_ : bool_<( L::value ^ R::value )>
    {};


  } // end of namespace mln::metal

} // end of namespace mln


#endif // ! MLN_METAL_BOOL_HH
