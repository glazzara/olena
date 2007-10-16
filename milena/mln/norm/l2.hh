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

#ifndef MLN_NORM_L2_HH
# define MLN_NORM_L2_HH

/*! \file mln/norm/l2.hh
 *
 * \brief Define some l2-norm related routines.
 */

# include <cmath>

# include <mln/metal/vec.hh>

namespace mln
{

  namespace norm
  {

    /// Infinity-norm of a vector \p vec.
    template <unsigned n, typename C>
    float l2(const C (&vec)[n]);

    /// Infinity-norm distance between vectors \p v1 and \p v2.
    template <unsigned n, typename C>
    float l2_distance(const C (&v1)[n], const C (&v2)[n]);

    template <unsigned n, typename C>
    float l2(const metal::vec<n,C>& vec);

    template <unsigned n, typename C>
    float l2_distance(const metal::vec<n,C>& vec1, const metal::vec<n,C>& vec2);

    // FIXME: Replace float by mln_value_sum(C)...


# ifndef MLN_INCLUDE_ONLY

    template <unsigned n, typename C>
    float l2(const C (&vec)[n])
    {
      C c = 0;
      for (unsigned i = 0; i < n; ++i)
	c += vec[i] * vec[i];
      return sqrt(c);
    }

    template <unsigned n, typename C>
    float l2_distance(const C (&v1)[n], const C (&v2)[n])
    {
      C d = 0;
      for (unsigned i = 0; i < n; ++i)
      {
	C dd = v1[i] - v2[i];
	d += dd * dd;
      }
      return sqrt(d);
    }

    template <unsigned n, typename C>
    float l2(const metal::vec<n,C>& vec)
    {
      C c = 0;
      for (unsigned i = 0; i < n; ++i)
	c += vec[i] * vec[i];
      return sqrt(c);
    }

    template <unsigned n, typename C>
    float l2(const metal::vec<n,C>& vec1, const metal::vec<n,C>& vec2)
    {
      C d = 0;
      for (unsigned i = 0; i < n; ++i)
      {
	C dd = vec1[i] - vec2[i];
	d += dd * dd;
      }
      return sqrt(d);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::norm

} // end of namespace mln


#endif // ! MLN_NORM_L2_HH
