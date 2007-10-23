// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
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

#ifndef MLN_VALUE_BUILTIN_INTEGERS_HH
# define MLN_VALUE_BUILTIN_INTEGERS_HH

/*! \file mln/value/builtin/integers.hh
 *
 * \brief Some definitions about built-in integer types.
 */

# include <mln/value/concept/built_in.hh>
# include <mln/value/concept/integer.hh>
# include <mln/trait/value_.hh>

# include <mln/metal/int.hh>
# include <mln/metal/if.hh>
# include <mln/metal/bool.hh>


namespace mln
{

  template <> struct category< unsigned char  > { typedef value::Built_In<void*> ret; typedef value::Integer<void> super; };
  template <> struct category<   signed char  > { typedef value::Built_In<void*> ret; typedef value::Integer<void> super; };
  template <> struct category< unsigned short > { typedef value::Built_In<void*> ret; typedef value::Integer<void> super; };
  template <> struct category<   signed short > { typedef value::Built_In<void*> ret; typedef value::Integer<void> super; };
  template <> struct category< unsigned int   > { typedef value::Built_In<void*> ret; typedef value::Integer<void> super; };
  template <> struct category<   signed int   > { typedef value::Built_In<void*> ret; typedef value::Integer<void> super; };
  template <> struct category< unsigned long  > { typedef value::Built_In<void*> ret; typedef value::Integer<void> super; };
  template <> struct category<   signed long  > { typedef value::Built_In<void*> ret; typedef value::Integer<void> super; };


  namespace trait
  {

    // FIXME: Move the definitions below elsewhere.

    namespace internal
    {

      template < bool small_n /* == true */, unsigned n_bits >
      struct helper_card_
      {
	typedef metal::math::pow_int<2, n_bits> pow_;
	typedef metal::int_<pow_::value> card;
      };

      template < unsigned n_bits >
      struct helper_card_< false, n_bits >
      {
	typedef metal::int_<0> card;
      };

    } // end of namespace mln::trait::internal


    template <unsigned n_bits>
    struct value_integer_
    {
      typedef typename internal::helper_card_<(n_bits <= 16), n_bits>::card card;

      typedef metal::int_<n_bits>             nbits;
      typedef trait::value::nature::integer   nature;
      typedef trait::value::kind::data        kind;
      typedef mln_value_quant_from_card(card) quant;
      typedef float                           sum;
    };


    template <> struct value_< unsigned char  > : value_integer_< 8 * sizeof(unsigned char)  > {};
    template <> struct value_<   signed char  > : value_integer_< 8 * sizeof(  signed char)  > {};
    template <> struct value_< unsigned short > : value_integer_< 8 * sizeof(unsigned short) > {};
    template <> struct value_<   signed short > : value_integer_< 8 * sizeof(  signed short) > {};
    template <> struct value_< unsigned int   > : value_integer_< 8 * sizeof(unsigned int)   > {};
    template <> struct value_<   signed int   > : value_integer_< 8 * sizeof(  signed int)   > {};
    template <> struct value_< unsigned long  > : value_integer_< 8 * sizeof(unsigned long)  > {};
    template <> struct value_<   signed long  > : value_integer_< 8 * sizeof(  signed long)  > {};

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_VALUE_BUILTIN_INTEGERS_HH
