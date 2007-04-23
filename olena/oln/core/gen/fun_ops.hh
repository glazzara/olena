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
# include <oln/core/gen/literal.hh>



# define oln_decl_p2v_cmp_(Name, Sym)							\
											\
  template <typename L, typename R>							\
  struct p2v_##Name##_ : public Function_p2b< p2v_##Name##_<L,R> >			\
  {											\
    typedef oln_arg_of_(L) argument;							\
    typedef bool result;  /* FIXME: trait! */						\
											\
    p2v_##Name##_(const Function_p2v<L>& left, const Function_p2v<R>& right)		\
      : left_(exact(left)),								\
	right_(exact(right))								\
    {											\
    }											\
    bool operator()(argument arg) const							\
    {											\
      return this->left_(arg) Sym this->right_(arg);					\
    }											\
  private:										\
    L left_;										\
    R right_;										\
  };											\
											\
  template <typename L, typename R>							\
  p2v_##Name##_<L,R>									\
  operator Sym (const Function_p2v<L>& left, const Function_p2v<R>& right)		\
  {											\
    mlc::assert_< mlc_is_a(oln_argument(L), Point) >::check();				\
    mlc::assert_< mlc_is_a(oln_argument(R), Point) >::check();				\
    mlc::assert_equal_< oln_argument(L), oln_argument(R) >::check();			\
    p2v_##Name##_<L,R> tmp(left, right);						\
    return tmp;										\
  }											\
											\
  template <typename L, typename R>							\
  p2v_##Name##_<L, lit_p2v_<oln_argument(L), R> >					\
  operator Sym (const Function_p2v<L>& left, const literal_<R>& right)			\
  {											\
    mlc::assert_< mlc_is_a(oln_argument(L), Point) >::check();				\
    lit_p2v_<oln_argument(L), R> right_(right.value());					\
    p2v_##Name##_<L, lit_p2v_<oln_argument(L), R> > tmp(left, right_);			\
    return tmp;										\
  }											\
											\
  template <typename L>									\
  p2v_##Name##_<L, lit_p2v_<oln_argument(L), oln_result(L)> >				\
  operator Sym (const Function_p2v<L>& left, const oln_result(L)& right)		\
  {											\
    mlc::assert_< mlc_is_a(oln_argument(L), Point) >::check();				\
    lit_p2v_<oln_argument(L), oln_result(L)> right_(right);				\
    p2v_##Name##_<L, lit_p2v_<oln_argument(L), oln_result(L)> > tmp(left, right_);	\
    return tmp;										\
  }											\
											\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n



# define oln_decl_p2v_arith_(Name, Sym)						\
										\
  template <typename L, typename R>						\
  struct p2v_##Name##_ : public Function_p2v< p2v_##Name##_<L,R> >		\
  {										\
    typedef oln_arg_of_(L) argument;						\
    typedef oln_res_of_(L) result; /* FIXME: trait! */				\
										\
    p2v_##Name##_(const Function_p2v<L>& left, const Function_p2v<R>& right)	\
      : left_(exact(left)),							\
	right_(exact(right))							\
    {										\
    }										\
    result operator()(argument arg) const					\
    {										\
      return this->left_(arg) Sym this->right_(arg);				\
    }										\
  private:									\
    L left_;									\
    R right_;									\
  };										\
										\
  template <typename L, typename R>						\
  p2v_##Name##_<L,R>								\
  operator Sym (const Function_p2v<L>& left, const Function_p2v<R>& right)	\
  {										\
    mlc::assert_< mlc_is_a(oln_argument(L), Point) >::check();			\
    mlc::assert_< mlc_is_a(oln_argument(R), Point) >::check();			\
    p2v_##Name##_<L,R> tmp(left, right);					\
    return tmp;									\
  }										\
										\
  template <typename L, typename R>						\
  p2v_##Name##_<L, lit_p2v_<oln_argument(L), R> >				\
  operator Sym (const Function_p2v<L>& left, const literal_<R>& right)		\
  {										\
    mlc::assert_< mlc_is_a(oln_argument(L), Point) >::check();			\
    lit_p2v_<oln_argument(L), R> right_(right.value());				\
    p2v_##Name##_<L, lit_p2v_<oln_argument(L), R> > tmp(left, right_);		\
    return tmp;									\
  }										\
										\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n



