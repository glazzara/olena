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

#ifndef MLN_NORM_INFTY_HH
# define MLN_NORM_INFTY_HH

/*! \file mln/norm/infty.hh
 *
 * \brief Define some infinity-norm related routines.
 */

# include <cmath>
# include <mln/metal/vec.hh>


namespace mln
{

  namespace norm
  {

    /// Infinity-norm of a vector \p v (being a C static array).
    template <unsigned n, typename C>
    C infty(const C (&v)[n]);

    /// Infinity-norm of a vector \p v (being a metal::vec).
    template <unsigned n, typename C>
    C infty(const metal::vec<n,C>& v);


    /// Infinity-norm distance between vectors \p v1 and \p v2.
    template <unsigned n, typename C>
    C infty_distance(const C (&v1)[n], const C (&v2)[n]);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned n, typename C>
    C infty(const C (&v)[n])
    {
      C c = 0;
      for (unsigned i = 0; i < n; ++i)
	if (std::abs(v[i]) > c)
	  c = std::abs(v[i]);
      return c;
    }

    template <unsigned n, typename C>
    C infty(const metal::vec<n,C>& v)
    {
      // FIXME: Add a ctor to metal::vec...
      C c = 0;
      for (unsigned i = 0; i < n; ++i)
	if (std::abs(v[i]) > c)
	  c = std::abs(v[i]);
      return c;
    }

    template <unsigned n, typename C>
    C infty_distance(const C (&v1)[n], const C (&v2)[n])
    {
      C d = 0;
      for (unsigned i = 0; i < n; ++i)
	{
	  C dd = v2[i] > v1[i] ? v2[i] - v1[i] : v1[i] - v2[i];
	  if (dd > d)
	    d = dd;
	}
      return d;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::norm

} // end of namespace mln


#endif // ! MLN_NORM_INFTY_HH
