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

#ifndef EXTENDED_OPTRAITS_HH
# define EXTENDED_OPTRAITS_HH

# include <xtd/traits.hh>
# include <xtd/builtin/traits.hh>



namespace xtd
{


  /// \{
  /// Tags to designate operators.

  namespace abstract
  {
    struct oper
    {};

    struct op_tag
    {};

  } // end of namespace xtd::abstract



  template <typename opname>
  struct op_

    : public abstract::oper,

      private mlc::assert_< mlc_is_a(opname, abstract::op_tag),
			    xtd::ERROR::FIXME >
  {
  };



  // logic

  struct land    : public abstract::op_tag {};
  struct lor     : public abstract::op_tag {};
  struct lxor    : public abstract::op_tag {};
  struct lnot    : public abstract::op_tag {};

  typedef op_<land> op_land;
  typedef op_<lor > op_lor;
  typedef op_<lxor> op_lxor;
  typedef op_<lnot> op_lnot;

  // cmp

  struct eq      : public abstract::op_tag {};
  struct neq     : public abstract::op_tag {};
  struct less    : public abstract::op_tag {};
  struct leq     : public abstract::op_tag {};
  struct greater : public abstract::op_tag {};
  struct geq     : public abstract::op_tag {};

  typedef op_<eq     > op_eq;
  typedef op_<neq    > op_neq;
  typedef op_<less   > op_less;
  typedef op_<leq    > op_leq;
  typedef op_<greater> op_greater;
  typedef op_<geq    > op_geq;

  // arith

  struct plus   : public abstract::op_tag {};
  struct minus  : public abstract::op_tag {};
  struct mult   : public abstract::op_tag {};
  struct div    : public abstract::op_tag {};
  struct mod    : public abstract::op_tag {};
  struct uminus : public abstract::op_tag {};

  typedef op_<plus  > op_plus;
  typedef op_<minus > op_minus;
  typedef op_<mult  > op_mult;
  typedef op_<div   > op_div;
  typedef op_<mod   > op_mod;
  typedef op_<uminus> op_uminus;

  /// \}





  template < typename name,
	     typename data >
  struct default_case_ < op_<name>, data >
  {
    typedef mlc::undefined ret;
  };


  template <typename name,
	    typename T>
  struct case_ < op_<name>, T,
		 1 > : public mlc::where_< mlc_is_builtin(T) >,

		       private mlc::assert_< mlc_is_not_a(T, mlc::pair_),
					     xtd::ERROR::FIXME >
  {
    typedef T ret; // FIXME: fake!!!
  };


  template <typename name,
	    typename L, typename R>
  struct case_ < op_<name>, mlc::pair_<L, R>,
		 1 > : public mlc::where_< mlc::and_<mlc_is_builtin(L), mlc_is_builtin(R)> >
  {
    struct protected_ {
      typedef typename xtd::switch_< internal::UAC, mlc::pair_<L, R> >::ret ret;
    };
  };



  namespace internal
  {


    template < typename name,
	       typename L, typename R >
    struct get_trait_ < op_<name>, L, R >

      : private mlc::assert_< mlc::implies_< mlc::is_defined_<   xtd::set_trait_<op_<name>, L, R> >,
					     mlc::ret_found_in_< xtd::set_trait_<op_<name>, L, R> > >,
			      xtd::ERROR::FIXME >,

	private mlc::assert_< mlc::implies_< mlc::is_undefined_< xtd::set_trait_<op_<name>, L, R> >,
					     mlc::neq_< typename get_case_< op_<name>, mlc::pair_<L, R> >::ret,
							mlc::none > >,
			      xtd::ERROR::FIXME >
    {
      typedef xtd::set_trait_< op_<name>, L, R > user_trait;
      typedef mlc_ret(user_trait) user_ret;

      typedef typename get_case_< op_<name>, mlc::pair_<L, R> >::ret case_t;
      typedef mlc_ret(case_t) case_ret;

      typedef typename mlc::if_< mlc_is_found(user_ret), user_ret, case_ret >::ret ret;
    };


    template < typename name,
	       typename T >
    struct get_trait_ < op_<name>, T >

      : private mlc::assert_< mlc::implies_< mlc::is_defined_<   xtd::set_trait_<op_<name>, T> >,
					     mlc::ret_found_in_< xtd::set_trait_<op_<name>, T> > >,
			      xtd::ERROR::FIXME >
    {
      typedef xtd::set_trait_< op_<name>, T > user_trait;
      typedef mlc_ret(user_trait) user_ret;

      typedef typename get_case_< op_<name>, T >::ret case_t;
      typedef mlc_ret(case_t) case_ret;

      typedef typename mlc::if_< mlc_is_found(user_ret), user_ret, case_ret >::ret ret;
    };

  } // end of namespace xtd::internal



} // end of namespace xtd



// logic

# define xtd_op_land_trait(L, R)  typename xtd::internal::get_trait_<xtd::op_land, L, R>::ret 
# define xtd_op_lor_trait(L, R)   typename xtd::internal::get_trait_<xtd::op_lor, L, R>::ret 
# define xtd_op_lxor_trait(L, R)  typename xtd::internal::get_trait_<xtd::op_lxor, L, R>::ret 

# define xtd_op_lnot_trait(T)     typename xtd::internal::get_trait_<xtd::op_lnot, T>::ret 


// cmp

# define xtd_op_eq_trait(L, R)      typename xtd::internal::get_trait_<xtd::op_eq, L, R>::ret 
# define xtd_op_neq_trait(L, R)     typename xtd::internal::get_trait_<xtd::op_neq, L, R>::ret 
# define xtd_op_less_trait(L, R)    typename xtd::internal::get_trait_<xtd::op_less, L, R>::ret 
# define xtd_op_leq_trait(L, R)     typename xtd::internal::get_trait_<xtd::op_leq, L, R>::ret 
# define xtd_op_greater_trait(L, R) typename xtd::internal::get_trait_<xtd::op_greater, L, R>::ret 
# define xtd_op_geq_trait(L, R)     typename xtd::internal::get_trait_<xtd::op_geq, L, R>::ret 


// arith

# define xtd_op_plus_trait(L, R)  typename xtd::internal::get_trait_<xtd::op_plus, L, R>::ret 
# define xtd_op_plus_trait_(L, R)          xtd::internal::get_trait_<xtd::op_plus, L, R>::ret 
# define xtd_op_minus_trait(L, R) typename xtd::internal::get_trait_<xtd::op_minus, L, R>::ret 
# define xtd_op_minus_trait_(L, R)         xtd::internal::get_trait_<xtd::op_minus, L, R>::ret 
# define xtd_op_mult_trait(L, R)  typename xtd::internal::get_trait_<xtd::op_mult, L, R>::ret 
# define xtd_op_mult_trait_(L, R)          xtd::internal::get_trait_<xtd::op_mult, L, R>::ret 
# define xtd_op_div_trait(L, R)   typename xtd::internal::get_trait_<xtd::op_div, L, R>::ret 
# define xtd_op_div_trait_(L, R)           xtd::internal::get_trait_<xtd::op_div, L, R>::ret 
# define xtd_op_mod_trait(L, R)   typename xtd::internal::get_trait_<xtd::op_mod, L, R>::ret 
# define xtd_op_mod_trait_(L, R)           xtd::internal::get_trait_<xtd::op_mod, L, R>::ret 

# define xtd_op_uminus_trait(T)   typename xtd::internal::get_trait_<xtd::op_uminus, T>::ret 



#endif // ! EXTENDED_OPTRAITS_HH
