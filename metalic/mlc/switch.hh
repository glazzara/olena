// Copyright (C) 2005, 2006 EPITA Research and Development Laboratory
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

#ifndef METALIC_SWITCH_HH
# define METALIC_SWITCH_HH

# include <mlc/bool.hh>
# include <mlc/typedef.hh>
# include <mlc/is_a.hh>
# include <mlc/implies.hh>
# include <mlc/comma.hh>
# include <mlc/cmp.hh>
# include <mlc/if.hh>


namespace mlc
{

  // FIXME: doc

  namespace internal
  {
    struct case_true  {};
    struct case_false {};

  } // end of namespace mlc::internal


  template <typename bexpr>
  struct where_ : public mlc_if_( typename bexpr::eval,
				  internal::case_true,
				  internal::case_false )
  {
  };



  // FIXME: doc

  template <typename context,
	    typename data,
 	    unsigned i>
  struct case_ : public undefined
  {
  };


  template <typename context,
	    typename data>
  struct case_ <context, data, 0>;


  template <typename context,
	    typename data>
  struct default_case_ : public undefined
  {
  };


  namespace ERROR
  {
    struct A_case_STATEMENT_SHOULD_DERIVE_FROM_mlc_where_;
    struct A_default_case_STATEMENT_SHOULD_NOT_DERIVE_FROM_mlc_where_;
    struct SWITCH_DOES_NOT_HAVE_A_CASE_FOR_YOUR_DATA;
    struct RESULT_IS_NOT_FOUND_IN_default_case_;
    template <unsigned i> struct RESULT_IS_NOT_FOUND_IN_case_;

  } // end of namespace mlc::ERROR


  namespace internal
  {

    template <typename context, typename data, unsigned i,
	      bool the_ith_case_derives_from_true,
	      bool the_ith_case_derives_from_false>
    struct handle_case_;



    // impossible situation

    template <typename context, typename data, unsigned i>
    struct handle_case_ <context, data, i,
			 1, 1>;



    // default case

    template <typename context, typename data>
    struct handle_default_case_

      : private assert_< implies_< mlc_is_not_a(mlc_comma_1(default_case_<context, data>),
						undefined),
				   mlc_is_not_a(mlc_comma_1(default_case_<context, data>),
						where_) >,
			 ERROR::A_default_case_STATEMENT_SHOULD_NOT_DERIVE_FROM_mlc_where_ >,

        private assert_< implies_< mlc_is_not_a(mlc_comma_1(default_case_<context, data>),
						undefined),
				   neq_<mlc_ret(mlc_comma_1(default_case_<context, data>)),
					not_found> >,
			 ERROR::RESULT_IS_NOT_FOUND_IN_default_case_ >

    {
      typedef default_case_<context, data> current_t;
      typedef typename mlc::if_<mlc_is_a(current_t, undefined),
	                        none,
	                        current_t>::ret ret;
    };



    // there is no more user-defined cases
    // so go to the default case (the last case handled here)

    template <typename context, typename data, unsigned i>
    struct handle_case_ <context, data, i,
			 0, 0>

      : private assert_< implies_< mlc_is_not_a(mlc_comma_2(case_<context, data, i>),
						undefined),
				   mlc_is_a(mlc_comma_2(case_<context, data, i>),
					    where_) >,
			 ERROR::A_case_STATEMENT_SHOULD_DERIVE_FROM_mlc_where_ >

    {
      typedef handle_default_case_<context, data> last_t;
      typedef mlc_ret(last_t) ret;
    };


    // current case is the one

    template <typename context, typename data, unsigned i>
    struct handle_case_ <context, data, i,
			 1, 0>

      : private assert_< or_< mlc_is_a(mlc_comma_2(case_<context, data, i>),
				       where_),
			      mlc_is_a(mlc_comma_2(case_<context, data, i>),
				       undefined) >,
			 ERROR::A_case_STATEMENT_IN_mlc_switch_SHOULD_DERIVE_FROM_mlc_where_ >,

        private assert_< implies_< mlc_is_not_a(mlc_comma_2(case_<context, data, i>),
						undefined),
				   neq_<mlc_ret(mlc_comma_2(case_<context, data, i>)),
					not_found> >,
			 ERROR::RESULT_IS_NOT_FOUND_IN_case_<i> >

    {
      typedef case_<context, data, i> ret;
    };



    // current case is not the one
    // so go to the next case

    template <typename context, typename data, unsigned i>
    struct handle_case_ <context, data, i,
			 0, 1>

      : private assert_< or_< mlc_is_a(mlc_comma_2(case_<context, data, i>),
				       where_),
			      mlc_is_a(mlc_comma_2(case_<context, data, i>),
				       undefined) >,
			 ERROR::A_case_STATEMENT_IN_mlc_switch_SHOULD_DERIVE_FROM_mlc_where_ >

    {
      typedef handle_case_ < context, data, i+1,
	                     mlc_is_a(mlc_comma_2(case_<context, data, i+1>),
				      internal::case_true)::value,
	                     mlc_is_a(mlc_comma_2(case_<context, data, i+1>),
				      internal::case_false)::value > next_t;
      typedef mlc_ret(next_t) ret;
    };



    // case_

    template <typename context, typename data>
    struct case_
    {
      typedef internal::handle_case_ < context, data, 1,
				       mlc_is_a(mlc_comma_2(case_<context, data, 1>),
						internal::case_true)::value,
				       mlc_is_a(mlc_comma_2(case_<context, data, 1>),
						internal::case_false)::value > handle_t;
      typedef mlc_ret(handle_t) ret;
    };


  } // end of namespace mlc::internal



  // FIXME: doc

  template <typename context, typename data>
  struct case_
    : private assert_< neq_< mlc_comma_1(typename internal::case_<context, data>::ret),
			     none >,
		       ERROR::SWITCH_DOES_NOT_HAVE_A_CASE_FOR_YOUR_DATA >
  {
    typedef typename internal::case_<context, data>::ret ret;
  };


  // FIXME: doc

  template <typename context, typename data>
  struct switch_
    : private assert_< neq_< mlc_comma_1(typename internal::case_<context, data>::ret),
			     none >,
		       ERROR::SWITCH_DOES_NOT_HAVE_A_CASE_FOR_YOUR_DATA >,

      private assert_< neq_< mlc_ret(),
			     not_found >,
		       ERROR::SWITCH_
  {
    typedef typename internal::case_<context, data>::ret case_t;
    typedef mlc_ret(case_t) ret;
  };


} // end of namespace mlc


#endif // ! METALIC_SWITCH_HH
