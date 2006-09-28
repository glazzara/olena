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

#ifndef XTD_MATH_FLOOR_HH
# define XTD_MATH_FLOOR_HH

# include <xtd/math/includes.hh>


namespace xtd
{

  /// \{
  /// Definition of xtd floor functions.


  template <typename T> struct floor_;

  template <> struct constraints_< floor_, float > {};
  template <> struct constraints_< floor_, double > {};
  template <> struct constraints_< floor_, long double > {};

  template <typename T>
  struct fun_traits_< floor_<T> >
  {
    typedef T arg_type;
    typedef T res_type;
  };


  template <typename T>
  struct floor_

    : public abstract::plain_nary_fun_< 1, floor_<T> >,

      private constraints_< floor_, T >
  {
    xtd_res(floor_<T>) impl_op(const T& arg) const
    {
      return std::floor(arg);
    }
  };


  typedef m1fun_<floor_> floor_type;


  /// \}



  /// \brief Plain function object xtd::floor_f : float -> float.
  const floor_<float>       floor_f;

  /// \brief Plain function object xtd::floor_d : double -> double.
  const floor_<double>      floor_d;

  /// \brief Plain function object xtd::floor_ld : long double -> long double.
  const floor_<long double> floor_ld;

  /// \brief Meta function object xtd::floor.
  const floor_type          floor;


} // end of namespace xtd



#endif // ! XTD_MATH_FLOOR_HH
