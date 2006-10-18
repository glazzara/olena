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

#ifndef XTD_ABSTRACT_PLAIN_NARY_FUN_HH
# define XTD_ABSTRACT_PLAIN_NARY_FUN_HH

# include <xtd/abstract/plain_fun.hh>
# include <xtd/abstract/nary_fun.hh>


// macros

# define xtd_arg(F)  typename xtd::typedef_::arg_type::from_<xtd::fun_traits_<F> >::ret
# define xtd_arg1(F) typename xtd::typedef_::arg1_type::from_<xtd::fun_traits_<F> >::ret
# define xtd_arg2(F) typename xtd::typedef_::arg2_type::from_<xtd::fun_traits_<F> >::ret
# define xtd_arg3(F) typename xtd::typedef_::arg3_type::from_<xtd::fun_traits_<F> >::ret




namespace xtd
{

  namespace ERROR
  {
    struct xtd_fun_traits_SHOULD_DEFINE_arg_type_FOR_AN_xtd_plain_nary_fun_WITH_n_BEING_1;

    struct xtd_fun_traits_SHOULD_DEFINE_arg1_type_FOR_AN_xtd_plain_nary_fun_WITH_n_BEING_2;
    struct xtd_fun_traits_SHOULD_DEFINE_arg2_type_FOR_AN_xtd_plain_nary_fun_WITH_n_BEING_2;

    struct xtd_fun_traits_SHOULD_DEFINE_arg1_type_FOR_AN_xtd_plain_nary_fun_WITH_n_BEING_3;
    struct xtd_fun_traits_SHOULD_DEFINE_arg2_type_FOR_AN_xtd_plain_nary_fun_WITH_n_BEING_3;
    struct xtd_fun_traits_SHOULD_DEFINE_arg3_type_FOR_AN_xtd_plain_nary_fun_WITH_n_BEING_3;

    struct AN_xtd_plain_function_DOES_NOT_ACCEPT_AN_xtd_function_AS_ARGUMENT;

    // FIXME: add error messages corresponding to definitions without sense
    // FIXME: such as having arg2 when the plain function is unary

  } // end of namespace xtd::ERROR



  mlc_decl_typedef(arg_type);
  mlc_decl_typedef(arg1_type);
  mlc_decl_typedef(arg2_type);
  mlc_decl_typedef(arg3_type);


  namespace abstract
  {

    /*! \class xtd::abstract::plain_nary_fun_<n, E>
    **
    ** Abstract base class for plain functions with an explicit number
    ** of arguments.
    **
    ** Parameter n is the number of arguments with n being 0, 1, 2, or
    ** 3.
    **
    ** Parameter E is the exact type of the function.
    */

    template <unsigned n, typename E>
    class plain_nary_fun_;



