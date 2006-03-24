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

#ifndef EXTENDED_MEXPR_HH
# define EXTENDED_MEXPR_HH

# include <mlc/case.hh>
# include <mlc/assert.hh>
# include <mlc/logic.hh>
# include <mlc/is_a.hh>

# include <xtd/abstract/nary_fun.hh>
# include <xtd/abstract/fun_expr.hh>
# include <xtd/args.hh>
# include <xtd/res.hh>
# include <xtd/literal.hh>





namespace xtd
{

  namespace ERROR
  {

    struct MIXED_ARGUMENTS_IN_CALLING_A_BINARY_META_FUNCTION__EXPR_VALUE;
    struct MIXED_ARGUMENTS_IN_CALLING_A_BINARY_META_FUNCTION__VALUE_EXPR;

    struct MIXED_ARGUMENTS_IN_CALLING_A_TERNARY_META_FUNCTION__EXPR1;
    struct MIXED_ARGUMENTS_IN_CALLING_A_TERNARY_META_FUNCTION__EXPR2;
    struct MIXED_ARGUMENTS_IN_CALLING_A_TERNARY_META_FUNCTION__EXPR3;

    struct FIXME;

  } // end of namespace xtd::ERROR



  // FIXME: temporary code; move it elsewhere!

  template <unsigned n1, unsigned n2>
  struct n_max {
    static const unsigned ret = (n1 > n2 ? n1 : n2);
  };



  // m0expr_

  template <typename F>
  struct m0expr_;

  
  template <typename F>
  struct nargs_< m0expr_<F> >
  {
    static const unsigned ret = 0;
  };


  template <typename F, typename Args>
  struct expr_res_< m0expr_<F>, Args >
  {
    // f()
    typedef xtd_res_0(F) ret;
  };


  template <typename F>
  struct m0expr_

    : private mlc::assert_< mlc_is_a(F, abstract::nary_fun_<0>),
			    ERROR::FIXME >,

      private mlc::assert_< mlc_is_a(F, abstract::meta_fun_),
			    ERROR::FIXME >,

      public abstract::nary_fun_expr_< 0, m0expr_<F> >
  {
    const F f;

    m0expr_() :
      f()
    {}

    m0expr_(const F& f) :
     f(f)
    {
      mlc::assert_< mlc_is_a(F, abstract::nary_fun_<0>),
	            ERROR::FIXME >::check();
    }

    template <typename Args>
    xtd_res_0(F) impl_eval(const Args& as) const
    {
      return f();
    }
  };



  // m1expr_

  template <typename F,
	    typename Expr>
  struct m1expr_;

  
  template <typename F,
	    typename Expr>
  struct nargs_< m1expr_<F, Expr> >

    : private mlc::assert_< mlc_is_a(Expr, abstract::fun_expr_),
			    xtd::ERROR::FIXME >
  {
    static const unsigned ret = xtd_nargs(Expr);
  };


  template <typename F,
	    typename Expr,
	    typename Args>
  struct expr_res_< m1expr_<F, Expr>,
		    Args >
  {
    // f(expr.eval(as))
    typedef xtd_expr_res(Expr, Args) eval_t;
    typedef xtd_res_1(F, eval_t) ret;
  };


  template <typename F,
	    typename Expr>
  struct m1expr_

    : private mlc::assert_< mlc_is_a(F, abstract::nary_fun_<1>),
			    ERROR::FIXME >,

      private mlc::assert_< mlc_is_a(F, abstract::meta_fun_),
			    ERROR::FIXME >,

      private mlc::assert_< mlc_is_a(Expr, abstract::fun_expr_),
			    xtd::ERROR::FIXME >,

