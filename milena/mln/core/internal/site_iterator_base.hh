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

#ifndef MLN_CORE_INTERNAL_SITE_ITERATOR_BASE_HH
# define MLN_CORE_INTERNAL_SITE_ITERATOR_BASE_HH

/*! \file mln/core/internal/site_iterator_base.hh
 *
 * \brief Base class to factor code for site iterator classes.
 */

# include <mln/core/concept/site_iterator.hh>


namespace mln
{

  namespace internal
  {

    /*! \internal A base class for site iterators.
     *
     * Parameter \c P is FIXME: a point site type.
     */
    template <typename P, typename E>
    struct site_iterator_base_ : Site_Iterator<E>,

                                 proxy_impl<P, E>,

                                 site_impl< false, // Constant access to site / subject.
					    typename site_from<P>::ret,
					    E >
    {

      // The associated site type.
      typedef typename internal::site_from<P>::ret site;

      // The associated subject type (as a Proxy).
      typedef P subject;

      // The associated q_subject type (as a Proxy).
      typedef const P& q_subject;

      /*! \brief Conversion towards the site it designates.
       *
       * \warning This is a final method; iterator classes should not
       * re-defined this method.
       *
       * \pre The iterator is valid.
       */ 
      operator site() const;

    protected:
      site_iterator_base_();
    };


#ifndef MLN_INCLUDE_ONLY

    template <typename P, typename E>
    inline
    site_iterator_base_<P, E>::site_iterator_base_()
    {
    }

    template <typename P, typename E>
    inline
    site_iterator_base_<P, E>::operator site() const
    {
      typedef proxy_impl<P, E> super;
      mln_precondition(exact(this)->is_valid());
      return this->super::operator site();
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_SITE_ITERATOR_BASE_HH
