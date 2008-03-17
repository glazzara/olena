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

#ifndef MLN_METAL_MATH_SQRT_HH
# define MLN_METAL_MATH_SQRT_HH

/*! \file mln/algebra/math/sqrt.hh
 *
 * \brief Definition of the 'sqrt' static function.
 */

# include <mln/metal/bool.hh>
# include <mln/metal/int.hh>


namespace mln
{

  namespace metal
  {

    namespace math
    {

      // sqrt_int<x, n>

      namespace impl
      {

	template <int n, int lo = 1, int hi = n>
	struct sqrt_int_
	{
	  enum { mid = (lo + hi + 1) / 2 };

	  enum { value = n < mid * mid
                         ? sqrt_int_<n, lo, mid-1>::value
		         : sqrt_int_<n, mid, hi>::result };
	};

	template<int n, int m>
	struct sqrt_int_<n, m, m>
	{
	  enum { value = m };
	};

	// Entry.

	template <int n, bool b>
	struct sqrt_int_if_ : sqrt_int_<n>
	{
	  enum { value_ = sqrt_int_<n>::value,
		 reminder_ = n - value_ * value_ };
	  // FIXME: Check that reminder_ == 0.
	};

	template <int n>
	struct sqrt_int_if_< n, false >
	{
	};

      } // end of namespace mln::algebra::math::impl

      template <int n>
      struct sqrt_int : impl::sqrt_int_if_< n, (n >= 0) >
      {
      };


      // sqrt<N>

      template <typename X, typename N>
      struct sqrt;

      template <int n>
      struct sqrt< int_<n> >
      {
	typedef int_< sqrt_int<n>::value > ret;
      };


    } // end of namespace mln::algebra::math

  } // end of namespace mln::metal

} // end of namespace mln


#endif // ! MLN_METAL_MATH_SQRT_HH
