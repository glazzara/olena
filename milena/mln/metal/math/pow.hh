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

#ifndef MLN_METAL_MATH_POW_HH
# define MLN_METAL_MATH_POW_HH

/*! \file
 *
 * \brief Definition of the 'power' static function.
 */

# include <mln/metal/bool.hh>
# include <mln/metal/int.hh>


# define mlc_pow(X, N)      typename mln::metal::math::pow< X, N >::ret
# define mlc_pow_int(x, n)           mln::metal::math::pow_int< x, n >::value



namespace mln
{

  namespace metal
  {

    namespace math
    {

      // pow_int<x, n>

      namespace impl
      {

	template <int x, int n>
	struct pow_int_
	{
	  static const int value = x * pow_int_<x, n-1>::value;
	};

	template <int x>
	struct pow_int_< x, 0 >
	{
	  static const int value = 1;
	};

	template <>
	struct pow_int_< 0, 0 >;
      

	// Entry.

	template <int x, int n, bool b>
	struct pow_int_if_ : pow_int_<x, n>
	{
	};

	template <int x, int n>
	struct pow_int_if_< x, n, false >
	{
	};

      }

      template <int x, int n>
      struct pow_int : impl::pow_int_if_< x, n,
					  (n >= 0 && ! (x == 0 && n == 0)) >
      {
      };


      // pow<X, N>

      template <typename X, typename N>
      struct pow;

      template <int x, int n>
      struct pow< int_<x>, int_<n> >
      {
	typedef int_< pow_int<x, n>::value > ret;
      };


    } // end of namespace mln::metal::math

  } // end of namespace mln::metal

} // end of namespace mln


#endif // ! MLN_METAL_MATH_POW_HH
