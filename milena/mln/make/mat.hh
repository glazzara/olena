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

#ifndef MLN_MAKE_MAT_HH
# define MLN_MAKE_MAT_HH

/*! \file mln/make/mat.hh
 *
 * \brief Routine to construct an mln::algebra::mat.
 */

# include <mln/algebra/mat.hh>

namespace mln
{

  namespace make
  {
    /*! \brief Create an mln::algebra::mat<n,m,T>.
     *
     * \param[in] tab Tab of value.
     *
     * \pre The dimension table N is such as N = n * m
     * with n and m, the dimensions oh the matrix.
     */
    template <unsigned n, unsigned m, unsigned N, typename T>
    algebra::mat<n,m,T> mat(const T tab[N]);
    
    template <unsigned n, unsigned m, typename T>
    algebra::mat<n,m,T> mat(algebra::vec<n,T> v);

# ifndef MLN_INCLUDE_ONLY

    template <unsigned n, unsigned m, unsigned N, typename T>
    inline
    algebra::mat<n,m,T> mat(const T tab[N])
    {
      mln_precondition(n * m == N);
      algebra::mat<n,m,T> tmp;
      for (unsigned i = 0; i < N; ++i)
	tmp(i / m, i % m) = tab[i];
      return tmp;
    }

    template <unsigned n, typename T>
    algebra::mat<n,1,T> mat(algebra::vec<n,T> v)
    {
      algebra::mat<n,1,T> tmp;
      for (unsigned i = 0; i < n; i++)
        tmp(i,1) = v[i];
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln

#endif // ! MLN_MAKE_MAT_HH
