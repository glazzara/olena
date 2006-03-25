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

#ifndef EXTENDED_OPS_EXPR_HH
# define EXTENDED_OPS_EXPR_HH

# include <xtd/abstract/fun_expr.hh>
# include <xtd/mexpr.hh>
# include <xtd/math/arith.hh>




// FIXME: doc



# define xtd_internal_decl_unary_operator(OperatorName, OperatorSymbol)	\
									\
  template <typename Expr>						\
  xtd::m1expr_<xtd::OperatorName##_type, Expr>				\
  operator OperatorSymbol (const xtd::abstract::fun_expr_<Expr>& expr)	\
  {									\
    xtd::m1expr_<xtd::OperatorName##_type, Expr> tmp(expr);		\
    return tmp;								\
  }									\
									\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n


// FIXME: add something like:

//   template <typename Expr>
//   struct OperatorName##_trait_ < Expr >
//   {
//     typedef xtd::m1expr_<xtd::OperatorName##_type, Expr> ret;
//   };




# define xtd_internal_decl_binary_operator(OperatorName, OperatorSymbol)	\
										\
  template <typename Lexpr, typename Rexpr>					\
  xtd::m2expr_<xtd::OperatorName##_type, Lexpr, Rexpr>				\
  operator OperatorSymbol (const xtd::abstract::fun_expr_<Lexpr>& lexpr,	\
			   const xtd::abstract::fun_expr_<Rexpr>& rexpr)	\
  {										\
    xtd::m2expr_<xtd::OperatorName##_type, Lexpr, Rexpr> tmp(lexpr, rexpr);	\
    return tmp;									\
  }										\
										\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n


// FIXME: add something like:

//   template <typename Lexpr, typename Rexpr>
//   struct OperatorName##_trait_ < Lexpr, Rexpr >
//   {
//     typedef xtd::m2expr_<xtd::OperatorName##_type, Lexpr, Rexpr> ret;
//   };





namespace xtd
{

  // logic

  xtd_internal_decl_binary_operator( land, and );
  xtd_internal_decl_binary_operator( lor,  or );
  xtd_internal_decl_binary_operator( lxor, xor );

  xtd_internal_decl_unary_operator( lnot, not );


  // cmp

  xtd_internal_decl_binary_operator( eq,      == );
  xtd_internal_decl_binary_operator( neq,     != );
  xtd_internal_decl_binary_operator( less,    <  );
  xtd_internal_decl_binary_operator( leq,     <= );
  xtd_internal_decl_binary_operator( greater, >  );
  xtd_internal_decl_binary_operator( geq,     >= );


  // arith

  xtd_internal_decl_binary_operator( plus,  + );
  xtd_internal_decl_binary_operator( minus, - );
  xtd_internal_decl_binary_operator( mult,  * );
  xtd_internal_decl_binary_operator( div,   / );
  xtd_internal_decl_binary_operator( mod,   % );

  xtd_internal_decl_unary_operator( uminus, - );

} // end of namespace xtd


#endif // ! EXTENDED_OPS_EXPR_HH
