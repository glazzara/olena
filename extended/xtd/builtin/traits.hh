// Copyright (C) 2002, 2005, 2006  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef EXTENDED_BUILTIN_TRAITS_HH
# define EXTENDED_BUILTIN_TRAITS_HH

# include <xtd/optraits.hh>



// FIXME: this is dummy and incomplete code!


# define xtd_internal_DUMMY_builtin_arith_traits(OperatorName)						\
													\
  template <typename T>											\
  struct OperatorName##_trait_ < T, T >									\
  {													\
    typedef T ret;											\
  };													\
													\
  template <> struct OperatorName##_trait_ < float, int > { typedef float ret; };			\
  template <> struct OperatorName##_trait_ < int, float > { typedef float ret; };			\
  template <> struct OperatorName##_trait_ < double, int > { typedef double ret; };			\
  template <> struct OperatorName##_trait_ < int, double > { typedef double ret; };			\
  template <> struct OperatorName##_trait_ < double, float > { typedef double ret; };			\
  template <> struct OperatorName##_trait_ < float, double > { typedef double ret; };			\
													\
  template <> struct OperatorName##_trait_ < long double, int > { typedef long double ret; };		\
  template <> struct OperatorName##_trait_ < int, long double > { typedef long double ret; };		\
  template <> struct OperatorName##_trait_ < long double, float > { typedef long double ret; };		\
  template <> struct OperatorName##_trait_ < float, long double > { typedef long double ret; };		\
  template <> struct OperatorName##_trait_ < long double, double > { typedef long double ret; };	\
  template <> struct OperatorName##_trait_ < double, long double > { typedef long double ret; };	\
													\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n


# define xtd_internal_DUMMY_builtin_logic_traits(OperatorName)	\
								\
  template <>							\
  struct OperatorName##_trait_ < bool, bool >			\
  {								\
    typedef bool ret;						\
  };								\
								\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n


# define xtd_internal_DUMMY_builtin_cmp_traits(OperatorName)	\
								\
  template <typename T>						\
  struct OperatorName##_trait_ < T, T >				\
  {								\
    typedef bool ret;						\
  };								\
								\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n


  

namespace xtd
{

  // logic

  xtd_internal_DUMMY_builtin_logic_traits( land );
  xtd_internal_DUMMY_builtin_logic_traits( lor );
  xtd_internal_DUMMY_builtin_logic_traits( lxor );

  template <> struct lnot_trait_< bool > { typedef bool ret; };

  // cmp

  xtd_internal_DUMMY_builtin_cmp_traits( eq );
  xtd_internal_DUMMY_builtin_cmp_traits( neq );
  xtd_internal_DUMMY_builtin_cmp_traits( less );
  xtd_internal_DUMMY_builtin_cmp_traits( leq );
  xtd_internal_DUMMY_builtin_cmp_traits( greater );
  xtd_internal_DUMMY_builtin_cmp_traits( geq );


  // arith

  xtd_internal_DUMMY_builtin_arith_traits( plus );
  xtd_internal_DUMMY_builtin_arith_traits( minus );
  xtd_internal_DUMMY_builtin_arith_traits( mult );
  xtd_internal_DUMMY_builtin_arith_traits( div );
  xtd_internal_DUMMY_builtin_arith_traits( mod );

  template <> struct uminus_trait_< int > { typedef int ret; };
  template <> struct uminus_trait_< float > { typedef float ret; };
  template <> struct uminus_trait_< double > { typedef double ret; };
  template <> struct uminus_trait_< long double > { typedef long double ret; };


} // end of namespace xtd



#endif // ! EXTENDED_BUILTIN_TRAITS_HH
