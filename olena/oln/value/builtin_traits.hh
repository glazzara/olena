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

#ifndef OLN_VALUE_BUILTIN_TRAITS_HH
# define OLN_VALUE_BUILTIN_TRAITS_HH

# include <oln/value/builtin.hh>
# include <oln/core/gen/traits.hh>


# define oln_decl_builtin_binary_trait(T1, T2, R)			\
									\
  template <typename Op>						\
  struct set_trait_< Value, T1, Op, Value, T2 > { typedef R ret; };	\
									\
  template <typename Op>						\
  struct set_trait_< Value, T2, Op, Value, T1 > { typedef R ret; };	\
									\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n


namespace oln
{

  oln_decl_builtin_binary_trait(unsigned char,  float, float);
  oln_decl_builtin_binary_trait(unsigned short, float, float);
  oln_decl_builtin_binary_trait(unsigned int,   float, float);
  oln_decl_builtin_binary_trait(unsigned long,  float, float);

  oln_decl_builtin_binary_trait(signed char,  float, float);
  oln_decl_builtin_binary_trait(signed short, float, float);
  oln_decl_builtin_binary_trait(signed int,   float, float);
  oln_decl_builtin_binary_trait(signed long,  float, float);

  oln_decl_builtin_binary_trait(unsigned char,  double, double);
  oln_decl_builtin_binary_trait(unsigned short, double, double);
  oln_decl_builtin_binary_trait(unsigned int,   double, double);
  oln_decl_builtin_binary_trait(unsigned long,  double, double);

  oln_decl_builtin_binary_trait(signed char,  double, double);
  oln_decl_builtin_binary_trait(signed short, double, double);
  oln_decl_builtin_binary_trait(signed int,   double, double);
  oln_decl_builtin_binary_trait(signed long,  double, double);

  oln_decl_builtin_binary_trait(float, double, double);

  // FIXME: To be continued...


} // end of namespace oln


#endif // ! OLN_VALUE_BUILTIN_TRAITS_HH

