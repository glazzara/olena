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

#ifndef EXTENDED_RES_HH
# define EXTENDED_RES_HH

# include <mlc/flags.hh>
# include <mlc/bool.hh> // FIXME: should be assert.hh
# include <mlc/if.hh>
# include <mlc/is_a.hh>
# include <mlc/comma.hh>
# include <mlc/cmp.hh>
# include <mlc/ret.hh>

# include <xtd/abstract/nary_fun.hh>



// forward declarations ...............................

namespace xtd
{

  namespace abstract
  {

    template <typename E>
    class meta_fun_;

    template <unsigned n, typename E>
    class meta_nary_fun_;

    template <typename E>
    class fun_expr_;

  } // end of namespace xtd::abstract

  template <typename F,
	    typename Expr>
  struct m1expr;

  template <typename F,
	    typename Expr1, typename Expr2>
  struct m2expr;

  template <typename F,
	    typename Expr1, typename Expr2, typename Expr3>
  struct m3expr;

} // end of namespace xtd


// end of forward declarations ........................




// macros

# define xtd_res_0(F)                 typename xtd::internal::get_res_<F>::ret
# define xtd_res_1(F, A1)             typename xtd::internal::get_res_<F, A1>::ret
# define xtd_res_2(F, A1, A2)         typename xtd::internal::get_res_<F, A1, A2>::ret
# define xtd_res_3(F, A1, A2, A3)     typename xtd::internal::get_res_<F, A1, A2, A3>::ret

# define xtd_expr_res(F, Args)        typename xtd::internal::get_expr_res_<F, Args>::ret


namespace xtd
{

  namespace ERROR
  {
    struct THE_MACRO_xtd_res_0_IS_FOR_USE_ON_AN_xtd_meta_fun;
    struct THE_MACRO_xtd_res_1_IS_FOR_USE_ON_AN_xtd_meta_fun;
    struct THE_MACRO_xtd_res_2_IS_FOR_USE_ON_AN_xtd_meta_fun;
    struct THE_MACRO_xtd_res_3_IS_FOR_USE_ON_AN_xtd_meta_fun;

    struct THE_MACRO_xtd_expr_res_IS_FOR_USE_ON_AN_xtd_fun_expr;
    struct THE_MACRO_xtd_expr_res_IS_FOR_USE_WITH_AN_xtd_args;

    struct THE_MACRO_xtd_res_0_IS_FOR_USE_ON_AN_xtd_meta_nary_fun_WITH_n_BEING_0;
    struct THE_MACRO_xtd_res_1_IS_FOR_USE_ON_AN_xtd_meta_nary_fun_WITH_n_BEING_1;
    struct THE_MACRO_xtd_res_2_IS_FOR_USE_ON_AN_xtd_meta_nary_fun_WITH_n_BEING_2;
    struct THE_MACRO_xtd_res_3_IS_FOR_USE_ON_AN_xtd_meta_nary_fun_WITH_n_BEING_3;

    struct SPECIALIZATION_OF_xtd_res_NOT_FOUND;
    struct SPECIALIZATION_OF_xtd_expr_res_NOT_FOUND;

    struct NO_ret_FOUND_IN_THE_SPECIALIZATION_OF_xtd_res;
    struct NO_ret_FOUND_IN_THE_SPECIALIZATION_OF_xtd_expr_res;

    struct YOU_SHOULD_NOT_DERIVE_DIRECTLY_FROM_xtd_meta_fun_BUT_FROM_xtd_meta_nary_fun_;

    struct FIXME_NAME_THIS_ERROR ;

  } // end of namespace xtd::ERROR




  // FIXME: doc

  template <typename F,
	    typename A1 = mlc::none,
	    typename A2 = mlc::none,
	    typename A3 = mlc::none>
  struct res_ : public mlc::undefined
  {
  };


  template <typename F,
	    typename Args>
  struct expr_res_ : public mlc::undefined
  {
  };



  namespace internal
  {
    // FIXME: doc...


    // for meta_fun that are *not* fun_expr
    // ------------------------------------


    // three args

    template <typename F,
	      typename A1 = mlc::none,
	      typename A2 = mlc::none,
	      typename A3 = mlc::none>
    struct do_get_res_

      : private mlc::assert_< mlc_is_a(F, xtd::abstract::meta_fun_),
 			      xtd::ERROR::THE_MACRO_xtd_res_3_IS_FOR_USE_ON_AN_xtd_meta_fun >,
      
	private mlc::assert_< mlc::and_< mlc_is_a(F, xtd::abstract::meta_fun_),
					 mlc_is_a(F, xtd::abstract::nary_fun_<3>) >,
			      xtd::ERROR::THE_MACRO_xtd_res_3_IS_FOR_USE_ON_AN_xtd_meta_nary_fun_WITH_n_BEING_3 >,

	private mlc::assert_< mlc_is_not_a(mlc_comma_3(xtd::res_<F,A1,A2,A3>), mlc::undefined),
			      xtd::ERROR::SPECIALIZATION_OF_xtd_res_NOT_FOUND >,

	private mlc::assert_< mlc::neq_< mlc_ret(mlc_comma_3(xtd::res_<F,A1,A2,A3>)), mlc::not_found >,
                              xtd::ERROR::NO_ret_FOUND_IN_THE_SPECIALIZATION_OF_xtd_res >
    {
      typedef mlc_ret(mlc_comma_3(xtd::res_<F,A1,A2,A3>)) ret;
    };



    // no arg

    template <typename F>
    struct do_get_res_< F >

