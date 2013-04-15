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

#ifndef MLN_MATH_ABS_HH
# define MLN_MATH_ABS_HH

/// \file
///
/// Define the absolute value (abs) routine.

# include <cmath>
# include <cstdlib>

# include <mln/value/int_u.hh>


namespace mln
{

  namespace math
  {

    /*! \brief Generic version.
      \ingroup mlnmath
    */
    template <typename T>
    T abs(const T& v);

    /*! Specializations for existing overloads of std::abs.

        Reference:
          ISO/IEC 14882:2003 C++ standard, section 26.5
          (C Library, [lib.c.math]).

	\ingroup mlnmath
    */
    /// \{
    int         abs(int v);
    long        abs(long v);

    float       abs(float v);
    double      abs(double v);
    long double abs(long double v);
    /// \}

    /*! Specialization for mln::value::int_u.
      \ingroup mlnmath
    */
    template <unsigned n>
    value::int_u<n> abs(const value::int_u<n>& v);



# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    T abs(const T& v)
    {
      return v >= 0 ? v : static_cast<T>(-v);
    }

    inline int         abs(int v)         { return std::abs(v); }
    inline long        abs(long v)        { return std::abs(v); }

    inline float       abs(float v)       { return std::abs(v); }
    inline double      abs(double v)      { return std::abs(v); }
    inline long double abs(long double v) { return std::abs(v); }

    template <unsigned n>
    inline
    value::int_u<n> abs(const value::int_u<n>& v)
    {
      return v;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::math

} // end of namespace mln


#endif // ! MLN_MATH_ABS_HH
