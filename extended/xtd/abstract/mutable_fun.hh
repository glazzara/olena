// Copyright (C) 2006  EPITA Research and Development Laboratory
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

#ifndef XTD_ABSTRACT_MUTABLE_FUN_HH
# define XTD_ABSTRACT_MUTABLE_FUN_HH

# include <xtd/res.hh>
# include <xtd/abstract/plain_fun.hh>
# include <xtd/abstract/meta_fun.hh>


namespace xtd
{

  namespace ERROR
  {
    struct ARG_SHOULD_NOT_BE_A_PLAIN_FUN;
    struct ARG1_SHOULD_NOT_BE_A_PLAIN_FUN;
    struct ARG2_SHOULD_NOT_BE_A_PLAIN_FUN;
    struct ARG3_SHOULD_NOT_BE_A_PLAIN_FUN;

    struct ARG_SHOULD_NOT_BE_A_META_FUN;

  } // end of namespace xtd::ERROR


  namespace tag
  {

    struct mutable_fun_operator;

  } // end of namespace xtd::tag


  // mutable_fun_<E>::operator()(const A& a) const
  // mutable_fun_<E>::operator()(A& a) const

  template <typename E, typename A>
  struct default_case_ < tag::mutable_fun_operator,
			 mlc::pair_<E, A> >
  {
    typedef typename res_<E, A>::ret res;
    typedef res& mutable_res;

    static res impl(const E& target,
		    const A& a)
    {
      return target.impl_op(a);
    }

    static mutable_res impl(const E& target,
			    A& a)
    {
      return target.impl_op(a);
    }
  };


  namespace abstract
  {

    template <typename E>
    class mutable_fun_

      : public open_fun_<E>
    {
    public:

      template <typename A>
      struct lcase_ 

	: private mlc::assert_< mlc_is_not_a(A, xtd::abstract::plain_fun_),
				xtd::ERROR::ARG_SHOULD_NOT_BE_A_PLAIN_FUN >,

	  private mlc::assert_< mlc_is_not_a(A, xtd::abstract::meta_fun_),
				xtd::ERROR::ARG_SHOULD_NOT_BE_A_META_FUN >,

	  public xtd::case_< xtd::tag::mutable_fun_operator,
			     mlc::pair_<E, A> >::ret
      {};

      // Const version.
      template <typename A>
      typename lcase_<A>::res
      operator()(const A& a) const
      {
	return lcase_<A>::impl(this->exact_(), a);
      }

      // Mutable version.
      template <typename A>
      typename lcase_<A>::mutable_res
      operator()(A& a) const
      {
	return lcase_<A>::impl(this->exact_(), a);
      }

    protected:
      mutable_fun_() {}
    };

  } // end of namespace xtd::abstract


} // end of namespace xtd


#endif // ! XTD_ABSTRACT_MUTABLE_FUN_HH
