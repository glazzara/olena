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

#ifndef XTD_ACCESSOR_HH
# define XTD_ACCESSOR_HH

# include <xtd/abstract/plain_nary_fun.hh>
# include <xtd/mfun.hh>


# define xtd_decl_accessor(MethodName)					\
									\
namespace xtd								\
{									\
									\
  namespace accessor							\
  {									\
    template <typename T> struct MethodName##_;				\
  }									\
									\
  template <typename T>							\
  struct fun_traits_< accessor::MethodName##_<T> >			\
  {									\
    typedef T arg_type;							\
    typedef typename T::MethodName##_type res_type;			\
  };									\
									\
  namespace accessor							\
  {									\
									\
    template <typename T>						\
    struct MethodName##_						\
      : public xtd::abstract::plain_nary_fun_< 1, MethodName##_<T> >	\
    {									\
      xtd_res(MethodName##_<T>) impl_op(const T& arg) const		\
      {									\
	return arg.MethodName();					\
      }									\
    };									\
									\
    typedef xtd::m1fun_<MethodName##_> MethodName##_type;		\
									\
    static const MethodName##_type MethodName;				\
									\
  }									\
									\
}									\
									\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



#endif // ! XTD_ACCESSOR_HH
