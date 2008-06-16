// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_UTIL_LESS_HH
# define MLN_UTIL_LESS_HH

/*! \file mln/util/less.hh
 *
 * \brief Definition of a "less" function-object with one type (T).
 *
 * \todo Use mln_trait_op_less as return type (?)
 */

# include <mln/util/less_than.hh>


namespace mln
{

  namespace util
  {

    /*! \brief Function-object to define a less-than comparison
     *  between a couple of objects with the same type \p T.
     *
     * This implementation relies on mln::util::less_than so prefer to
     * overload mln::util::less_than (because it is more general).
     *
     * \see mln::util::less_than
     */
    template <typename T>
    struct less
    {
      bool operator()(const T& lhs, const T& rhs) const;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    bool
    less<T>::operator()(const T& lhs, const T& rhs) const
    {
      static const less_than<T,T> lt_ = less_than<T,T>();
      return lt_(lhs, rhs);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_UTIL_LESS_HH
