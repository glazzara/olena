// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_APPROX_EQUAL_HH
# define MLN_APPROX_EQUAL_HH

/// \file
///
/// Define an approximative equality test.


namespace mln
{


  namespace def
  {

    /// Epsilon.
    const float epsilon = 1e-5; // FIXME
    const float minus_epsilon = - epsilon;
    const float one_plus_epsilon = 1 + epsilon;

  } // def


  bool approx_equal(float lhs,  float rhs);
  bool approx_equal(float lhs,  double rhs);
  bool approx_equal(double lhs, float rhs);
  bool approx_equal(double lhs, double rhs);


# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {
    inline float my_fabs_(float f) { return f > 0.f ? f : - f; }
  }

  inline
  bool approx_equal(float lhs, float rhs)
  {
    // FIXME: Cf. boost.
    float
      diff = lhs > rhs ? lhs - rhs : rhs - lhs,
      mean = internal::my_fabs_(lhs + rhs) / 2;
    return
      mean > def::epsilon ?
      diff / mean < def::epsilon :
      diff < def::epsilon;
  }

  inline
  bool approx_equal(float lhs, double rhs)
  {
    return approx_equal(lhs, float(rhs));
  }

  inline
  bool approx_equal(double lhs, float rhs)
  {
    return approx_equal(float(lhs), rhs);
  }

  inline
  bool approx_equal(double lhs, double rhs)
  {
    return approx_equal(float(lhs), float(rhs));
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_APPROX_EQUAL_HH
