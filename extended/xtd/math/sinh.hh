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

#ifndef EXTENDED_MATH_SINH_HH
# define EXTENDED_MATH_SINH_HH

# include <xtd/math/includes.hh>


namespace xtd
{

  /// \{
  /// Definition of xtd hyperbolic sine functions.


  template <typename T> struct sinh_;

  template <> struct constraints_< sinh_, float > {};
  template <> struct constraints_< sinh_, double > {};
  template <> struct constraints_< sinh_, long double > {};

  template <typename T>
  struct fun_traits_< sinh_<T> >
  {
    typedef T arg_type;
    typedef T res_type;
  };


  template <typename T>
  struct sinh_

    : public abstract::plain_nary_fun_< 1, sinh_<T> >,

      private constraints_< sinh_, T >
  {
    xtd_res(sinh_<T>) impl_op(const T& arg) const
    {
      return std::sinh(arg);
    }
  };


  typedef m1fun_<sinh_> sinh_type;


  /// \}



  /// \brief Plain function object xtd::sinh_f : float -> float.
  const sinh_<float>       sinh_f;

  /// \brief Plain function object xtd::sinh_d : double -> double.
  const sinh_<double>      sinh_d;

  /// \brief Plain function object xtd::sinh_ld : long double -> long double.
  const sinh_<long double> sinh_ld;

  /// \brief Meta function object xtd::sinh.
  const sinh_type          sinh;


} // end of namespace xtd



#endif // ! EXTENDED_MATH_SINH_HH
