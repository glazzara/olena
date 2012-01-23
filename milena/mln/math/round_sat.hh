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

#ifndef MLN_MATH_ROUND_SAT_HH
# define MLN_MATH_ROUND_SAT_HH

/// \file
///
/// Round a given value considering its type as circular.

# include <cmath>

# include <mln/fun/v2v/round_sat.hh>


namespace mln
{

  namespace math
  {

    /*! \brief Round a given value considering its type as circular.
      \param[in] v The value to be rounded.

      \return A round value of type \tparam R.

      \warning The return type \tparam must be passed as template
      parameter on function call.

      \ingroup mlnmath
    */
    template <typename R, typename T>
    R round_sat(const T& v);

    /*! \brief Round a given value considering its type as circular.
      \param[in] v The value to be rounded.
      \param[in] return_type The returned type to be used.

      \return A round value of type \tparam R.

      \ingroup mlnmath
    */
    template <typename R, typename T>
    R round_sat(const T& v, const R& return_type);


# ifndef MLN_INCLUDE_ONLY

    template <typename R, typename T>
    R round_sat(const T& v)
    {
      static fun::v2v::round_sat<R> f;
      return f(v);
    }

    template <typename R, typename T>
    R round_sat(const T& v, const R&)
    {
      static fun::v2v::round_sat<R> f;
      return f(v);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::math

} // end of namespace mln


#endif // ! MLN_MATH_ROUND_SAT_HH
