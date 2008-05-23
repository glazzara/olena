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

#ifndef MLN_CORE_INTERNAL_PSEUDO_SITE_BASE_HH
# define MLN_CORE_INTERNAL_PSEUDO_SITE_BASE_HH

/*! \file mln/core/internal/pseudo_site_base.hh
 *
 * \brief Base class to factor code for pseudo site classes.
 */

# include <mln/core/concept/pseudo_site.hh>


namespace mln
{

  namespace internal
  {

    /*! \internal A base class for pseudo sites.
     *
     * Parameter \c P is FIXME: a point site type.
     */
    template <bool is_mutable, typename P, typename E>
    struct pseudo_site_base_ : Pseudo_Site<E>,

                               proxy_impl<P, E>,

                               site_impl< is_mutable,
					  typename site_from<P>::ret,
					  E >
    {

      // The associated site type.
      typedef typename internal::site_from<P>::ret site;

    protected:
      pseudo_site_base_();
    };


#ifndef MLN_INCLUDE_ONLY

    template <bool is_mutable, typename P, typename E>
    inline
    pseudo_site_base_<is_mutable, P, E>::pseudo_site_base_()
    {
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_PSEUDO_SITE_BASE_HH
