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

#ifndef MLN_TRAIT_PROMOTE_HH
# define MLN_TRAIT_PROMOTE_HH


# define mln_promote(T, U) typename mln::trait::promote< T , U >::ret



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
    struct promote;


    template <>
    struct promote<int, float>
    {
      typedef float ret;
    };
    template <>
    struct promote<float, int>
    {
      typedef float ret;
    };

    template <>
    struct promote<int, double>
    {
      typedef double ret;
    };
    template <>
    struct promote<double, int>
    {
      typedef double ret;
    };

    template <>
    struct promote<double, float>
    {
      typedef double ret;
    };
    template <>
    struct promote<float, double>
    {
      typedef double ret;
    };

    template <unsigned n, typename T, typename U>
    struct promote<metal::vec<n, T>, metal::vec<n, U> >
    {
      typedef metal::vec<n, mln_promote(T, U)> ret;
    };

    template <unsigned n, unsigned m, typename T, typename U>
    struct promote<metal::mat<n, m, T>, metal::mat<n, m, U> >
    {
      typedef metal::mat<n, m, mln_promote(T, U)> ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln

#endif // ! MLN_TRAIT_PROMOTE_HH
