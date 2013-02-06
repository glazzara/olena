// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
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

#ifndef MLN_METAL_MATH_ROOT_HH
# define MLN_METAL_MATH_ROOT_HH

/*! \file
 *
 * \brief Definition of the 'nth-root' static function.
 *
 * \todo Have consistent writing of math meta-routines.
 */

# include <mln/metal/math/pow.hh>


# define mlc_root(N,X) mln::metal::math::root<( N ),( X )>


namespace mln
{

  namespace metal
  {

    namespace math
    {

      namespace impl
      {

	template <unsigned n, unsigned x,
		  unsigned lo = 1, unsigned hi = x>
	struct root
	{
	  enum {
	    mid = (lo + hi + 1) / 2,
	    val_lo = root<n, x, lo, mid-1>::value,
	    val_hi = root<n, x, mid, hi>::value
	  };
	  enum { value = x < unsigned(mlc_pow_int(mid, n)) ? val_lo : val_hi };
	};

	template<unsigned n, unsigned x, unsigned m>
	struct root<n, x, m, m>
	{
	  enum { value = m }; // Found.
	};

      } // end of namespace mln::metal::math::impl

      template <unsigned n, unsigned x>
      struct root : bool_<(n != 0)>::check_t
      {
	enum { value    = impl::root<n,x>::value,
	       reminder = x - mlc_pow_int(value, n) };
      };

      template <unsigned n>
      struct root<n, 0> : bool_<(n != 0)>::check_t
      {
	enum { value = 0,
	       reminder = 0};
      };

    } // end of namespace mln::metal::math

  } // end of namespace mln::metal

} // end of namespace mln


#endif // ! MLN_METAL_MATH_ROOT_HH