      public abstract::nary_fun_expr_< xtd_nargs(Expr),
				       m1expr_<F, Expr> >
  {
    typedef m1expr_<F, Expr> self;

    const F f;
    const Expr expr;

    m1expr_() :
      f(),
      expr()
    {}

    m1expr_(const abstract::fun_expr_<Expr>& expr) :
      f(),
      expr(exact_of(expr))
    {}

    m1expr_(const F& f, // FIXME: constraint?
	    const abstract::fun_expr_<Expr>& expr) :
      f(f),
      expr(exact_of(expr))
    {
      mlc::assert_< mlc_is_a(F, abstract::nary_fun_<1>),
	            ERROR::FIXME >::check();
    }

    template <typename Args>
    xtd_expr_res(self, Args) impl_eval(const Args& as) const
    {
      return f(expr.eval(as));
    }
  };




  // m2expr_


  template <typename F,
	    typename Expr1, typename Expr2>
  struct m2expr_;


  template <typename F,
	    typename Expr1, typename Expr2>
  struct nargs_< m2expr_<F, Expr1, Expr2> >

    : private mlc::assert_< mlc::and_< mlc_is_a(Expr1, abstract::fun_expr_),
				       mlc_is_a(Expr2, abstract::fun_expr_) >,
			    xtd::ERROR::FIXME >
  {
    static const unsigned ret = n_max< xtd_nargs(Expr1),
				       xtd_nargs(Expr2) >::ret;
  };


  template <typename F,
	    typename Expr1, typename Expr2,
	    typename Args>
  struct expr_res_< m2expr_<F, Expr1, Expr2>,
		    Args >
  {
    // f(expr1.eval(as), expr2.eval(as))
    typedef xtd_expr_res(Expr1, Args) eval1_t;
    typedef xtd_expr_res(Expr2, Args) eval2_t;
    typedef xtd_res_2(F, eval1_t, eval2_t) ret;
  };


  template <typename F,
	    typename Expr1, typename Expr2>
  struct m2expr_

    : private mlc::assert_< mlc_is_a(F, abstract::nary_fun_<2>),
			    ERROR::FIXME >,

      private mlc::assert_< mlc_is_a(F, abstract::meta_fun_),
			    ERROR::FIXME >,

      private mlc::assert_< mlc::and_< mlc_is_a(Expr1, abstract::fun_expr_),
				       mlc_is_a(Expr2, abstract::fun_expr_) >,
			    xtd::ERROR::FIXME >,

      public abstract::nary_fun_expr_< xtd_nargs(mlc_comma_2(m2expr_<F, Expr1, Expr2>)),
				       m2expr_<F, Expr1, Expr2> >
  {
    typedef m2expr_<F, Expr1, Expr2> self;

    const F f;
    const Expr1 expr1;
    const Expr2 expr2;

    m2expr_() :
      f(),
      expr1(),
      expr2()
    {}

    m2expr_(const abstract::fun_expr_<Expr1>& expr1,
	    const abstract::fun_expr_<Expr2>& expr2) :
      f(),
      expr1(exact_of(expr1)),
      expr2(exact_of(expr2))
    {}

    m2expr_(const F& f,
	    const abstract::fun_expr_<Expr1>& expr1,
	    const abstract::fun_expr_<Expr2>& expr2) :
      f(f),
      expr1(exact_of(expr1)),
      expr2(exact_of(expr2))
    {
      mlc::assert_< mlc_is_a(F, abstract::nary_fun_<2>),
	            ERROR::FIXME >::check();
    }

    template <typename Args>
    xtd_expr_res(self, Args) impl_eval(const Args& as) const
    {
      return f(expr1.eval(as), expr2.eval(as));
    }
  };



  // m3expr_

  template <typename F,
	    typename Expr1, typename Expr2, typename Expr3>
  struct m3expr_;


  template <typename F,
	    typename Expr1, typename Expr2, typename Expr3>
  struct nargs_< m3expr_<F, Expr1, Expr2, Expr3> >

