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

#ifndef XTD_MATH_ABS_HH
# define XTD_MATH_ABS_HH

# include <xtd/math/includes.hh>


namespace xtd
{

  /// \{
  /// Definition of xtd absolute value functions.


  template <typename T> struct abs_;

  template <> struct constraints_< abs_, int > {};
  template <> struct constraints_< abs_, long int > {};

  template <> struct constraints_< abs_, float > {};
  template <> struct constraints_< abs_, double > {};
  template <> struct constraints_< abs_, long double > {};

  template <typename T>
  struct fun_traits_< abs_<T> >
  {
    typedef T arg_type;
    typedef T res_type;
  };

  namespace internal {

    int           abs_(int x)           { return x < 0 ? -x : x; }
    long int      abs_(long int x)      { return x < 0 ? -x : x; }

    float         abs_(float x)         { return std::abs(x); }
    double        abs_(double x)        { return std::abs(x); }
    long double   abs_(long double x)   { return std::abs(x); }

  } // end of namespace xtd::internal


  template <typename T>
  struct abs_

    : public abstract::plain_nary_fun_< 1, abs_<T> >,

      private constraints_< abs_, T >
  {
    xtd_res(abs_<T>) impl_op(const T& arg) const
    {
      return internal::abs_(arg);
    }
  };


  typedef m1fun_<abs_> abs_type;


  /// \}



  /// \brief Plain function object xtd::abs_f : int -> int.
  const abs_<int>           abs_i;

  /// \brief Plain function object xtd::abs_d : long int -> long int.
  const abs_<long int>      abs_li;


  /// \brief Plain function object xtd::abs_f : float -> float.
  const abs_<float>         abs_f;

  /// \brief Plain function object xtd::abs_d : double -> double.
  const abs_<double>        abs_d;

  /// \brief Plain function object xtd::abs_ld : long double -> long double.
  const abs_<long double>   abs_ld;


  /// \brief Meta function object xtd::abs.
  const abs_type            abs;


} // end of namespace xtd



#endif // ! XTD_MATH_ABS_HH
