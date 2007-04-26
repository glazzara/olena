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

#ifndef OLN_CORE_GEN_FUN_OPS_HH
# define OLN_CORE_GEN_FUN_OPS_HH

# include <oln/core/gen/fun.hh>
# include <oln/core/gen/constant.hh>



# define oln_decl_binary_fun_(Kind, Name, Sym)					\
										\
  template <typename L, typename R>						\
  struct Kind##_##Name##_ : public Function_##Kind< Kind##_##Name##_<L,R> >	\
  {										\
    typedef oln_argument(L) argument;						\
    typedef oln_##Name##_trait(oln_result(L), oln_result(R)) result;		\
										\
    Kind##_##Name##_(const L& left, const R& right)				\
      : left_(left),								\
	right_(right)								\
    {										\
    }										\
										\
    result operator()(argument arg) const					\
    {										\
      return this->left_(arg) Sym this->right_(arg);				\
    }										\
										\
  private:									\
    L left_;									\
    R right_;									\
  };										\
										\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n




# define oln_decl_unary_fun_(Kind, Name, Sym)					\
										\
  template <typename T>								\
  struct Kind##_##Name##_ : public Function_##Kind< Kind##_##Name##_<T> >	\
  {										\
    typedef oln_argument(T) argument;						\
    typedef oln_##Name##_trait(oln_result(T)) result;				\
										\
    Kind##_##Name##_(const T& oper)						\
      : oper_(oper)								\
    {										\
    }										\
										\
    result operator()(argument arg) const					\
    {										\
      return Sym this->oper_(arg);						\
    }										\
										\
  private:									\
    T oper_;									\
  };										\
										\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n




# define oln_decl_binary_op_(IKind, Name, Sym, OKind)					\
											\
											\
  oln_decl_binary_fun_(OKind, Name, Sym);						\
											\
											\
  template <typename L, typename R>							\
  struct set_trait_< Function_##IKind, L, Name##_id, Function_##IKind, R >		\
  {											\
    typedef OKind##_##Name##_<L, R> ret;						\
  };											\
											\
  template <typename L, typename R>							\
  oln_##Name##_trait(L, R)								\
  operator Sym (const Function_##IKind<L>& left, const Function_##IKind<R>& right)	\
  {											\
    mlc::assert_< mlc_is_a(oln_argument(L), Point) >::check();				\
    mlc::assert_< mlc_is_a(oln_argument(R), Point) >::check();				\
    mlc::assert_equal_< oln_argument(L), oln_argument(R) >::check();			\
    oln_##Name##_trait(L, R) tmp(exact(left), exact(right));				\
    return tmp;										\
  }											\
											\
											\
  template <typename L, typename R>							\
  struct set_trait_< Function_##IKind, L, Name##_id, Value, R >				\
  {											\
    typedef OKind##_##Name##_<L, constant_##IKind##_<oln_argument(L), R> > ret;		\
  };											\
											\
  template <typename L, typename R>							\
  oln_##Name##_trait(L, value_<R>)							\
  operator Sym (const Function_##IKind<L>& left, const value_<R>& right)		\
  {											\
    mlc::assert_< mlc_is_a(oln_argument(L), Point) >::check();				\
    oln_##Name##_trait(L, value_<R>) tmp(exact(left), make_value(right));		\
    return tmp;										\
  }											\
											\
											\
  template <typename L, typename R>							\
  struct set_trait_< Value, L, Name##_id, Function_##IKind, R  >			\
  {											\
    typedef OKind##_##Name##_<constant_##IKind##_<oln_argument(R), L>, R> ret;		\
  };											\
											\
  template <typename L, typename R>							\
  oln_##Name##_trait(value_<L>, R)							\
  operator Sym (const value_<L>& left, const Function_##IKind<R>& right)		\
  {											\
    mlc::assert_< mlc_is_a(oln_argument(R), Point) >::check();				\
    oln_##Name##_trait(value_<L>, R) tmp(make_value(left), exact(right));		\
    return tmp;										\
  }											\
											\
											\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n





# define oln_decl_unary_op_(IKind, Name, Sym, OKind)	\
							\
							\
  oln_decl_unary_fun_(OKind, Name, Sym);		\
							\
							\
  template <typename T>					\
  struct set_utrait_< Name##_id, Function_##IKind, T >	\
  {							\
    typedef OKind##_##Name##_<T> ret;			\
  };							\
							\
  template <typename T>					\
  oln_##Name##_trait(T)					\
  operator Sym (const Function_##IKind<T>& oper)	\
  {							\
    oln_##Name##_trait(T) tmp(exact(oper));		\
    return tmp;						\
  }							\
							\
							\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n




# define oln_decl_builtin_op_(IKind, Name, Sym, Builtin)			\
										\
  template <typename L>								\
  oln_##Name##_trait(L, value_<Builtin>)					\
  operator Sym (const Function_##IKind<L>& left, const Builtin& right)		\
  {										\
    mlc::assert_< mlc_is_a(oln_argument(L), Point) >::check();			\
    oln_##Name##_trait(L, value_<Builtin>) tmp(exact(left), make_value(right));	\
    return tmp;									\
  }										\
										\
  template <typename R>								\
  oln_##Name##_trait(value_<Builtin>, R)					\
  operator Sym (const Builtin& left, const Function_##IKind<R>& right)		\
  {										\
    mlc::assert_< mlc_is_a(oln_argument(R), Point) >::check();			\
    oln_##Name##_trait(value_<Builtin>, R) tmp(make_value(left), exact(right));	\
    return tmp;									\
  }										\
										\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n



# define oln_decl_binary_op_p2v_(Name, Sym, OKind)	\
							\
  oln_decl_binary_op_( p2v, Name, Sym, OKind );		\
							\
  oln_decl_builtin_op_( p2v, Name, Sym, unsigned );	\
  oln_decl_builtin_op_( p2v, Name, Sym, int );		\
  oln_decl_builtin_op_( p2v, Name, Sym, float );	\
  oln_decl_builtin_op_( p2v, Name, Sym, double );	\
							\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n



namespace oln
{

  //                input              output
  //                 vvv                vvv

  oln_decl_binary_op_p2v_( eq,      ==, p2b );
  oln_decl_binary_op_p2v_( neq,     !=, p2b );

  oln_decl_binary_op_p2v_( less,    < , p2b );
  oln_decl_binary_op_p2v_( leq,     <=, p2b );
  oln_decl_binary_op_p2v_( greater, > , p2b );
  oln_decl_binary_op_p2v_( geq,     >=, p2b );

  oln_decl_binary_op_p2v_( plus,    +,  p2v );
  oln_decl_binary_op_p2v_( minus,   -,  p2v );
  oln_decl_binary_op_p2v_( times,   *,  p2v );
  oln_decl_binary_op_p2v_( div,     /,  p2v );
  oln_decl_binary_op_p2v_( mod,     %,  p2v );

  oln_decl_unary_op_( p2v, uminus,  -,  p2v );

  oln_decl_binary_op_( p2b, and,    &&, p2b );
  oln_decl_binary_op_( p2b, or,     ||, p2b );
  oln_decl_binary_op_( p2b, xor,     ^, p2b );

  oln_decl_unary_op_(  p2b, not,     !, p2b );


} // end of namespace oln


  // FIXME: nand, nor, xnor?


#endif // ! OLN_CORE_GEN_FUN_OPS_HH
