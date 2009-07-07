// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_MAKE_H_MAT_HH
# define MLN_MAKE_H_MAT_HH

/// \file
///
/// Routine to construct an mln::algebra::h_mat.

# include <mln/algebra/h_mat.hh>
# include <mln/metal/math/sqrt.hh>


namespace mln
{

  namespace make
  {


    /// Create an mln::algebra::mat<n,n,T>.
    /*
     * \param[in] tab C-array of values.
     *
     * \pre The array dimension N has to be square (N = n * n).
     */
    template <typename T, unsigned N>
    algebra::h_mat<mlc_sqrt_int(N), T> h_mat(const T (&tab)[N]);



# ifndef MLN_INCLUDE_ONLY

    template <typename T, unsigned N>
    inline
    algebra::h_mat<mlc_sqrt_int(N), T>
    h_mat(const T (&tab)[N])
    {
      enum { n = mlc_sqrt_int(N) };
      mlc_bool(N == n * n)::check();
      algebra::h_mat<n,T> tmp;
      for (unsigned i = 0; i < N; ++i)
	tmp(i / n, i % n) = tab[i];
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_H_MAT_HH
