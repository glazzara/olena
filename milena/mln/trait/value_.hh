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

#ifndef MLN_TRAIT_VALUE__HH
# define MLN_TRAIT_VALUE__HH

/*! \file mln/trait/value_.hh
 *
 * \brief Some base trait types for value types.
 */

# include <iostream>
# include <string>

# include <mln/metal/math/pow.hh>
# include <mln/metal/if.hh>

# include <mln/trait/value/all.hh>


# define mln_trait_value_nature(V) typename mln::trait::value_< V >::nature
# define mln_trait_value_kind(V)   typename mln::trait::value_< V >::kind
# define mln_trait_value_quant(V)  typename mln::trait::value_< V >::quant
# define mln_trait_value_card(V)   typename mln::trait::value_< V >::card


# define mln_value_quant_from_card(C)					\
   mlc_if(mln::metal::bool_<( C::value > 65536 || C::value == 0 )>,	\
	  mln::trait::value::quant::high,				\
	  mln::trait::value::quant::low)



namespace mln
{

  namespace trait
  {


    struct undefined_value_
    {
      typedef undef nature;
      typedef undef kind;
      typedef undef quant;
      typedef undef card;
      typedef undef sum;
      // FIXME: signed or not, with zero or not, centered or not, etc.
    };


    struct default_value_
    {
      typedef trait::value::nature::unknown nature;
      typedef trait::value::kind::data      kind;
      typedef trait::value::quant::high     quant;
      typedef metal::int_<0>                card;
      typedef undef                         sum;
    };


    template <typename V>
    struct value_ : undefined_value_
    {
    };


    template <unsigned n_bits, int card_ = 1>
    struct value_integer_
    {
      typedef metal::math::pow_int<2, n_bits> pow_;

      typedef metal::int_<n_bits>             nbits;
      typedef trait::value::nature::integer   nature;
      typedef trait::value::kind::data        kind;
      typedef metal::int_<pow_::value>        card;
      typedef mln_value_quant_from_card(card) quant;
      typedef float                           sum;
    };

    template <unsigned n_bits>
    struct value_integer_< n_bits, 0 >
    {
      typedef metal::int_<n_bits>    nbits;
      typedef trait::value::nature::integer nature;
      typedef value::kind::data      kind;
      typedef metal::int_<0>         card;
      typedef value::quant::high     quant;
      typedef float                  sum;
    };


  } // end of namespace mln::trait

} // end of namespace mln


# include <mln/trait/value/print.hh>


#endif // ! MLN_TRAIT_VALUE__HH
