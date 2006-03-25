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

#ifndef EXTENDED_ABSTRACT_OPEN_NARY_FUN_HH
# define EXTENDED_ABSTRACT_OPEN_NARY_FUN_HH

# include <xtd/abstract/open_fun.hh>
# include <xtd/abstract/nary_fun.hh>
# include <xtd/res.hh>




// macros

namespace xtd
{


  namespace ERROR
  {
    struct ARG_SHOULD_NOT_BE_A_PLAIN_FUN;
    struct ARG1_SHOULD_NOT_BE_A_PLAIN_FUN;
    struct ARG2_SHOULD_NOT_BE_A_PLAIN_FUN;
    struct ARG3_SHOULD_NOT_BE_A_PLAIN_FUN;

    struct ARG_SHOULD_NOT_BE_A_META_FUN;
    struct ARG1_SHOULD_NOT_BE_A_META_FUN;
    struct ARG2_SHOULD_NOT_BE_A_META_FUN;
    struct ARG3_SHOULD_NOT_BE_A_META_FUN;

  }// end of namespace xtd



  // FIXME: document case stuff...

  namespace tag
  {

    struct fun_operator_1;
    struct fun_operator_2;
    struct fun_operator_3;

  } // end of namespace xtd::tag


  // open_nary_fun_<1, E>::operator()(const A& a) const

  template <typename E, typename A>
  struct default_case_ < tag::fun_operator_1,
			 mlc::pair_<E, A> >
  {
    typedef xtd_res_1(E, A) res;

    static res impl(const E& target,
		    const A& a)
    {
      return target.impl_calc(a);
    }
  };


  // open_nary_fun_<2, E>::operator()(const A1& a1, const A2& a2) const

  template <typename E, typename A1, typename A2>
  struct default_case_ < tag::fun_operator_2,
			 mlc::valist_<E, A1, A2> >
  {
    typedef xtd_res_2(E, A1, A2) res;

    static res impl(const E& target,
		    const A1& a1, const A2& a2)
    {
      return target.impl_calc(a1, a2);
    }
  };


  // open_nary_fun_<3, E>::operator()(const A1& a1, const A2& a2, const A3& a3) const

  template <typename E, typename A1, typename A2, typename A3>
  struct default_case_ < tag::fun_operator_3,
			 mlc::valist_<E, A1, A2, A3> >
  {
    typedef xtd_res_3(E, A1, A2, A3) res;

    static res impl(const E& target,
		    const A1& a1, const A2& a2, const A3& a3)
    {
      return target.impl_calc(a1, a2, a3);
    }
  };


} // end of namespace xtd




namespace xtd
{

  namespace abstract
  {

    /// Forward declarations.
    /// \{

    template <typename E> class plain_fun_;

    template <typename E> class meta_fun_;

    /// \}



    /*! \class xtd::abstract::open_nary_fun_<n, E>
    **
    ** FIXME: doc
    **
    ** Parameter n is the number of arguments with n being 1, 2, or 3.
    ** Parameter E is the exact type of the function.
    */

    template <unsigned n, typename E>
    class open_nary_fun_;



    /*! \class xtd::abstract::open_nary_fun_<0, E>
    **
    ** FIXME: doc
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class open_nary_fun_< 0, E >

      : public open_fun_<E>,
	public nary_fun_<0>
    {
    public:

      /*
      ** This member is not templated so its behavior at compile-time
      ** is different than open_nary_fun_<n,E>::operator() where n >
      ** 0.  Here the return type should be known before the hierarchy
      ** classes are fully compiled thus static assertions in the
      ** macro xtd_res_0 do *not* work.  An alternate macro is thus
      ** in use. 
      */

      xtd_internal_res_0(E)
      operator()() const
      {
	return this->exact_().impl_calc();
      }

    protected:
      open_nary_fun_() {}
    };



