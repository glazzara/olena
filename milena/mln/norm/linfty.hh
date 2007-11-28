// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_NORM_LINFTY_HH
# define MLN_NORM_LINFTY_HH

/*! \file mln/norm/linfty.hh
 *
 *  \brief Define some L-infinity-norm related routines.
 *  \see http://mathworld.wolfram.com/L-Infinity-Norm.html for more
 *  information.
 */

# include <mln/math/abs.hh>
# include <mln/metal/vec.hh>


namespace mln
{

  namespace norm
  {

    /// L-infinity-norm of a vector \a vec.
    /// \{
    template <unsigned n, typename C>
    C linfty(const C (&vec)[n]);

    template <unsigned n, typename C>
    C linfty(const metal::vec<n,C>& vec);
    /// \}


    /// L-infinity-norm distance between vectors \a vec1 and \a vec2.
    /// \{
    template <unsigned n, typename C>
    C linfty_distance(const C (&vec1)[n], const C (&vec2)[n]);

    template <unsigned n, typename C>
    C linfty_distance(const metal::vec<n,C>& vec1,
		      const metal::vec<n,C>& vec2);
    /// \}


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <unsigned n, typename C, typename V>
      inline
      C linfty_(const V& vec)
      {
	C m = 0;
	for (unsigned i = 0; i < n; ++i)
	  {
	    // Compute the maximal component absolute value of the
	    // vector.
	    C mc = mln::math::abs(vec[i]);
	    if (mc > m)
	      m = mc;
	  }
	return m;
      }

      template <unsigned n, typename C, typename V>
      inline
      C linfty_distance_(const V& vec1, const V& vec2)
      {
	C d = 0;
	for (unsigned i = 0; i < n; ++i)
	  {
	    // Compute the maximal absolute value of the distance
	    // between components of the same index.
	    C dc = mln::math::abs(vec1[i] - vec2[i]);
	    if (dc > d)
	      d = dc;
	  }
	return d;
      }

    } // end of namespace mln::norm::impl


    /*----------.
    | Facades.  |
    `----------*/

    template <unsigned n, typename C>
    inline
    C linfty(const C (&vec)[n])
    {
      return impl::linfty_<n, C>(vec);
    }

    template <unsigned n, typename C>
    inline
    C linfty(const metal::vec<n,C>& vec)
    {
      return impl::linfty_<n, C>(vec);
    }

    template <unsigned n, typename C>
    inline
    C linfty_distance(const C (&vec1)[n], const C (&vec2)[n])
    {
      return impl::linfty_distance_<n, C>(vec1, vec2);
    }

    template <unsigned n, typename C>
    inline
    C linfty_distance(const metal::vec<n,C>& vec1,
		      const metal::vec<n,C>& vec2)
    {
      return impl::linfty_distance_<n, C>(vec1, vec2);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::norm

} // end of namespace mln


#endif // ! MLN_NORM_LINFTY_HH
