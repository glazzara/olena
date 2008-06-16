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

#ifndef MLN_UTIL_LESS_THAN_HH
# define MLN_UTIL_LESS_THAN_HH

/*! \file mln/util/less_than.hh
 *
 * \brief Definition of a "less-than" function-object with a couple of
 * types (L and R).
 *
 * \todo Use mln_trait_op_less as return type (?)
 */


namespace mln
{

  namespace util
  {

    /*! \brief Function-object to define a less-than comparison
     *  between a couple of objects respectively with types \p L and
     *  \p R.
     *
     * This is a default implementation that relies on 'operator<'.
     * It shall be overloaded for types that do not have such an
     * operator when those types are involved with an ordered
     * container.
     *
     * \see mln::util::less_than
     */
    template <typename L, typename R>
    struct less_than
    {
      bool operator()(const L& lhs, const R& rhs) const;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename L, typename R>
    inline
    bool
    less_than<L,R>::operator()(const L& lhs, const R& rhs) const
    {
      return lhs < rhs;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_UTIL_LESS_THAN_HH
