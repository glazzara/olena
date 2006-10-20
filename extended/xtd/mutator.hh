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

#ifndef XTD_MUTATOR_HH
# define XTD_MUTATOR_HH

# include <mlc/typedef.hh>
# include <mlc/cmp.hh>

# include <xtd/abstract/plain_nary_fun.hh>
# include <xtd/abstract/mutable_fun.hh>


# define xtd_decl_mutator(MethodName)										\
														\
namespace xtd													\
{														\
														\
  namespace mutator												\
  {														\
    struct MethodName##_type;											\
														\
    mlc_decl_typedef(MethodName##_type);									\
														\
    template <typename T>											\
    struct ERROR__DECLARATION_typedef_OF_##MethodName##_type_NOT_FOUND_IN_;					\
  }														\
														\
														\
  template <typename A>												\
  struct res_< mutator::MethodName##_type, A >									\
														\
    : private mlc::assert_< mlc::neq_< mlc_typedef_in(mutator, A, MethodName##_type),				\
				       mlc::not_found >,							\
			    mutator::ERROR__DECLARATION_typedef_OF_##MethodName##_type_NOT_FOUND_IN_<A> >	\
														\
  {														\
    typedef mlc_typedef_in(mutator, A, MethodName##_type) ret;							\
  };														\
														\
  namespace mutator												\
  {														\
														\
    struct MethodName##_type											\
														\
      : public abstract::mutable_fun_< MethodName##_type >							\
    {														\
      template <typename A>											\
      typename res_<MethodName##_type, A>::ret									\
      impl_op(const A& arg) const										\
      {														\
	return arg.MethodName();										\
      }														\
														\
      template <typename A>											\
      typename res_<MethodName##_type, A>::ret &								\
      impl_op(A& arg) const											\
      {														\
	return arg.MethodName();										\
      }														\
    };														\
														\
    static const MethodName##_type MethodName;									\
														\
  }														\
														\
}														\
														\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



#endif // ! XTD_MUTATOR_HH