# define oln_decl_p2v_un_(Name, Sym)					\
									\
  template <typename T>							\
  struct p2v_##Name##_ : public Function_p2v< p2v_##Name##_<T> >	\
  {									\
    typedef oln_arg_of_(T) argument;					\
    typedef oln_res_of_(T) result;  /* FIXME: trait! */			\
									\
    p2v_##Name##_(const Function_p2v<T>& oper)				\
      : oper_(exact(oper))						\
    {									\
    }									\
    result operator()(argument arg) const				\
    {									\
      return Sym this->oper_(arg);					\
    }									\
  private:								\
    T oper_;								\
  };									\
									\
  template <typename T>							\
  p2v_##Name##_<T>							\
  operator Sym (const Function_p2v<T>& oper)				\
  {									\
    p2v_##Name##_<T> tmp(oper);						\
    return tmp;								\
  }									\
									\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n



# define oln_decl_p2b_bin_(Name, Sym)						\
										\
  template <typename L, typename R>						\
  struct p2b_##Name##_ : public Function_p2b< p2b_##Name##_<L,R> >		\
  {										\
    typedef oln_arg_of_(L) argument;						\
    typedef oln_res_of_(L) result;						\
										\
    p2b_##Name##_(const Function_p2b<L>& left, const Function_p2b<R>& right)	\
      : left_(exact(left)),							\
	right_(exact(right))							\
    {										\
    }										\
    result operator()(argument arg) const					\
    {										\
      return this->left_(arg) Sym this->right_(arg);				\
    }										\
  private:									\
    L left_;									\
    R right_;									\
  };										\
										\
  template <typename L, typename R>						\
  p2b_##Name##_<L,R>								\
  operator Sym (const Function_p2b<L>& left, const Function_p2b<R>& right)	\
  {										\
    mlc::assert_< mlc_is_a(oln_argument(L), Point) >::check();			\
    mlc::assert_< mlc_is_a(oln_argument(R), Point) >::check();			\
    p2b_##Name##_<L,R> tmp(left, right);					\
    return tmp;									\
  }										\
										\
  template <typename L, typename R>						\
  p2b_##Name##_<L, lit_p2b_<oln_argument(L), R> >				\
  operator Sym (const Function_p2b<L>& left, const literal_<R>& right)		\
  {										\
    mlc::assert_< mlc_is_a(oln_argument(L), Point) >::check();			\
    lit_p2b_<oln_argument(L), R> right_(right.value());				\
    p2b_##Name##_<L, lit_p2b_<oln_argument(L), R> > tmp(left, right_);		\
    return tmp;									\
  }										\
										\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n




# define oln_decl_p2b_un_(Name, Sym)					\
									\
  template <typename T>							\
  struct p2b_##Name##_ : public Function_p2b< p2b_##Name##_<T> >	\
  {									\
    typedef oln_arg_of_(T) argument;					\
    typedef oln_res_of_(T) result; /* FIXME: trait! */			\
									\
    p2b_##Name##_(const Function_p2b<T>& oper)				\
      : oper_(exact(oper))						\
    {									\
    }									\
    result operator()(argument arg) const				\
    {									\
      return Sym this->oper_(arg);					\
    }									\
  private:								\
    T oper_;								\
  };									\
									\
  template <typename T>							\
  p2b_##Name##_<T>							\
  operator Sym (const Function_p2b<T>& oper)				\
  {									\
    p2b_##Name##_<T> tmp(oper);						\
    return tmp;								\
  }									\
									\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n



namespace oln
{

  oln_decl_p2v_cmp_( eq,      == );
  oln_decl_p2v_cmp_( not_eq,  != );
  oln_decl_p2v_cmp_( less,    <  );
  oln_decl_p2v_cmp_( leq,     <= );
  oln_decl_p2v_cmp_( greater, >  );
  oln_decl_p2v_cmp_( geq,     >= );

  oln_decl_p2v_arith_( plus,  + );
  oln_decl_p2v_arith_( minus, - );
  oln_decl_p2v_arith_( times, * );
  oln_decl_p2v_arith_( div,   / );
  oln_decl_p2v_arith_( mod,   % );

  oln_decl_p2v_un_( uminus, - );

  oln_decl_p2b_bin_( and, && );
  oln_decl_p2b_bin_(  or, || );
  oln_decl_p2b_bin_( xor, ^ );
  // FIXME: nand, nor, xnor?

  oln_decl_p2b_un_( not, ! );


} // end of namespace oln


#endif // ! OLN_CORE_GEN_FUN_OPS_HH