    : private mlc::assert_< mlc::and_list_< mlc_is_a(Expr1, abstract::fun_expr_),
					    mlc_is_a(Expr2, abstract::fun_expr_),
					    mlc_is_a(Expr3, abstract::fun_expr_) >,
			    xtd::ERROR::FIXME >
  {
    static const unsigned ret = n_max< xtd_nargs(Expr1),
				       n_max< xtd_nargs(Expr2),
					      xtd_nargs(Expr3) >::ret >::ret;
  };


  template <typename F,
	    typename Expr1, typename Expr2, typename Expr3,
	    typename Args>
  struct expr_res_< m3expr_<F, Expr1, Expr2, Expr3>,
		    Args >
  {
    // f(expr1.eval(as), expr2.eval(as), expr3.eval(as))
    typedef xtd_expr_res(Expr1, Args) eval1_t;
    typedef xtd_expr_res(Expr2, Args) eval2_t;
    typedef xtd_expr_res(Expr3, Args) eval3_t;
    typedef xtd_res_3(F, eval1_t, eval2_t, eval3_t) ret;
  };


  template <typename F,
	    typename Expr1, typename Expr2, typename Expr3>
  struct m3expr_

    : private mlc::assert_< mlc_is_a(F, abstract::nary_fun_<3>),
			    ERROR::FIXME >,

      private mlc::assert_< mlc_is_a(F, abstract::meta_fun_),
			    ERROR::FIXME >,

      private mlc::assert_< mlc::and_list_< mlc_is_a(Expr1, abstract::fun_expr_),
					    mlc_is_a(Expr2, abstract::fun_expr_),
					    mlc_is_a(Expr3, abstract::fun_expr_) >,
			    xtd::ERROR::FIXME >,

      public abstract::nary_fun_expr_< xtd_nargs(mlc_comma_3(m3expr_<F, Expr1, Expr2, Expr3>)),
				       m3expr_<F, Expr1, Expr2, Expr3> >
  {
    typedef m3expr_<F, Expr1, Expr2, Expr3> self;

    const F f;
    const Expr1 expr1;
    const Expr2 expr2;
    const Expr3 expr3;

    m3expr_() :
      f(),
      expr1(),
      expr2(),
      expr3()
    {}

    m3expr_(const abstract::fun_expr_<Expr1>& expr1,
	    const abstract::fun_expr_<Expr2>& expr2,
	    const abstract::fun_expr_<Expr3>& expr3) :
      f(),
      expr1(exact_of(expr1)),
      expr2(exact_of(expr2)),
      expr3(exact_of(expr3))
    {}

    m3expr_(const F& f,
	    const abstract::fun_expr_<Expr1>& expr1,
	    const abstract::fun_expr_<Expr2>& expr2,
	    const abstract::fun_expr_<Expr3>& expr3) :
      f(f),
      expr1(exact_of(expr1)),
      expr2(exact_of(expr2)),
      expr3(exact_of(expr3))
    {
      mlc::assert_< mlc_is_a(F, abstract::nary_fun_<3>),
	            ERROR::FIXME >::check();
    }

    template <typename Args>
    xtd_expr_res(self, Args) impl_eval(const Args& as) const
    {
      return f(expr1.eval(as), expr2.eval(as), expr3.eval(as));
    }
  };





  // meta_nary_fun_<1, F>::operator()(abstract::fun_expr_<Expr>& expr) const

  template <typename F, typename Expr>
  struct case_< tag::meta_1ary_fun_operator,
		mlc::pair_<F, Expr>,
		1 >
    : public mlc::where_< mlc_is_a(Expr, abstract::fun_expr_) >
  {
    typedef m1expr_<F, Expr> res;

    static res impl(const abstract::meta_nary_fun_<1, F>* this_,
		    const abstract::fun_expr_<Expr>& expr)
    {
      res tmp(exact_of(*this_), expr);
      return tmp;
    }
  };



  // meta_nary_fun_<2, F>::operator()(const fun_expr_<Expr1>& expr1,
  // 				      const fun_expr_<Expr2>& expr2) const


