// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_METAL_MATH_SQRT_HH
# define MLN_METAL_MATH_SQRT_HH

/*! \file
 *
 * \brief Definition of the 'sqrt' static function.
 *
 * \todo Rely on nth-root.
 */

# include <mln/metal/bool.hh>
# include <mln/metal/int.hh>


# define mlc_sqrt_int(N) mln::metal::math::sqrt_int<( N )>::value


namespace mln
{

  namespace metal
  {

    namespace math
    {

      // sqrt_int<x>

      namespace impl
      {

	template <int n, int lo = 1, int hi = n>
	struct sqrt_int_
	{
	  enum {
	    mid = (lo + hi + 1) / 2,
	    val_lo = sqrt_int_<n, lo, mid-1>::value,
	    val_hi = sqrt_int_<n, mid, hi>::value
	  };
	  enum { value = n < mid * mid ? val_lo : val_hi };
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

      } // end of namespace mln::metal::math::impl

      template <int n>
      struct sqrt_int : impl::sqrt_int_if_< n, (n >= 0) >
      {
      };


      // sqrt<N>

      template <typename N>
      struct sqrt;

      template <int n>
      struct sqrt< int_<n> >
      {
	typedef int_< sqrt_int<n>::value > ret;
      };


    } // end of namespace mln::metal::math

  } // end of namespace mln::metal

} // end of namespace mln


#endif // ! MLN_METAL_MATH_SQRT_HH
