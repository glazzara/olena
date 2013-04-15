// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_NORM_L1_HH
# define MLN_NORM_L1_HH

/// \file
///
/// Define some L1-norm related routines.
/// \see http://mathworld.wolfram.com/L1-Norm.html for more information.

# include <mln/math/abs.hh>
# include <mln/algebra/vec.hh>


namespace mln
{

  namespace norm
  {

    /*! \brief L1-norm of a vector \a vec.
      \ingroup mlnlinear
    */
    /// \{
    template <unsigned n, typename C>
    mln_sum_product(C,C) l1(const C (&vec)[n]);

    template <unsigned n, typename C>
    mln_sum_product(C,C) l1(const algebra::vec<n,C>& vec);
    /// \}

    /*! \brief L1-norm distance between vectors \a vec1 and \a vec2.
      \ingroup mlnlinear
     */
    /// \{
    template <unsigned n, typename C>
    mln_sum_product(C,C) l1_distance(const C (&vec1)[n], const C (&vec2)[n]);

    template <unsigned n, typename C>
    mln_sum_product(C,C) l1_distance(const algebra::vec<n,C>& vec1,
			   const algebra::vec<n,C>& vec2);
    /// \}


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {
      template <unsigned n, typename C, typename V>
      inline
      mln_sum_product(C,C)
      l1_(const V& vec)
      {
	typedef mln_sum_product(C,C) M;
	M m = 0;
	for (unsigned i = 0; i < n; ++i)
	  {
	    M v_i = static_cast<M>(mln::math::abs(vec[i]));
	    m = static_cast<M>(m + v_i);
	  }
	return m;
      }

      template <unsigned n, typename C, typename V>
      inline
      mln_sum_product(C,C)
      l1_distance_(const V& vec1, const V& vec2)
      {
	typedef mln_sum_product(C,C) D;
	D d = 0;
	for (unsigned i = 0; i < n; ++i)
	  {
	    D v1_v2 = static_cast<D>(mln::math::abs(vec1[i] - vec2[i]));
	    d = static_cast<D>(d + v1_v2);
	  }
	return d;
      }

    } // end of namespace mln::norm::impl


    /*----------.
    | Facades.  |
    `----------*/

    template <unsigned n, typename C>
    inline
    mln_sum_product(C,C)
    l1(const C (&vec)[n])
    {
      return impl::l1_<n, C>(vec);
    }

    template <unsigned n, typename C>
    inline
    mln_sum_product(C,C)
    l1(const algebra::vec<n,C>& vec)
    {
      return impl::l1_<n, C>(vec);
    }

    template <unsigned n, typename C>
    inline
    mln_sum_product(C,C)
    l1_distance(const C (&vec1)[n], const C (&vec2)[n])
    {
      return impl::l1_distance_<n, C>(vec1, vec2);
    }

    template <unsigned n, typename C>
    inline
    mln_sum_product(C,C)
    l1_distance(const algebra::vec<n,C>& vec1, const algebra::vec<n,C>& vec2)
    {
      return impl::l1_distance_<n, C>(vec1, vec2);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::norm

} // end of namespace mln


#endif // ! MLN_NORM_L1_HH
