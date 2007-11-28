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

#ifndef MLN_MATH_ROUND_HH
# define MLN_MATH_ROUND_HH

/*! \file mln/math/round.hh
 *
 * \brief Define round routine.
 */

# include <cmath>

# include <mln/core/concept/function.hh>



namespace mln
{

  namespace math
  {


    template <typename R>
    struct round : public Function_v2v< round<R> >
    {
      typedef R result;

      template <typename T>
      result operator()(const T& v) const;

    };


# ifndef MLN_INCLUDE_ONLY

    template <typename R>
    template <typename T>
    inline
    R round<R>::operator()(const T& v) const
    {
      return (long int)(v + 0.49999); // FIXME: !!!
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::math

} // end of namespace mln


#endif // ! MLN_MATH_ROUND_HH