  template <typename F, typename Expr1, typename Expr2>
  struct case_< tag::meta_2ary_fun_operator,
		mlc::valist_<F, Expr1, Expr2>,
		1 >
    : public mlc::where_< mlc::or_< mlc_is_a(Expr1, abstract::fun_expr_),
				    mlc_is_a(Expr2, abstract::fun_expr_) > >,

      private mlc::assert_< mlc::implies_< mlc_is_a(Expr1, abstract::fun_expr_),
					   mlc_is_a(Expr2, abstract::fun_expr_) >,
			    xtd::ERROR::MIXED_ARGUMENTS_IN_CALLING_A_BINARY_META_FUNCTION__EXPR_VALUE >,

      private mlc::assert_< mlc::implies_< mlc_is_a(Expr2, abstract::fun_expr_),
					   mlc_is_a(Expr1, abstract::fun_expr_) >,
			    xtd::ERROR::MIXED_ARGUMENTS_IN_CALLING_A_BINARY_META_FUNCTION__VALUE_EXPR >
  {
    typedef m2expr_<F, Expr1, Expr2> res;

    static res impl(const abstract::meta_nary_fun_<2, F>* this_,
		    const abstract::fun_expr_<Expr1>& expr1,
		    const abstract::fun_expr_<Expr2>& expr2)
      {
	res tmp(exact_of(*this_), expr1, expr2);
	return tmp;
      }

  };



  //   meta_nary_fun_<3, F>::operator()(const fun_expr_<Expr1>& expr1,
  // 				        const fun_expr_<Expr2>& expr2,
  // 				        const fun_expr_<Expr3>& expr3) const

  template <typename F, typename Expr1, typename Expr2, typename Expr3>
  struct case_< tag::meta_3ary_fun_operator,
		mlc::valist_<F, Expr1, Expr2, Expr3>,
		1 >
    : public mlc::where_< mlc::or_list_< mlc_is_a(Expr1, abstract::fun_expr_),
					 mlc_is_a(Expr2, abstract::fun_expr_),
					 mlc_is_a(Expr3, abstract::fun_expr_) > >,

      // FIXME: try to get more precise error detections below...

      private mlc::assert_< mlc::implies_< mlc_is_a(Expr1, abstract::fun_expr_),
					   mlc::and_< mlc_is_a(Expr2, abstract::fun_expr_),
						      mlc_is_a(Expr3, abstract::fun_expr_) > >,
			    xtd::ERROR::MIXED_ARGUMENTS_IN_CALLING_A_TERNARY_META_FUNCTION__EXPR1 >,

      private mlc::assert_< mlc::implies_< mlc_is_a(Expr2, abstract::fun_expr_),
					   mlc::and_< mlc_is_a(Expr1, abstract::fun_expr_),
						      mlc_is_a(Expr3, abstract::fun_expr_) > >,
			    xtd::ERROR::MIXED_ARGUMENTS_IN_CALLING_A_TERNARY_META_FUNCTION__EXPR2 >,

      private mlc::assert_< mlc::implies_< mlc_is_a(Expr3, abstract::fun_expr_),
					   mlc::and_< mlc_is_a(Expr1, abstract::fun_expr_),
						      mlc_is_a(Expr2, abstract::fun_expr_) > >,
			    xtd::ERROR::MIXED_ARGUMENTS_IN_CALLING_A_TERNARY_META_FUNCTION__EXPR3 >

  {
    typedef m3expr_<F, Expr1, Expr2, Expr3> res;

    static res impl(const abstract::meta_nary_fun_<3, F>* this_,
		    const abstract::fun_expr_<Expr1>& expr1,
		    const abstract::fun_expr_<Expr2>& expr2,
		    const abstract::fun_expr_<Expr3>& expr3)
    {
      res tmp(exact_of(*this_), expr1, expr2, expr3);
      return tmp;
    }
  };



} // end of namespace xtd



#endif // ! EXTENDED_MEXPR_HH
