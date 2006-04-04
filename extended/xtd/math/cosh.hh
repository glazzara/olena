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

#ifndef EXTENDED_MATH_COSH_HH
# define EXTENDED_MATH_COSH_HH

# include <xtd/math/includes.hh>


namespace xtd
{

  /// \{
  /// Definition of xtd hyperbolic cosine functions.


  template <typename T> struct cosh_;

  template <> struct constraints_< cosh_, float > {};
  template <> struct constraints_< cosh_, double > {};
  template <> struct constraints_< cosh_, long double > {};

  template <typename T>
  struct fun_traits_< cosh_<T> >
  {
    typedef T arg_type;
    typedef T res_type;
  };


  template <typename T>
  struct cosh_

    : public abstract::plain_nary_fun_< 1, cosh_<T> >,

      private constraints_< cosh_, T >
  {
    xtd_res(cosh_<T>) impl_op(const T& arg) const
    {
      return std::cosh(arg);
    }
  };


  typedef m1fun_<cosh_> cosh_type;


  /// \}



  /// \brief Plain function object xtd::cosh_f : float -> float.
  const cosh_<float>       cosh_f;

  /// \brief Plain function object xtd::cosh_d : double -> double.
  const cosh_<double>      cosh_d;

  /// \brief Plain function object xtd::cosh_ld : long double -> long double.
  const cosh_<long double> cosh_ld;

  /// \brief Meta function object xtd::cosh.
  const cosh_type          cosh;


} // end of namespace xtd



#endif // ! EXTENDED_MATH_COSH_HH
