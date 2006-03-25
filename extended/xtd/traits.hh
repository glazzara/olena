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

#ifndef EXTENDED_TRAITS_HH
# define EXTENDED_TRAITS_HH

# include <mlc/flags.hh>



// logic

# define xtd_land(L, R)  typename xtd::land_trait_<L, R>::ret 
# define xtd_lor(L, R)   typename xtd::lor_trait_<L, R>::ret 
# define xtd_lxor(L, R)  typename xtd::lxor_trait_<L, R>::ret 

# define xtd_lnot(T)     typename xtd::lnot_trait_<T>::ret 


// cmp

# define xtd_eq(L, R)      typename xtd::eq_trait_<L, R>::ret 
# define xtd_neq(L, R)     typename xtd::neq_trait_<L, R>::ret 
# define xtd_less(L, R)    typename xtd::less_trait_<L, R>::ret 
# define xtd_leq(L, R)     typename xtd::leq_trait_<L, R>::ret 
# define xtd_greater(L, R) typename xtd::greater_trait_<L, R>::ret 
# define xtd_geq(L, R)     typename xtd::geq_trait_<L, R>::ret 


// arith

# define xtd_plus(L, R)  typename xtd::plus_trait_<L, R>::ret 
# define xtd_minus(L, R) typename xtd::minus_trait_<L, R>::ret 
# define xtd_mult(L, R)  typename xtd::mult_trait_<L, R>::ret 
# define xtd_div(L, R)   typename xtd::div_trait_<L, R>::ret 
# define xtd_mod(L, R)   typename xtd::mod_trait_<L, R>::ret 

# define xtd_uminus(T)   typename xtd::uminus_trait_<T>::ret 



// FIXME: xtd_plus(L, R) should be xtd::INTERNAL::plus_trait_< L, R >::ret
// FIXME: which checks that the trait *is* defined



namespace xtd
{

  // logic

  template <typename L, typename R>
  struct land_trait_ : public mlc::undefined
  {
  };

  template <typename L, typename R>
  struct lor_trait_ : public mlc::undefined
  {
  };

  template <typename L, typename R>
  struct lxor_trait_ : public mlc::undefined
  {
  };

  template <typename T>
  struct lnot_trait_ : public mlc::undefined
  {
  };


  // cmp

  template <typename L, typename R>
  struct eq_trait_ : public mlc::undefined
  {
  };

  template <typename L, typename R>
  struct neq_trait_ : public mlc::undefined
  {
  };

  template <typename L, typename R>
  struct less_trait_ : public mlc::undefined
  {
  };

  template <typename L, typename R>
  struct leq_trait_ : public mlc::undefined
  {
  };

  template <typename L, typename R>
  struct greater_trait_ : public mlc::undefined
  {
  };

  template <typename L, typename R>
  struct geq_trait_ : public mlc::undefined
  {
  };


  // arith

  template <typename L, typename R>
  struct plus_trait_ : public mlc::undefined
  {
  };

  template <typename L, typename R>
  struct minus_trait_ : public mlc::undefined
  {
  };

  template <typename L, typename R>
  struct mult_trait_ : public mlc::undefined // FIXME: or "times"?
  {
  };

  template <typename L, typename R>
  struct div_trait_ : public mlc::undefined
  {
  };

  template <typename L, typename R>
  struct mod_trait_ : public mlc::undefined
  {
  };

  template <typename T>
  struct uminus_trait_ : public mlc::undefined
  {
  };



} // end of namespace xtd



#endif // ! EXTENDED_TRAITS_HH
