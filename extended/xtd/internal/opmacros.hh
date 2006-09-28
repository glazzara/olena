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

#ifndef XTD_INTERNAL_OPMACROS_HH
# define XTD_INTERNAL_OPMACROS_HH

# include <mlc/if.hh>
# include <xtd/math/includes.hh>

# include <xtd/abstract/fun_expr.hh>
# include <xtd/mexpr.hh>
# include <xtd/literal.hh>

# include <xtd/optraits.hh>
# include <xtd/builtin/traits.hh>



# define xtd_internal_decl_unary_operator(OperatorName, OperatorSymbol)			\
											\
											\
  template <typename T>									\
  struct plain_##OperatorName##_;							\
											\
  template <typename T>									\
  struct fun_traits_< plain_##OperatorName##_<T> >					\
  {											\
    typedef T arg_type;									\
    typedef xtd_op_##OperatorName##_trait(T) res_type;					\
  };											\
											\
											\
  template <typename T>									\
  struct plain_##OperatorName##_							\
											\
    : public abstract::plain_nary_fun_< 1, plain_##OperatorName##_<T> >			\
  {											\
    typedef plain_##OperatorName##_<T> self;						\
    xtd_res(self) impl_op(const T& arg) const						\
    {											\
      return OperatorSymbol arg;							\
    }											\
  };											\
											\
											\
  typedef m1fun_<plain_##OperatorName##_> OperatorName##_type;				\
  const OperatorName##_type OperatorName;						\
											\
											\
  template <typename Expr>								\
  struct case_ < op_##OperatorName, Expr,						\
		 2 > : public mlc::where_< mlc_is_a(Expr, xtd::abstract::fun_expr_) >	\
  {											\
    struct protected_ {									\
      typedef xtd::m1expr_<xtd::OperatorName##_type, Expr> ret;				\
    };											\
  };											\
											\
											\
  template <typename Expr>								\
  xtd::m1expr_<xtd::OperatorName##_type, Expr>						\
  operator OperatorSymbol (const xtd::abstract::fun_expr_<Expr>& expr)			\
  {											\
    xtd::m1expr_<xtd::OperatorName##_type, Expr> tmp(expr);				\
    return tmp;										\
  }											\
											\
											\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n





# define xtd_internal_decl_binop_w_lit(OperatorName, OperatorSymbol, Builtin)	\
										\
										\
  template <typename Rexpr>							\
  xtd::m2expr_<xtd::OperatorName##_type, xtd::literal_expr_<Builtin>, Rexpr>	\
  operator OperatorSymbol (Builtin value,					\
			   const xtd::abstract::fun_expr_<Rexpr>& rexpr)	\
  {										\
    typedef xtd::literal_expr_<Builtin> Lexpr;					\
    Lexpr lexpr(value);								\
    xtd::m2expr_<xtd::OperatorName##_type, Lexpr, Rexpr> tmp(lexpr, rexpr);	\
    return tmp;									\
  }										\
										\
										\
  template <typename Lexpr>							\
  xtd::m2expr_<xtd::OperatorName##_type, Lexpr, xtd::literal_expr_<Builtin> >	\
  operator OperatorSymbol (const xtd::abstract::fun_expr_<Lexpr>& lexpr,	\
			   Builtin value)					\
  {										\
    typedef xtd::literal_expr_<Builtin> Rexpr;					\
    Rexpr rexpr(value);								\
    xtd::m2expr_<xtd::OperatorName##_type, Lexpr, Rexpr> tmp(lexpr, rexpr);	\
    return tmp;									\
  }										\
										\
										\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n




# define xtd_internal_decl_binary_operator(OperatorName, OperatorSymbol)			\
												\
												\
  template <typename T1, typename T2>								\
  struct plain_##OperatorName##_;								\
												\
  template <typename T1, typename T2>								\
  struct fun_traits_< plain_##OperatorName##_<T1, T2> >						\
  {												\
    typedef T1 arg1_type;									\
    typedef T2 arg2_type;									\
    typedef xtd_op_##OperatorName##_trait(T1, T2) res_type;					\
  };												\
												\
												\
  template <typename T1, typename T2>								\
  struct plain_##OperatorName##_								\
												\
    : public abstract::plain_nary_fun_< 2, plain_##OperatorName##_<T1, T2> >			\
  {												\
    typedef plain_##OperatorName##_<T1, T2> self;						\
    xtd_res(self) impl_op(const T1& arg1, const T2& arg2) const					\
    {												\
      return arg1 OperatorSymbol arg2;								\
    }												\
  };												\
												\
												\
  typedef m2fun_<plain_##OperatorName##_> OperatorName##_type;					\
  const OperatorName##_type OperatorName;							\
												\
												\
  template <typename L, typename R>								\
  struct case_ < op_##OperatorName, mlc::pair_<L, R>,						\
		 2 > : public mlc::where_< mlc::or_< mlc_is_a(L, xtd::abstract::fun_expr_),	\
						     mlc_is_a(R, xtd::abstract::fun_expr_) > >	\
  {												\
    struct protected_ {										\
      typedef typename mlc::if_< mlc_is_a(L, xtd::abstract::fun_expr_),				\
				 L,								\
				 xtd::literal_expr_<L> >::ret Lexpr;				\
												\
      typedef typename mlc::if_< mlc_is_a(R, xtd::abstract::fun_expr_),				\
				 R,								\
				 xtd::literal_expr_<R> >::ret Rexpr;				\
												\
      typedef xtd::m2expr_<xtd::OperatorName##_type, Lexpr, Rexpr> ret;				\
    };												\
  };												\
												\
												\
  template <typename Lexpr, typename Rexpr>							\
  xtd::m2expr_<xtd::OperatorName##_type, Lexpr, Rexpr>						\
  operator OperatorSymbol (const xtd::abstract::fun_expr_<Lexpr>& lexpr,			\
			   const xtd::abstract::fun_expr_<Rexpr>& rexpr)			\
  {												\
    xtd::m2expr_<xtd::OperatorName##_type, Lexpr, Rexpr> tmp(lexpr, rexpr);			\
    return tmp;											\
  }												\
												\
												\
  xtd_internal_decl_binop_w_lit(OperatorName, OperatorSymbol, int);				\
  xtd_internal_decl_binop_w_lit(OperatorName, OperatorSymbol, float);				\
  xtd_internal_decl_binop_w_lit(OperatorName, OperatorSymbol, double);				\
												\
												\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n




#endif // ! XTD_INTERNAL_OPMACROS_HH
