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

#ifndef EXTENDED_MATH_ARITH_HH
# define EXTENDED_MATH_ARITH_HH

# include <xtd/builtin/traits.hh>
# include <xtd/abstract/plain_nary_fun.hh>
# include <xtd/mfun.hh>



namespace xtd
{


  /*! \class xtd::plain_plus_<T>
  **
  ** FIXME: doc
  */

  template <typename T1, typename T2> struct plain_plus_; // fwd decl

  template <typename T1, typename T2>
  struct fun_traits_< plain_plus_<T1, T2> >
  {
    typedef T1 arg1_type;
    typedef T2 arg2_type;
    typedef xtd_plus(T1, T2) res_type;
  };
  
  template <typename T1, typename T2>
  struct plain_plus_ : public abstract::plain_nary_fun_< 2, plain_plus_<T1, T2> >
  {
    typedef plain_plus_<T1, T2> self;
    xtd_res(self) impl_op(const T1& arg1, const T2& arg2) const
    {
      return arg1 + arg2;
    }
  };

  typedef m2fun_<plain_plus_> plus_type;
  const plus_type plus;



  /*! \class xtd::plain_mult_<T>
  **
  ** FIXME: doc
  */

  template <typename T1, typename T2> struct plain_mult_; // fwd decl

  template <typename T1, typename T2>
  struct fun_traits_< plain_mult_<T1, T2> >
  {
    typedef T1 arg1_type;
    typedef T2 arg2_type;
    typedef xtd_mult(T1, T2) res_type;
  };
  
  template <typename T1, typename T2>
  struct plain_mult_ : public abstract::plain_nary_fun_< 2, plain_mult_<T1, T2> >
  {
    typedef plain_mult_<T1, T2> self;
    xtd_res(self) impl_op(const T1& arg1, const T2& arg2) const
    {
      return arg1 * arg2;
    }
  };

  typedef m2fun_<plain_mult_> mult_type;
  const mult_type mult;


} // end of namespace xtd



#endif // ! EXTENDED_MATH_ARITH_HH
