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

#ifndef MLN_NORM_L1_HH
# define MLN_NORM_L1_HH

/*! \file mln/norm/l1.hh
 *
 *  \brief Define some L1-norm related routines.
 *  \see http://mathworld.wolfram.com/L1-Norm.html for more information.
 */

# include <mln/math/abs.hh>
# include <mln/metal/vec.hh>

// FIXME: Use mln_sum_x (to be renamed as mln_sum_product) instead of
// mln_sum.

namespace mln
{

  namespace norm
  {

    /// L1-norm of a vector \a vec.
    /// \{
    template <unsigned n, typename C>
    mln_sum(C) l1(const C (&vec)[n]);

    template <unsigned n, typename C>
    mln_sum(C) l1(const metal::vec<n,C>& vec);
    /// \}

    /// L1-norm distance between vectors \a vec1 and \a vec2.
    /// \{
    template <unsigned n, typename C>
    mln_sum(C) l1_distance(const C (&vec1)[n], const C (&vec2)[n]);

    template <unsigned n, typename C>
    mln_sum(C) l1_distance(const metal::vec<n,C>& vec1,
			   const metal::vec<n,C>& vec2);
    /// \}


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {
      template <unsigned n, typename C, typename V>
      inline
      mln_sum(C)
      l1_(const V& vec)
      {
	mln_sum(C) m = 0;
	for (unsigned i = 0; i < n; ++i)
	  m += mln::math::abs (vec[i]);
	return m;
      }

      template <unsigned n, typename C, typename V>
      inline
      mln_sum(C)
      l1_distance_(const V& vec1, const V& vec2)
      {
	mln_sum(C) d = 0;
	for (unsigned i = 0; i < n; ++i)
	  d += mln::math::abs (vec1[i] - vec2[i]);
	return d;
      }

    } // end of namespace mln::norm::impl


    /*----------.
    | Facades.  |
    `----------*/

    template <unsigned n, typename C>
    inline
    mln_sum(C)
    l1(const C (&vec)[n])
    {
      return impl::l1_<n, C>(vec);
    }

    template <unsigned n, typename C>
    inline
    mln_sum(C)
    l1(const metal::vec<n,C>& vec)
    {
      return impl::l1_<n, C>(vec);
    }

    template <unsigned n, typename C>
    inline
    mln_sum(C)
    l1_distance(const C (&vec1)[n], const C (&vec2)[n])
    {
      return impl::l1_distance_<n, C>(vec1, vec2);
    }

    template <unsigned n, typename C>
    inline
    mln_sum(C)
    l1_distance(const metal::vec<n,C>& vec1, const metal::vec<n,C>& vec2)
    {
      return impl::l1_distance_<n, C>(vec1, vec2);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::norm

} // end of namespace mln


#endif // ! MLN_NORM_L1_HH
