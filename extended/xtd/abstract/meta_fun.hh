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

#ifndef EXTENDED_ABSTRACT_META_FUN_HH
# define EXTENDED_ABSTRACT_META_FUN_HH

# include <mlc/pair.hh>
# include <mlc/valist.hh>

# include <xtd/abstract/exact.hh>
# include <xtd/abstract/fun.hh>
# include <xtd/abstract/nary_fun.hh>
# include <xtd/args.hh>
# include <xtd/res.hh>
# include <xtd/case.hh>




// macros

namespace xtd
{

  // FIXME: document case stuff...

  namespace tag
  {

    struct meta_1ary_fun_operator;
    struct meta_2ary_fun_operator;
    struct meta_3ary_fun_operator;

  } // end of namespace xtd::tag


  // meta_nary_fun_<1, E>::operator()(const A& a) const

  template <typename E, typename A>
  struct default_case_ < tag::meta_1ary_fun_operator,
			 mlc::pair_<E, A> >
  {
    typedef xtd_res_1(E, A) res;

    static res impl(const abstract::meta_nary_fun_<1, E>* this_,
		    const A& a)
    {
      return exact_of(this_)->impl_calc(a);
    }
  };


  // meta_nary_fun_<2, E>::operator()(const A1& a1, const A2& a2) const

  template <typename E, typename A1, typename A2>
  struct default_case_ < tag::meta_2ary_fun_operator,
			 mlc::valist_<E, A1, A2> >
  {
    typedef xtd_res_2(E, A1, A2) res;

    static res impl(const abstract::meta_nary_fun_<2, E>* this_,
		    const A1& a1, const A2& a2)
    {
      return exact_of(this_)->impl_calc(a1, a2);
    }
  };


  // meta_nary_fun_<3, E>::operator()(const A1& a1, const A2& a2, const A3& a3) const

  template <typename E, typename A1, typename A2, typename A3>
  struct default_case_ < tag::meta_3ary_fun_operator,
			 mlc::valist_<E, A1, A2, A3> >
  {
    typedef xtd_res_3(E, A1, A2, A3) res;

    static res impl(const abstract::meta_nary_fun_<3, E>* this_,
		    const A1& a1, const A2& a2, const A3& a3)
    {
      return exact_of(this_)->impl_calc(a1, a2, a3);
    }
  };


} // end of namespace xtd


namespace xtd
{

  namespace abstract
  {


    /*! \class xtd::abstract::meta_fun_<E>
    **
    ** Abstract base class for meta functions.  Parameter E is the
    ** exact type of the function.
    */

    template <typename E>
    class meta_fun_

      : public fun_<E>
    {
    protected:
      meta_fun_()
      {
	// FIXME: mlc_is_a does not work with unsigned parameter...
// 	mlc::assert_< mlc_is_a(E, xtd::abstract::meta_nary_fun_),
// 	              xtd::ERROR::YOU_SHOULD_NOT_DERIVE_DIRECTLY_FROM_xtd_meta_fun_BUT_FROM_xtd_meta_nary_fun_
// 	            >::check();
      }
    };


    /*! \class xtd::abstract::meta_nary_fun_<n, E>
    **
    ** Abstract base class for meta functions with an explicit number
    ** of arguments.
    **
    ** Parameter n is the number of arguments with n being 1, 2, or 3.
    **
    ** Parameter E is the exact type of the function.
    */

    template <unsigned n, typename E>
    class meta_nary_fun_;



    /*! \class xtd::abstract::meta_nary_fun_<0, E>
    **
    ** Abstract base class for meta functions taking no argument.
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class meta_nary_fun_< 0, E >

      : public meta_fun_<E>,
	public nary_fun_<0>
    {
    public:

      xtd_res_0(E)
      operator()() const
      {
	return exact_of(this)->impl_calc();
      }

    protected:
      meta_nary_fun_() {}
    };



    /*! \class xtd::abstract::meta_nary_fun_<1, E>
    **
    ** Abstract base class for meta functions taking one argument.
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class meta_nary_fun_< 1, E >

      : public meta_fun_<E>,
	public nary_fun_<1>
    {
    public:

      template <typename A>
      struct case_ : public xtd::case_< xtd::tag::meta_1ary_fun_operator,
					mlc::pair_<E, A> >::ret
      {};

      template <typename A>
      typename case_<A>::res
      operator()(const A& a) const
      {
	return case_<A>::impl(this, a);
      }

    protected:
      meta_nary_fun_() {}
    };



    /*! \class xtd::abstract::meta_nary_fun_<2, E>
    **
    ** Abstract base class for meta functions taking two arguments.
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class meta_nary_fun_< 2, E >

      : public meta_fun_<E>,
	public nary_fun_<2>
    {
    public:

      template <typename A1, typename A2>
      struct case_ : public xtd::case_< xtd::tag::meta_2ary_fun_operator,
					mlc::valist_<E, A1, A2> >::ret
      {};

      template <typename A1, typename A2>
      typename case_<A1, A2>::res
      operator()(const A1& a1, const A2& a2) const
      {
	return case_<A1, A2>::impl(this, a1, a2);
      }

    protected:
      meta_nary_fun_() {}
    };


    /*! \class xtd::abstract::meta_nary_fun_<3, E>
    **
    ** Abstract base class for meta functions taking three arguments.
    ** This class is defined as a specialization.
    **
    ** Parameter E is the exact type of the function.
    */

    template <typename E>
    class meta_nary_fun_< 3, E >

      : public meta_fun_<E>,
	public nary_fun_<3>
    {
    public:

      template <typename A1, typename A2, typename A3>
      struct case_ : public xtd::case_< xtd::tag::meta_3ary_fun_operator,
					mlc::valist_<E, A1, A2, A3> >::ret
      {};

      template <typename A1, typename A2, typename A3>
      typename case_<A1, A2, A3>::res
      operator()(const A1& a1, const A2& a2, const A3& a3) const
      {
	return case_<A1, A2, A3>::impl(this, a1, a2, a3);
      }

    protected:
      meta_nary_fun_() {}
    };


  } // end of namespace xtd::abstract

} // end of namespace xtd



#endif // ! EXTENDED_ABSTRACT_META_FUN_HH
