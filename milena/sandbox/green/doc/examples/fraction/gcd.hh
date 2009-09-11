// Copyright (C) 2006, 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MATH_GCD_HH
# define MLN_MATH_GCD_HH

#include <mln/math/min.hh>
#include <mln/math/max.hh>
#include <mln/math/abs.hh>

namespace mln
{
  namespace math
  {    
    template <typename T>
    T gcd(const T& a, const T& b);

    template <typename T>
    T gcd_(const T& a, const T& b);

# ifndef MLN_INCLUDE_ONLY

    // Safety interface for the greatest common divisor
    template <typename T>
    inline
    T gcd(const T& a, const T& b)
    {
      trace::entering("const T mln::math::gcd(const T& a, const T& b)");

      T result;
      T abs_a = math::abs(a);
      T abs_b = math::abs(b);

      if ((0 == abs_a) || (0 == abs_b))
	result = 1;
      else
	result = gcd_(math::max(abs_a, abs_b), math::min(abs_a, abs_b));

      trace::exiting("const T mln::math::gcd(const T& a, const T& b)");
      return result;
    }

    // Euclidian algorithm for the greatest common divisor
    template <typename T>
    inline
    T gcd_(const T& a, const T& b)
    {
      trace::entering("T mln::math::cgd_(const T& a, const T& b)");
      mln_assertion(a > 0);
      mln_assertion(b > 0);
      mln_assertion(a >= b);

      T rest = a%b;

      if (0 == rest) 
	rest = b;
      else
	rest = gcd_(b, rest);

      trace::exiting("T mln::math::cgd_(const T& a, const T& b)");
      return rest;
    }

# endif // ! MLN_INCLUDE_ONLY

  }
}

#endif // ! MLN_MATH_GCD_HH
