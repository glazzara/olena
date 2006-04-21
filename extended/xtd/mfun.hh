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

#ifndef EXTENDED_MFUN_HH
# define EXTENDED_MFUN_HH

# include <xtd/abstract/plain_fun.hh>
# include <xtd/abstract/meta_nary_fun.hh>



// FIXME: in this file replace impl_calc by impl_op


namespace xtd
{


  namespace ERROR
  {

    struct AN_xtd_m1fun_SHOULD_TAKE_AS_PARAMETER_AN_xtd_plain_nary_fun_WITH_n_BEING_1;
    struct AN_xtd_m2fun_SHOULD_TAKE_AS_PARAMETER_AN_xtd_plain_nary_fun_WITH_n_BEING_2;
    struct AN_xtd_m3fun_SHOULD_TAKE_AS_PARAMETER_AN_xtd_plain_nary_fun_WITH_n_BEING_3;

  } // end of namespace xtd::ERROR




  // equipment for xtd::m1fun_<F>

  template <template <typename> class F>
  struct m1fun_;

  template <template <typename> class F, typename A>
  struct res_< m1fun_<F>, A >

    : private mlc::assert_< mlc::and_< mlc_is_a(F<A>, abstract::plain_fun_),
				       mlc_is_a(F<A>, abstract::nary_fun_<1>) >,
			    xtd::ERROR::AN_xtd_m1fun_SHOULD_TAKE_AS_PARAMETER_AN_xtd_plain_nary_fun_WITH_n_BEING_1 >

  {
    typedef xtd_res(F<A>) ret;
  };


  /*! \class xtd::m1fun_<F>
  **
  ** FIXME: doc
  */

  template <template <typename> class F>
  struct m1fun_ : public abstract::meta_nary_fun_< 1, m1fun_<F> >
  {
    template <typename A>
    xtd_res(F<A>) impl_calc(const A& a) const
      //          ---------
    {
      typedef F<A> F_;
      mlc::assert_< mlc::and_< mlc_is_a(F_, abstract::plain_fun_),
			       mlc_is_a(F_, abstract::nary_fun_<1>) >,
	            xtd::ERROR::AN_xtd_m1fun_SHOULD_TAKE_AS_PARAMETER_AN_xtd_plain_nary_fun_WITH_n_BEING_1
	>::check();
      static const F_ f_;
      return f_(a);
    }
  };





  // equipment for xtd::m2fun_<F>

  template <template <typename, typename> class F>
  struct m2fun_;


  template <template <typename, typename> class F,
	    typename A1, typename A2>

  struct res_< m2fun_<F>, A1, A2 >

    : private mlc::assert_< mlc::and_< mlc_is_a(mlc_comma_1(F<A1,A2>), abstract::plain_fun_),
				       mlc_is_a(mlc_comma_1(F<A1,A2>), abstract::nary_fun_<2>) >,
			    xtd::ERROR::AN_xtd_m2fun_SHOULD_TAKE_AS_PARAMETER_AN_xtd_plain_nary_fun_WITH_n_BEING_2 >
  {
    typedef F<A1,A2> F_;
    typedef xtd_res(F_) ret;
  };


  /*! \class xtd::m2fun_<F>
  **
  ** FIXME: doc
  */

  template <template <typename, typename> class F>
  struct m2fun_ : public abstract::meta_nary_fun_< 2, m2fun_<F> >
  {
    template <typename A1, typename A2>
    xtd_res(mlc_comma_1(F<A1,A2>)) impl_calc(const A1& a1, const A2& a2) const
      //                           ---------
    {
      typedef F<A1,A2> F_;
      mlc::assert_< mlc::and_< mlc_is_a(F_, abstract::plain_fun_),
			       mlc_is_a(F_, abstract::nary_fun_<2>) >,
	            xtd::ERROR::AN_xtd_m2fun_SHOULD_TAKE_AS_PARAMETER_AN_xtd_plain_nary_fun_WITH_n_BEING_2 >::check();
      static const F_ f_;
      return f_(a1, a2);
    }
  };






  // equipment for xtd::m3fun_<F>

  template <template <typename, typename, typename> class F>
  struct m3fun_;


  template <template <typename, typename, typename> class F,
	    typename A1, typename A2, typename A3>

  struct res_< m3fun_<F>, A1, A2, A3 >

    : private mlc::assert_< mlc::and_< mlc_is_a(mlc_comma_2(F<A1,A2,A3>), abstract::plain_fun_),
				       mlc_is_a(mlc_comma_2(F<A1,A2,A3>), abstract::nary_fun_<3>) >,
			    xtd::ERROR::AN_xtd_m3fun_SHOULD_TAKE_AS_PARAMETER_AN_xtd_plain_nary_fun_WITH_n_BEING_3 >
  {
    typedef F<A1,A2,A3> F_;
    typedef xtd_res(F_) ret;
  };


  /*! \class xtd::m3fun_<F>
  **
  ** FIXME: doc
  */

  template <template <typename, typename, typename> class F>
  struct m3fun_ : public abstract::meta_nary_fun_< 3, m3fun_<F> >
  {
    template <typename A1, typename A2, typename A3>
    xtd_res(mlc_comma_2(F<A1,A2,A3>)) impl_calc(const A1& a1, const A2& a2, const A3& a3) const
      //                              ---------
    {
      typedef F<A1,A2,A3> F_;
      mlc::assert_< mlc::and_< mlc_is_a(F_, abstract::plain_fun_),
			       mlc_is_a(F_, abstract::nary_fun_<3>) >,
	            xtd::ERROR::AN_xtd_m3fun_SHOULD_TAKE_AS_PARAMETER_AN_xtd_plain_nary_fun_WITH_n_BEING_3 >::check();
      static const F_ f_;
      return f_(a1, a2, a3);
    }
  };



} // end of namespace xtd



#endif // ! EXTENDED_MFUN_HH
