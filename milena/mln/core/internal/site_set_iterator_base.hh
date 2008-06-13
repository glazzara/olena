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

#ifndef MLN_CORE_INTERNAL_SITE_SET_ITERATOR_BASE_HH
# define MLN_CORE_INTERNAL_SITE_SET_ITERATOR_BASE_HH

/*! \file mln/core/internal/site_set_iterator_base.hh
 *
 * \brief Base class to factor code for iterator classes directly
 * working on site sets.
 */

# include <mln/core/internal/site_iterator_base.hh>


namespace mln
{

  namespace internal
  {

    /*! \internal A base class for site iterators.
     *
     * Parameter \c S is the targeted site set type.
     */
    template <typename S, typename E>
    struct site_set_iterator_base : site_iterator_base<S, E>
    {

      /// Give the site set that this iterator browses.
      const S& site_set() const;

    protected:

      site_set_iterator_base();
    };


#ifndef MLN_INCLUDE_ONLY

    template <typename S, typename E>
    inline
    site_set_iterator_base<S, E>::site_set_iterator_base()
    {
    }

    template <typename S, typename E>
    inline
    const S&
    site_set_iterator_base<S, E>::site_set() const
    {
      mln_precondition(this->s_ != 0);
      return *this->s_;
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_SITE_SET_ITERATOR_BASE_HH
