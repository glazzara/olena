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

#ifndef MLN_CORE_TRAIT_OP_MULT_HH
# define MLN_CORE_TRAIT_OP_MULT_HH

/*!
 * \file   mln/core/trait/op_mult.hh
 *
 * \brief  FIXME.
 *
 */


# define mln_op_mult(T, U) typename mln::trait::op_mult< T , U >::ret



namespace mln
{

  namespace algebra
  {

    template <unsigned n, typename T>
    class vec;

    template <unsigned n, unsigned m, typename T>
    class mat;

  } // end of namespace mln::metal

  namespace trait
  {

    template <typename T, typename U>
    struct op_mult;


    template <typename T>
    struct op_mult<T, T>
    {
      typedef T ret;
    };

    template <>
    struct op_mult<int, float>
    {
      typedef float ret;
    };
    template <>
    struct op_mult<float, int>
    {
      typedef float ret;
    };

    template <>
    struct op_mult<int, double>
    {
      typedef double ret;
    };
    template <>
    struct op_mult<double, int>
    {
      typedef double ret;
    };

    template <>
    struct op_mult<double, float>
    {
      typedef double ret;
    };
    template <>
    struct op_mult<float, double>
    {
      typedef double ret;
    };

    template <unsigned n, typename T, typename U>
    struct op_mult<algebra::vec<n, T>, U>
    {
      typedef algebra::vec<n, mln_op_mult(T, U)> ret;
    };
    template <typename U, unsigned n, typename T>
    struct op_mult<U, algebra::vec<n, T> >
    {
      typedef algebra::vec<n, mln_op_mult(T, U)> ret;
    };

    template <unsigned n, unsigned m, typename T, typename U>
    struct op_mult<algebra::mat<n, m, T>, U>
    {
      typedef algebra::mat<n, m, mln_op_mult(T, U)> ret;
    };
    template <typename U, unsigned n, unsigned m, typename T>
    struct op_mult<U, algebra::mat<n, m, T> >
    {
      typedef algebra::mat<n, m, mln_op_mult(T, U)> ret;
    };

    template <unsigned n, unsigned o, typename T, unsigned m, typename U>
    struct op_mult<algebra::mat<n, o, T>, algebra::mat<o, m, U> >
    {
      typedef algebra::mat<n, m, mln_op_mult(T, U)> ret;
    };

    template <unsigned m, unsigned n, typename T, typename U>
    struct op_mult<algebra::mat<m, n, T>, algebra::vec<n, U> >
    {
      typedef algebra::mat<m, 1, mln_op_mult(T, U)> ret;
    };

    template <unsigned n, typename U, unsigned m, typename T>
    struct op_mult< algebra::vec<n, U>, algebra::mat<n, m, T> >
    {
      typedef algebra::mat<1, m, mln_op_mult(T, U)> ret;
    };


  } // end of namespace mln::trait

} // end of namespace mln

#endif // ! MLN_CORE_TRAIT_OP_MULT_HH
