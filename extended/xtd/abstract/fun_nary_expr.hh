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

#ifndef EXTENDED_ABSTRACT_FUN_NARY_EXPR_HH
# define EXTENDED_ABSTRACT_FUN_NARY_EXPR_HH

# include <xtd/abstract/fun_expr.hh>
# include <xtd/abstract/open_nary_fun.hh>



namespace xtd
{


  /// Forward declarations.
  /// \{

  template <typename F, typename Expr1, typename Expr2>
  struct m2expr_;

  template <typename F, typename Expr1, typename Expr2, typename Expr3>
  struct m3expr_;

  template <typename T>
  struct literal_expr_;

  template <unsigned i>
  struct arg_;

  /// \}




  namespace abstract
  {

    /*! \class xtd::abstract::fun_nary_expr_<n, E>
    **
    ** Abstract base class for function expressions with an explicit
    ** number of arguments.
    **
    ** For "open_nary_fun_<n, E>::operator()(arg1,...)" we provide
    ** "::impl_calc(arg1,...)".  This method first packs the argument list:
    ** precisely "(arg1,...)" becomes "(arglist)" where the type of
    ** "arglist" is "args_<Arg1,...>".  Then the computation is delegated
    ** to "fun_expr_<E>::eval(arglist)".
    **
    ** Parameter n is the number of arguments with n being 0, 1, 2, or
    ** 3.  Parameter E is the exact type of the function.
    */

    template <unsigned n, typename E>
    class fun_nary_expr_;



    /*! \class xtd::abstract::fun_nary_expr_<0, E>
    **
    ** Abstract base class for function expressions taking no
    ** argument.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class fun_nary_expr_< 0, E >

      : public fun_expr_<E>,
	public open_nary_fun_<0, E>
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
      fun_nary_expr_() {}
    };



    /*! \class xtd::abstract::fun_nary_expr_<1, E>
    **
    ** Abstract base class for function expressions taking one
    ** argument.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class fun_nary_expr_< 1, E >

      : public fun_expr_<E>,
	public open_nary_fun_<1, E>
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
      fun_nary_expr_() {}
    };



    /*! \class xtd::abstract::fun_nary_expr_<2, E>
    **
    ** Abstract base class for function expressions taking two
    ** arguments.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class fun_nary_expr_< 2, E >

      : public fun_expr_<E>,
	public open_nary_fun_<2, E>
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
      bind_1(T value) const;

      template <typename T>
      m2expr_< E, arg_<1>, literal_expr_<T> >
      bind_2(T value) const;

    protected:
      fun_nary_expr_() {}
    };



    /*! \class xtd::abstract::fun_nary_expr_<3, E>
    **
    ** Abstract base class for function expressions taking three
    ** arguments.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class fun_nary_expr_< 3, E >

      : public fun_expr_<E>,
	public open_nary_fun_<3, E>
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
      bind_1(T value) const;

      template <typename T>
      m3expr_< E, arg_<1>, literal_expr_<T>, arg_<3> >
      bind_2(T value) const;

      template <typename T>
      m3expr_< E, arg_<1>, arg_<2>, literal_expr_<T> >
      bind_3(T value) const;

    protected:
      fun_nary_expr_() {}
    };


  } // end of namespace xtd::abstract


} // end of namespace xtd


# include <xtd/bind.hh>


#endif // ! EXTENDED_ABSTRACT_FUN_NARY_EXPR_HH
