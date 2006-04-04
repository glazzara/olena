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

#ifndef EXTENDED_MATH_ATAN2_HH
# define EXTENDED_MATH_ATAN2_HH

# include <xtd/math/includes.hh>


namespace xtd
{

  /// \{
  /// Definition of xtd arc tangent functions taking two arguments.


  template <typename T1, typename T2> struct atan2_;

  template <> struct constraints_2_< atan2_, float, float > {};
  template <> struct constraints_2_< atan2_, double, double > {};
  template <> struct constraints_2_< atan2_, long double, long double > {};

  template <typename T>
  struct fun_traits_< atan2_<T, T> >
  {
    typedef T arg1_type;
    typedef T arg2_type;
    typedef T res_type;
  };


  template <typename T1, typename T2>
  struct atan2_

    : public abstract::plain_nary_fun_< 2, atan2_<T1, T2> >,

      private constraints_2_< atan2_, T1, T2 >
  {
    xtd_res(mlc_comma_1(atan2_<T1,T2>)) impl_op(const T1& arg1, const T2& arg2) const
    {
      return std::atan2(arg1, arg2);
    }
  };


  typedef m2fun_<atan2_> atan2_type;


  /// \}



  /// \brief Plain function object xtd::atan2_f : (float, float) -> float.
  const atan2_<float, float>             atan2_f;

  /// \brief Plain function object xtd::atan2_d : (double, double) -> double.
  const atan2_<double, double>           atan2_d;

  /// \brief Plain function object xtd::atan2_ld : (long double, long double) -> long double.
  const atan2_<long double, long double> atan2_ld;

  /// \brief Meta function object xtd::atan2.
  const atan2_type                       atan2;


} // end of namespace xtd



#endif // ! EXTENDED_MATH_ATAN2_HH
