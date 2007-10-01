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

#ifndef MLN_TRAIT_OP_MINUS_HH
# define MLN_TRAIT_OP_MINUS_HH


# define mln_op_minus(T, U) typename mln::trait::op_minus< T , U >::ret



namespace mln
{

  namespace metal
  {

    template <unsigned n, typename T>
    class vec;

    template <unsigned n, unsigned m, typename T>
    class mat;

  } // end of namespace mln::metal

  namespace trait
  {

    template <typename T, typename U>
    struct op_minus;


    template <typename T>
    struct op_minus<T, T>
    {
      typedef T ret;
    };

    template <typename T>
    struct op_minus<unsigned, T>
    {
      typedef mln_op_minus(signed, T) ret;
    };

    template <>
    struct op_minus<int, float>
    {
      typedef float ret;
    };
    template <>
    struct op_minus<float, int>
    {
      typedef float ret;
    };

    template <>
    struct op_minus<int, double>
    {
      typedef double ret;
    };
    template <>
    struct op_minus<double, int>
    {
      typedef double ret;
    };

    template <>
    struct op_minus<double, float>
    {
      typedef double ret;
    };
    template <>
    struct op_minus<float, double>
    {
      typedef double ret;
    };

    template <unsigned n, typename T, typename U>
    struct op_minus<metal::vec<n, T>, U>
    {
      typedef metal::vec<n, mln_op_minus(T, U)> ret;
    };
    template <typename U, unsigned n, typename T>
    struct op_minus<U, metal::vec<n, T> >
    {
      typedef metal::vec<n, mln_op_minus(T, U)> ret;
    };

    template <unsigned n, typename T, typename U>
    struct op_minus<metal::vec<n, T>, metal::vec<n, U> >
    {
      typedef metal::vec<n, mln_op_minus(T, U)> ret;
    };

    template <unsigned n, unsigned m, typename T, typename U>
    struct op_minus<metal::mat<n, m, T>, U>
    {
      typedef metal::mat<n, m, mln_op_minus(T, U)> ret;
    };
    template <typename U, unsigned n, unsigned m, typename T>
    struct op_minus<U, metal::mat<n, m, T> >
    {
      typedef metal::mat<n, m, mln_op_minus(T, U)> ret;
    };

    template <unsigned n, unsigned m, typename T, typename U>
    struct op_minus<metal::mat<n, m, T>, metal::mat<n, m, U> >
    {
      typedef metal::mat<n, m, mln_op_minus(T, U)> ret;
    };


  } // end of namespace mln::trait

} // end of namespace mln

#endif // ! MLN_TRAIT_OP_MINUS_HH
