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

#ifndef EXTENDED_MATH_POW_HH
# define EXTENDED_MATH_POW_HH

# include <xtd/math/includes.hh>


namespace xtd
{

  /// \{
  /// Definition of xtd arc tangent functions taking two arguments.


  template <typename T1, typename T2> struct pow_;

  template <> struct constraints_2_< pow_, float,       float > {};
  template <> struct constraints_2_< pow_, double,      double > {};
  template <> struct constraints_2_< pow_, long double, long double > {};

  template <> struct constraints_2_< pow_, float,       int > {};
  template <> struct constraints_2_< pow_, double,      int > {};
  template <> struct constraints_2_< pow_, long double, int > {};

  template <typename T1, typename T2>
  struct fun_traits_< pow_<T1, T2> >
  {
    typedef T1 arg1_type;
    typedef T2 arg2_type;
    typedef T1 res_type;
  };


  template <typename T1, typename T2>
  struct pow_

    : public abstract::plain_nary_fun_< 2, pow_<T1, T2> >,

      private constraints_2_< pow_, T1, T2 >
  {
    xtd_res(mlc_comma_1(pow_<T1,T2>)) impl_op(const T1& arg1, const T2& arg2) const
    {
      return std::pow(arg1, arg2);
    }
  };


  typedef m2fun_<pow_> pow_type;


  /// \}



  /// \brief Plain function object xtd::pow_f : (float, float) -> float.
  const pow_<float, float>             pow_f;

  /// \brief Plain function object xtd::pow_d : (double, double) -> double.
  const pow_<double, double>           pow_d;

  /// \brief Plain function object xtd::pow_ld : (long double, long double) -> long double.
  const pow_<long double, long double> pow_ld;


  /// \brief Plain function object xtd::pow_f : (float, int) -> float.
  const pow_<float, int>               pow_fi;

  /// \brief Plain function object xtd::pow_d : (double, int) -> double.
  const pow_<double, int>              pow_di;

  /// \brief Plain function object xtd::pow_ld : (long double, int) -> long double.
  const pow_<long double, int>         pow_ldi;


  /// \brief Meta function object xtd::pow.
  const pow_type                       pow;



} // end of namespace xtd



#endif // ! EXTENDED_MATH_POW_HH
