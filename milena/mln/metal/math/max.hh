// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_METAL_MATH_MAX_HH
# define MLN_METAL_MATH_MAX_HH

/*! \file
 *
 * \brief Definition of the 'max' static function.
 */

# include <mln/metal/bool.hh>
# include <mln/metal/int.hh>

# define mlc_max(X, Y)      typename mln::metal::math::max< X, Y >::ret
# define mlc_max_int(x, y)           mln::metal::math::max_int< x, y >::value

namespace mln
{

  namespace metal
  {

    namespace math
    {

      // max_int<x, y>

      template <int x, int y>
      struct max_int
      {
	enum { value = (x > y ? x : y) };
      };


      // max<X, Y>

      template <typename X, typename Y>
      struct max;

      template <int x, int y>
      struct max< int_<x>, int_<y> >
      {
	typedef int_< max_int<x, y>::value > ret;
      };


    } // end of namespace mln::metal::math

  } // end of namespace mln::metal

} // end of namespace mln


#endif // ! MLN_METAL_MATH_MAX_HH
