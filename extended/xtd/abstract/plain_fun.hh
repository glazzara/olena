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

#ifndef EXTENDED_ABSTRACT_PLAIN_FUN_HH
# define EXTENDED_ABSTRACT_PLAIN_FUN_HH

# include <xtd/abstract/fun.hh>


// macro

# define xtd_res(F)  typename xtd::typedef_::res_type::from_<xtd::fun_traits_<F> >::ret




namespace xtd
{

  namespace ERROR
  {
    struct SPECIALIZATION_OF_xtd_fun_traits_NOT_FOUND_FOR_AN_xtd_plain_fun;
    struct xtd_fun_traits_SHOULD_DEFINE_res_type_FOR_AN_xtd_plain_fun;
    struct YOU_SHOULD_NOT_DERIVE_FROM_xtd_plain_fun_BUT_FROM_xtd_plain_nary_fun;

  } // end of namespace xtd::ERROR



  template <typename F>
  struct fun_traits_ : public mlc::undefined
  {
  };


  mlc_decl_typedef(res_type);


  namespace abstract
  {

    /*! \class xtd::abstract::plain_fun_
    **
    ** Abstract base class for plain functions.  Parameter E is the
    ** exact type of the function.
    */

    template <typename E>
    class plain_fun_

      : private mlc::assert_< mlc_is_not_a(xtd::fun_traits_<E>, mlc::undefined),
			      xtd::ERROR::SPECIALIZATION_OF_xtd_fun_traits_NOT_FOUND_FOR_AN_xtd_plain_fun >,

	private mlc::assert_< mlc::neq_<xtd_res(E), mlc::not_found>,
                              xtd::ERROR::xtd_fun_traits_SHOULD_DEFINE_res_type_FOR_AN_xtd_plain_fun >,

	public fun_<E>
    {
    protected:
      plain_fun_(){
	// FIXME: unsigned is parameter so mlc_is_a does not work
	// 	mlc::assert_< mlc_is_a(E, plain_nary_fun_),
	// 	              xtd::ERROR::YOU_SHOULD_NOT_DERIVE_FROM_xtd_plain_fun_BUT_FROM_xtd_plain_nary_fun
	// 	  >::check();
      }
    };


  } // end of namespace xtd::abstract

} // end of namespace xtd



#endif // ! EXTENDED_ABSTRACT_PLAIN_FUN_HH
