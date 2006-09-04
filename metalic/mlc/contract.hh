// Copyright (C) 2001, 2002, 2003, 2004, 2005  EPITA Research and
// Development Laboratory.
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

#ifndef METALIC_CONTRACT_HH
# define METALIC_CONTRACT_HH

# include <cassert>

// FIXME: a lot of code has been removed here for simplification
// purpose only.

# define implies(lexpr, rexpr) (not (rexpr) or (lexpr))

# define assertion(expr)     assert(expr)
# define invariant(expr)     assert(expr) 
# define precondition(expr)  assert(expr) 
# define postcondition(expr) assert(expr) 

// FIXME: Shouln't we move these macros into Static?

# define mlc_check_method_impl(WHERE, RETURN, NAME, ARG, CONST)	\
{								\
  typedef RETURN (WHERE::*meth)(ARG) CONST;			\
  meth adr = &WHERE::impl_##NAME;				\
  adr = 0;							\
}								\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n


# define mlc_check_method_impl_2(WHERE, RETURN, NAME, ARG1, ARG2, CONST) \
{									 \
  typedef RETURN (WHERE::*meth)(ARG1, ARG2) CONST;			 \
  meth adr = &WHERE::impl_##NAME;					 \
  adr = 0;								 \
}									 \
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n

#endif // ! METALIC_CONTRACT_HH