      : private mlc::assert_< mlc_is_a(F, xtd::abstract::meta_fun_),
 			      xtd::ERROR::THE_MACRO_xtd_res_0_IS_FOR_USE_ON_AN_xtd_meta_fun >,
      
	private mlc::assert_< mlc::and_< mlc_is_a(F, xtd::abstract::meta_fun_),
					 mlc_is_a(F, xtd::abstract::nary_fun_<0>) >,
			      xtd::ERROR::THE_MACRO_xtd_res_0_IS_FOR_USE_ON_AN_xtd_meta_nary_fun_WITH_n_BEING_0 >,

	private mlc::assert_< mlc_is_not_a(xtd::res_<F>, mlc::undefined),
			      xtd::ERROR::SPECIALIZATION_OF_xtd_res_NOT_FOUND >,

	private mlc::assert_< mlc::neq_< mlc_ret(xtd::res_<F>), mlc::not_found >,
                              xtd::ERROR::NO_ret_FOUND_IN_THE_SPECIALIZATION_OF_xtd_res >
    {
      typedef mlc_ret(xtd::res_<F>) ret;
    };


    // a single arg

    template <typename F, typename A1>
    struct do_get_res_< F, A1 >

      : private mlc::assert_< mlc_is_a(F, xtd::abstract::meta_fun_),
 			      xtd::ERROR::THE_MACRO_xtd_res_1_IS_FOR_USE_ON_AN_xtd_meta_fun >,
      
	private mlc::assert_< mlc::and_< mlc_is_a(F, xtd::abstract::meta_fun_),
					 mlc_is_a(F, xtd::abstract::nary_fun_<1>) >,
			      xtd::ERROR::THE_MACRO_xtd_res_1_IS_FOR_USE_ON_AN_xtd_meta_nary_fun_WITH_n_BEING_1 >,

	private mlc::assert_< mlc_is_not_a(mlc_comma_1(xtd::res_<F,A1>), mlc::undefined),
			      xtd::ERROR::SPECIALIZATION_OF_xtd_res_NOT_FOUND >,

	private mlc::assert_< mlc::neq_< mlc_ret(mlc_comma_1(xtd::res_<F,A1>)), mlc::not_found >,
                              xtd::ERROR::NO_ret_FOUND_IN_THE_SPECIALIZATION_OF_xtd_res >
    {
      typedef mlc_ret(mlc_comma_1(xtd::res_<F,A1>)) ret;
    };


    // a couple of args

    template <typename F, typename A1, typename A2>
    struct do_get_res_< F, A1, A2 >

      : private mlc::assert_< mlc_is_a(F, xtd::abstract::meta_fun_),
 			      xtd::ERROR::THE_MACRO_xtd_res_2_IS_FOR_USE_ON_AN_xtd_meta_fun >,
      
	private mlc::assert_< mlc::and_< mlc_is_a(F, xtd::abstract::meta_fun_),
					 mlc_is_a(F, xtd::abstract::nary_fun_<2>) >,
			      xtd::ERROR::THE_MACRO_xtd_res_2_IS_FOR_USE_ON_AN_xtd_meta_nary_fun_WITH_n_BEING_2 >,

	private mlc::assert_< mlc_is_not_a(mlc_comma_2(xtd::res_<F,A1,A2>), mlc::undefined),
			      xtd::ERROR::SPECIALIZATION_OF_xtd_res_NOT_FOUND >,

	private mlc::assert_< mlc::neq_< mlc_ret(mlc_comma_2(xtd::res_<F,A1,A2>)), mlc::not_found >,
                              xtd::ERROR::NO_ret_FOUND_IN_THE_SPECIALIZATION_OF_xtd_res >
    {
      typedef mlc_ret(mlc_comma_2(xtd::res_<F,A1,A2>)) ret;
    };



    // for meta_fun that *are* fun_expr
    // ------------------------------------

    template <typename F,
	      typename Args>
    struct get_expr_res_

      : private mlc::assert_< mlc_is_a(F, xtd::abstract::fun_expr_),
 			      xtd::ERROR::THE_MACRO_xtd_expr_res_IS_FOR_USE_ON_AN_xtd_fun_expr >,

        private mlc::assert_< mlc_is_a(Args, xtd::abstract::args),
 			      xtd::ERROR::THE_MACRO_xtd_expr_res_IS_FOR_USE_WITH_AN_xtd_args >,

        private mlc::assert_< mlc_is_not_a(mlc_comma_1(xtd::expr_res_<F,Args>), mlc::undefined),
			      xtd::ERROR::SPECIALIZATION_OF_xtd_expr_res_NOT_FOUND >,

	private mlc::assert_< mlc::neq_< mlc_ret(mlc_comma_1(xtd::expr_res_<F,Args>)), mlc::not_found >,
                              xtd::ERROR::NO_ret_FOUND_IN_THE_SPECIALIZATION_OF_xtd_expr_res >
    {
      typedef mlc_ret(mlc_comma_1(xtd::expr_res_<F,Args>)) ret;
    };



    // get_res_ is the entry point
    // ------------------------------------

    template <typename F,
	      typename A1 = mlc::none,
	      typename A2 = mlc::none,
	      typename A3 = mlc::none>
    struct get_res_ : public mlc::if_< mlc_is_a(F, xtd::abstract::fun_expr_),
				       get_expr_res_<F, xtd::args_<A1, A2, A3> >,
				       do_get_res_<F, A1, A2, A3> >::ret
    {};

    // FIXME: add extra assertions here(?)
    


  } // end of namespace xtd::internal

} // end of namespace xtd



#endif // ! EXTENDED_RES_HH
