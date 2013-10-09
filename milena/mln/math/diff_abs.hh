// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE).
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

#ifndef MLN_MATH_DIFF_ABS_HH
# define MLN_MATH_DIFF_ABS_HH

/// \file
/// \brief Routine computing the absolute difference of two values.
///
/// \todo Do we want a specific version for vectorial types?


namespace mln
{

  namespace math
  {

    /// \brief Return the absolute difference between \a v1 and \a v2.
    ///
    /// That is, compute \f$|v1 - v2|\f$.
    ///
    /// \ingroup mlnmath
    template <typename T>
    T diff_abs(const T& v1, const T& v2);


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    T diff_abs(const T& v1, const T& v2)
    {
      return v1 > v2 ? v1 - v2 : v2 - v1;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::math

} // end of namespace mln


#endif // ! MLN_MATH_DIFF_ABS_HH
