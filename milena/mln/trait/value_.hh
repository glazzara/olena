// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

/// \file mln/trait/value_.hh
///
/// Some base trait types for value types.

# include <iostream>
# include <string>

# include <mln/metal/int.hh>
# include <mln/metal/math/pow.hh>
# include <mln/metal/if.hh>

# include <mln/trait/value/all.hh>
# include <mln/metal/math/pow.hh>
# include <mln/core/def/low_quant_nbits.hh>


# define mln_trait_value_nature(V) typename mln::trait::value_< V >::nature
# define mln_trait_value_nature_(V)         mln::trait::value_< V >::nature
# define mln_trait_value_kind(V)   typename mln::trait::value_< V >::kind
# define mln_trait_value_kind_(V)           mln::trait::value_< V >::kind
# define mln_trait_value_quant(V)  typename mln::trait::value_< V >::quant
# define mln_trait_value_quant_(V)          mln::trait::value_< V >::quant


# define mln_card(V)     mln::trait::value_< V >::card
# define mln_dim(V)      mln::trait::value_< V >::dim
# define mln_epsilon(V)  mln::trait::value_< V >::epsilon()
# define mln_max(V)      mln::trait::value_< V >::max()
# define mln_min(V)      mln::trait::value_< V >::min()
# define mln_nbits(V)    mln::trait::value_< V >::nbits


/// Give the summation type for values of type \c T.
# define mln_trait_value_sum(V)   typename mln::trait::value_< V >::sum
# define mln_trait_value_sum_(V)           mln::trait::value_< V >::sum
# define mln_sum(V)  mln_trait_value_sum(V)
# define mln_sum_(V) mln_trait_value_sum_(V)

/// Give the value type name
# define mln_trait_value_name(V)         mln::trait::value_< V >::name()

/// FIXME: check that the -1 is correct
# define mln_value_quant_from_(C)					\
  mlc_if(mln::metal::bool_<( int(C) > int(mlc_pow_int(2, mln::def::low_quant_nbits)) \
			     || int(C) == 0				\
			     || int(C) == -1)>,				\
	 mln::trait::value::quant::high,				\
	 mln::trait::value::quant::low)


/// Give the cardinality of the set of values having N bits.
// 
// Technical note:
// This macro might seem weird, yet it is NOT.  We have to change the value
// of N, even when N is less than the low quant threshold, because otherwise
// the call to mlc_pow_int would lead to compilation errors.
# define mln_value_card_from_(N)		\
  (int(N) <= int(mln::def::low_quant_nbits)	\
   ? mlc_pow_int((N <= 16 ? 2 : 1),		\
		 (N <= 16 ? N : 1))		\
   : 0)



namespace mln
{

  namespace trait
  {


    struct undefined_value_
    {
      typedef undef nature;
      typedef undef kind;
      typedef undef quant;

      /*
       * enum {
       *   dim = ?
       *   nbits = ?,
       *   card  = ?
       * };
       *
       * static const E min();
       * static const E max();
       * static const E epsilon();
       */

      typedef undef sum;
      // FIXME: signed or not, with zero or not, centered or not, etc.
    };


    struct default_value_ : undefined_value_
    {
      enum {
	dim = 0,
	nbits = 0,
	card = 0
      };

      typedef trait::value::nature::unknown nature;
      typedef trait::value::kind::data      kind;
      typedef trait::value::quant::high     quant;

      /*
       * typedef ? comp_0;
       * typedef ? comp_1;
       * ...
       */
    };


    template <typename V>
    struct value_ : default_value_
    {
    };


  } // end of namespace mln::trait

} // end of namespace mln


# include <mln/trait/value/print.hh>


#endif // ! MLN_TRAIT_VALUE__HH