    /*! \class xtd::abstract::open_nary_fun_<1, E>
    **
    ** FIXME: doc
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class open_nary_fun_< 1, E >

      : public open_fun_<E>,
	public nary_fun_<1>
    {
    public:

      template <typename A>
      struct case_ 

	: private mlc::assert_< mlc_is_not_a(A, xtd::abstract::plain_fun_),
				xtd::ERROR::ARG_SHOULD_NOT_BE_A_PLAIN_FUN >,

	  private mlc::assert_< mlc_is_not_a(A, xtd::abstract::meta_fun_),
				xtd::ERROR::ARG_SHOULD_NOT_BE_A_META_FUN >,

	  public xtd::case_< xtd::tag::fun_operator_1,
			     mlc::pair_<E, A> >::ret
      {};

      template <typename A>
      typename case_<A>::res
      operator()(const A& a) const
      {
	return case_<A>::impl(this->exact_(), a);
      }

    protected:
      open_nary_fun_() {}
    };



    /*! \class xtd::abstract::operator_<2, E>
    **
    ** Abstract base class for meta functions taking two arguments.
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class open_nary_fun_< 2, E >

      : public open_fun_<E>,
	public nary_fun_<2>
    {
    public:

      template <typename A1, typename A2>
      struct case_

	: private mlc::assert_< mlc_is_not_a(A1, xtd::abstract::plain_fun_),
				xtd::ERROR::ARG1_SHOULD_NOT_BE_A_PLAIN_FUN >,

	  private mlc::assert_< mlc_is_not_a(A1, xtd::abstract::meta_fun_),
				xtd::ERROR::ARG1_SHOULD_NOT_BE_A_META_FUN >,

	  private mlc::assert_< mlc_is_not_a(A2, xtd::abstract::plain_fun_),
				xtd::ERROR::ARG2_SHOULD_NOT_BE_A_PLAIN_FUN >,

	  private mlc::assert_< mlc_is_not_a(A2, xtd::abstract::meta_fun_),
				xtd::ERROR::ARG2_SHOULD_NOT_BE_A_META_FUN >,

	  public xtd::case_< xtd::tag::fun_operator_2,
			     mlc::valist_<E, A1, A2> >::ret
      {};

      template <typename A1, typename A2>
      typename case_<A1, A2>::res
      operator()(const A1& a1, const A2& a2) const
      {
	return case_<A1, A2>::impl(this->exact_(), a1, a2);
      }

    protected:
      open_nary_fun_() {}
    };


    /*! \class xtd::abstract::open_nary_fun_<3, E>
    **
    ** Abstract base class for meta functions taking three arguments.
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class open_nary_fun_< 3, E >

      : public open_fun_<E>,
	public nary_fun_<3>
    {
    public:

      template <typename A1, typename A2, typename A3>
      struct case_

	: private mlc::assert_< mlc_is_not_a(A1, xtd::abstract::plain_fun_),
				xtd::ERROR::ARG1_SHOULD_NOT_BE_A_PLAIN_FUN >,

	  private mlc::assert_< mlc_is_not_a(A1, xtd::abstract::meta_fun_),
				xtd::ERROR::ARG1_SHOULD_NOT_BE_A_META_FUN >,

	  private mlc::assert_< mlc_is_not_a(A2, xtd::abstract::plain_fun_),
				xtd::ERROR::ARG2_SHOULD_NOT_BE_A_PLAIN_FUN >,

	  private mlc::assert_< mlc_is_not_a(A2, xtd::abstract::meta_fun_),
				xtd::ERROR::ARG2_SHOULD_NOT_BE_A_META_FUN >,

	  private mlc::assert_< mlc_is_not_a(A3, xtd::abstract::plain_fun_),
				xtd::ERROR::ARG3_SHOULD_NOT_BE_A_PLAIN_FUN >,

	  private mlc::assert_< mlc_is_not_a(A3, xtd::abstract::meta_fun_),
				xtd::ERROR::ARG3_SHOULD_NOT_BE_A_META_FUN >,

	  public xtd::case_< xtd::tag::fun_operator_3,
			     mlc::valist_<E, A1, A2, A3> >::ret
      {};

      template <typename A1, typename A2, typename A3>
      typename case_<A1, A2, A3>::res
      operator()(const A1& a1, const A2& a2, const A3& a3) const
      {
	return case_<A1, A2, A3>::impl(this->exact_(), a1, a2, a3);
      }

    protected:
      open_nary_fun_() {}
    };


  } // end of namespace xtd::abstract

} // end of namespace xtd



#endif // ! EXTENDED_ABSTRACT_OPEN_NARY_FUN_HH
