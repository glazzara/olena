// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
// (LRDE)
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


    /// Create an mln::algebra::h_mat<n,T>.
    /*
     * \param[in] tab C-array of values.
     *
     * \pre The array dimension N has to be square (N = n * n).
     */
    template <typename T, unsigned N>
    algebra::h_mat<mlc_sqrt_int(N), T> h_mat(const T (&tab)[N]);


    /*! \brief Create a rotation matrix as mln::algebra::h_mat<n,T>.

      \param[in] q A quaternion.

      \return A rotation matrix based on \p q.
     */
    template <typename C>
    algebra::h_mat<3, C> h_mat(const C& v, const algebra::quat& q);

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


    template <typename C>
    inline
    algebra::h_mat<3, C>
    h_mat(const C& v, const algebra::quat& q)
    {
      mln_precondition(q.is_unit());
      (void) v;

      algebra::h_mat<3, C> m;
      C
	w = q.to_vec()[0],
	x = q.to_vec()[1],  x2 = 2*x*x,  xw = 2*x*w,
	y = q.to_vec()[2],  y2 = 2*y*y,  xy = 2*x*y,  yw = 2*y*w,
	z = q.to_vec()[3],  z2 = 2*z*z,  xz = 2*x*z,  yz = 2*y*z,  zw = 2*z*w;

      C t[9] = {1.f - y2 - z2,  xy - zw,  xz + yw,
		xy + zw,  1.f - x2 - z2,  yz - xw,
		xz - yw,  yz + xw,  1.f - x2 - y2};

      m = mln::make::h_mat(t);
      return m;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_H_MAT_HH