    /*! \class xtd::abstract::plain_nary_fun_<0, E>
    **
    ** Abstract base class for plain functions taking no argument.
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class plain_nary_fun_< 0, E >

      : public plain_fun_<E>,
	public nary_fun_<0>

    // FIXME: add many assertions in this class and the following ones
    // FIXME: for instance, eq_< xtd_arg(E), mlc::not_found >, etc.
    {
    public:

      xtd_res(E) operator()() const
      {
	return this->exact().impl_op();
      }

    protected:
      plain_nary_fun_() {}
    };



    /*! \class xtd::abstract::plain_nary_fun_<1, E>
    **
    ** Abstract base class for plain functions taking one argument.
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class plain_nary_fun_< 1, E >

      : public plain_fun_<E>,
	public nary_fun_<1>,

	private mlc::assert_< mlc::neq_<xtd_arg(E), mlc::not_found>,
                              xtd::ERROR::xtd_fun_traits_SHOULD_DEFINE_arg_type_FOR_AN_xtd_plain_nary_fun_WITH_n_BEING_1 >
    {
    public:

      xtd_res(E) operator()(const xtd_arg(E)& arg) const
      {
	return this->exact().impl_op(arg);
      }

      // The "mutable arg" version below is dedicated to mutators.
      xtd_res(E)& operator()(xtd_arg(E)& arg) const
      {
	return this->exact().impl_op(arg);
      }

      template <typename A>
      xtd_res(E) operator()(const A& arg) const
      {
	mlc::assert_< mlc_is_not_a(A, abstract::fun_),
	              xtd::ERROR::AN_xtd_plain_function_DOES_NOT_ACCEPT_AN_xtd_function_AS_ARGUMENT >::check();
	// FIXME: check that conversion is ok
	return this->exact().impl_op(arg);
      }

    protected:
      plain_nary_fun_() {}
    };



    /*! \class xtd::abstract::plain_nary_fun_<2, E>
    **
    ** Abstract base class for plain functions taking two arguments.
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class plain_nary_fun_< 2, E >

      : public plain_fun_<E>,
	public nary_fun_<2>,

	private mlc::assert_< mlc::neq_<xtd_arg1(E), mlc::not_found>,
                              xtd::ERROR::xtd_fun_traits_SHOULD_DEFINE_arg1_type_FOR_AN_xtd_plain_nary_fun_WITH_n_BEING_2 >,

	private mlc::assert_< mlc::neq_<xtd_arg2(E), mlc::not_found>,
                              xtd::ERROR::xtd_fun_traits_SHOULD_DEFINE_arg2_type_FOR_AN_xtd_plain_nary_fun_WITH_n_BEING_2 >
    {
    public:

      xtd_res(E) operator()(const xtd_arg1(E)& arg1,
			    const xtd_arg2(E)& arg2) const
      {
	return this->exact().impl_op(arg1, arg2);
      }

      template <typename A1, typename A2>
      xtd_res(E) operator()(const A1& arg1, const A2& arg2) const
      {
	// FIXME: be more specific in the assertions below (and also in the other classes)
	mlc::assert_< mlc_is_not_a(A1, abstract::fun_),
	              xtd::ERROR::AN_xtd_plain_function_DOES_NOT_ACCEPT_AN_xtd_function_AS_ARGUMENT >::check();
	mlc::assert_< mlc_is_not_a(A2, abstract::fun_),
	              xtd::ERROR::AN_xtd_plain_function_DOES_NOT_ACCEPT_AN_xtd_function_AS_ARGUMENT >::check();
	// FIXME: check that conversions below are ok
	return this->exact().impl_op(arg1, arg2);
      }

    protected:
      plain_nary_fun_() {}
    };



    /*! \class xtd::abstract::plain_nary_fun_<3, E>
    **
    ** Abstract base class for plain functions taking three arguments.
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class plain_nary_fun_< 3, E >

      : public plain_fun_<E>,
	public nary_fun_<3>,

	private mlc::assert_< mlc::neq_<xtd_arg1(E), mlc::not_found>,
                              xtd::ERROR::xtd_fun_traits_SHOULD_DEFINE_arg1_type_FOR_AN_xtd_plain_nary_fun_WITH_n_BEING_3 >,

	private mlc::assert_< mlc::neq_<xtd_arg2(E), mlc::not_found>,
                              xtd::ERROR::xtd_fun_traits_SHOULD_DEFINE_arg2_type_FOR_AN_xtd_plain_nary_fun_WITH_n_BEING_3 >,

	private mlc::assert_< mlc::neq_<xtd_arg3(E), mlc::not_found>,
                              xtd::ERROR::xtd_fun_traits_SHOULD_DEFINE_arg3_type_FOR_AN_xtd_plain_nary_fun_WITH_n_BEING_3 >
    {
    public:

      xtd_res(E) operator()(const xtd_arg1(E)& arg1,
			    const xtd_arg2(E)& arg2,
			    const xtd_arg3(E)& arg3) const
      {
	return this->exact().impl_op(arg1, arg2, arg3);
      }

      template <typename A1, typename A2, typename A3>
      xtd_res(E) operator()(const A1& arg1,
			    const A2& arg2,
			    const A3& arg3) const
      {
	mlc::assert_< mlc_is_not_a(A1, abstract::fun_),
	              xtd::ERROR::AN_xtd_plain_function_DOES_NOT_ACCEPT_AN_xtd_function_AS_ARGUMENT >::check();
	mlc::assert_< mlc_is_not_a(A2, abstract::fun_),
	              xtd::ERROR::AN_xtd_plain_function_DOES_NOT_ACCEPT_AN_xtd_function_AS_ARGUMENT >::check();
	mlc::assert_< mlc_is_not_a(A3, abstract::fun_),
	              xtd::ERROR::AN_xtd_plain_function_DOES_NOT_ACCEPT_AN_xtd_function_AS_ARGUMENT >::check();
	// FIXME: check that conversions below are ok
	return this->exact().impl_op(arg1, arg2, arg3);
      }

    protected:
      plain_nary_fun_() {}
    };


  } // end of namespace xtd::abstract

} // end of namespace xtd



#endif // ! XTD_ABSTRACT_PLAIN_NARY_FUN_HH
