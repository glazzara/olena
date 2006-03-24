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

#ifndef EXTENDED_ABSTRACT_FUN_EXPR_HH
# define EXTENDED_ABSTRACT_FUN_EXPR_HH

# include <mlc/flags.hh>
# include <mlc/bool.hh>
# include <mlc/is_a.hh>
# include <mlc/comma.hh>

# include <xtd/args.hh>
# include <xtd/res.hh>
# include <xtd/abstract/meta_fun.hh>



// FIXME: doc

# define xtd_nargs(F) xtd::internal::get_nargs_<F>::ret




namespace xtd
{

  namespace ERROR
  {

    struct INTERNAL_ILL_FORMED_CALL_TO_xtd_fun_expr_eval;
    struct SPECIALIZATION_OF_xtd_nargs_IS_FOR_xtd_fun_expr_ONLY;
    struct SPECIALIZATION_OF_xtd_nargs_NOT_FOUND_FOR_AN_xtd_fun_expr;

  } // end of namespace xtd::ERROR




  /// Forward declarations.
  /// \{

  namespace abstract
  {
    template <typename E> class fun_expr_;
  }

  template <typename F, typename Expr1, typename Expr2>
  struct m2expr_;

  template <typename F, typename Expr1, typename Expr2, typename Expr3>
  struct m3expr_;

  template <typename T>
  struct literal_expr_;

  template <unsigned i>
  struct arg_;

  /// \}



  /*! \class xtd::nargs_<F>
  **
  ** FIXME: doc
  ** FIXME: and add a mechanism so that the type of F is checked
  ** FIXME: Cf. get_res0_ in xtd/abstract/meta_fun.hh 
  */

  template <typename F>
  struct nargs_ : public mlc::undefined
  {
  };


  namespace internal
  {

    template <typename F>
    struct get_nargs_

      : // private mlc::assert_< mlc_is_a(F, xtd::abstract::fun_expr_),
	//	                 xtd::ERROR::SPECIALIZATION_OF_xtd_nargs_IS_FOR_xtd_fun_expr_ONLY >,
        // FIXME: the static assertion above does *not* compile...

	private mlc::assert_< mlc_is_not_a(xtd::nargs_<F>, mlc::undefined),
			      xtd::ERROR::SPECIALIZATION_OF_xtd_nargs_NOT_FOUND_FOR_AN_xtd_fun_expr >
    {
      static const unsigned ret = xtd::nargs_<F>::ret;
    };

  } // end of xtd::internal



  namespace abstract
  {


    /*! \class xtd::abstract::fun_expr_<E>
    **
    ** Abstract base class for function expressions.  Parameter E is
    ** the exact type of the function expression.
    **
    ** Design note: this class does not derive from xtd::abstract::any
    ** to avoid diamond inheritance since fun_expr classes are also
    ** meta_fun classes.
    */

    template <typename E>
    class fun_expr_

    // FIXME: at that point, we should verify that nargs_<E> is user-defined...

      : public fun_<E>
    {
    public:

      template <typename Args>
      xtd_expr_res(E, Args)
	eval(const Args& as) const
      {
	mlc::assert_< mlc_is_a(Args, xtd::abstract::args),
	  xtd::ERROR::INTERNAL_ILL_FORMED_CALL_TO_xtd_fun_expr_eval >::check();
	return exact_of(this)->impl_eval(as);
      }

    protected:
      fun_expr_() {}
    };






    /*! \class xtd::abstract::nary_fun_expr_<n, E>
    **
    ** Abstract base class for function expressions with an explicit
    ** number of arguments.
    **
    ** Parameter n is the number of arguments with n being 0, 1, 2, or
    ** 3.
    **
    ** Parameter E is the exact type of the function.
    */

    template <unsigned n, typename E>
    class nary_fun_expr_;



    /*! \class xtd::abstract::nary_fun_expr_<0, E>
    **
    ** Abstract base class for function expressions taking no
    ** argument.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class nary_fun_expr_< 0, E >

      : public fun_expr_<E>,
	public meta_nary_fun_<0, E>
    {
    public:

      // the return type here is not xtd_res_0(E)
      // the explanations are given with meta_nary_fun_<0, E>::operator()()
      // in file xtd/abstract/meta_fun.hh

      xtd_internal_res_0(E)
	impl_calc() const
      {
	return this->eval(mk_args());
      }

      // no bind_i method here

    protected:
      nary_fun_expr_() {}
    };



    /*! \class xtd::abstract::nary_fun_expr_<1, E>
    **
    ** Abstract base class for function expressions taking one
    ** argument.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class nary_fun_expr_< 1, E >

      : public fun_expr_<E>,
	public meta_nary_fun_<1, E>
    {
    public:

      template <typename A>
      xtd_res_1(E, A)
	impl_calc(const A& a) const
      {
	return this->eval(mk_args(a));
      }

      // no bind_i method here

    protected:
      nary_fun_expr_() {}
    };



    /*! \class xtd::abstract::nary_fun_expr_<2, E>
    **
    ** Abstract base class for function expressions taking two
    ** arguments.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class nary_fun_expr_< 2, E >

      : public fun_expr_<E>,
	public meta_nary_fun_<2, E>
    {
    public:

      template <typename A1, typename A2>
      xtd_res_2(E, A1, A2)
	impl_calc(const A1& a1, const A2& a2) const
      {
	return this->eval(mk_args(a1, a2));
      }

      template <typename T>
      m2expr_< E, literal_expr_<T>, arg_<2> >
      bind_1(const T& value) const;

      template <typename T>
      m2expr_< E, arg_<1>, literal_expr_<T> >
      bind_2(const T& value) const;

    protected:
      nary_fun_expr_() {}
    };



    /*! \class xtd::abstract::nary_fun_expr_<3, E>
    **
    ** Abstract base class for function expressions taking three
    ** arguments.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class nary_fun_expr_< 3, E >

      : public fun_expr_<E>,
	public meta_nary_fun_<3, E>
    {
    public:

      template <typename A1, typename A2, typename A3>
      xtd_res_3(E, A1, A2, A3)
	impl_calc(const A1& a1, const A2& a2, const A3& a3) const
      {
	return this->eval(mk_args(a1, a2, a3));
      }

      template <typename T>
      m3expr_< E, literal_expr_<T>, arg_<2>, arg_<3> >
      bind_1(const T& value) const;

      template <typename T>
      m3expr_< E, arg_<1>, literal_expr_<T>, arg_<3> >
      bind_2(const T& value) const;

      template <typename T>
      m3expr_< E, arg_<1>, arg_<2>, literal_expr_<T> >
      bind_3(const T& value) const;

    protected:
      nary_fun_expr_() {}
    };


  } // end of namespace xtd::abstract


} // end of namespace xtd



#endif // ! EXTENDED_ABSTRACT_FUN_EXPR_HH
