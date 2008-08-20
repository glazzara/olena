// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_INTERNAL_SITE_SET_BASE_HH
# define MLN_CORE_INTERNAL_SITE_SET_BASE_HH

/*! \file mln/core/internal/site_set_base.hh
 *
 * \brief Definition of the common base class for all site set
 * classes.
 */

# include <mln/core/concept/site_set.hh>
# include <mln/core/concept/site_proxy.hh>


namespace mln
{

  namespace internal
  {


    /*! \internal A base class for site set classes.
     * \p P is a psite type.
     */
    template <typename P, typename E>
    struct site_set_base_ : public Site_Set<E>
    {
      /// Site associated type.
      typedef typename subject<P>::ret site;

      /// Test if the site set is empty.  This final method dispatches
      /// to is_empty_ whose default implementation relies on the
      /// 'nsites' method.
      bool is_empty() const;

    protected:
      site_set_base_();

    private:
      // Default impl based on the number of sites.  It can be
      // overridden in subclasses that do not feature the 'nsites'
      // method.
      bool is_empty_() const;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename S, typename E>
    inline
    site_set_base_<S,E>::site_set_base_()
    {
    }

    template <typename S, typename E>
    inline
    bool
    site_set_base_<S,E>::is_empty() const
    {
      return exact(this)->is_empty_();
    }

    template <typename S, typename E>
    inline
    bool
    site_set_base_<S,E>::is_empty_() const
    {
      return exact(this)->nsites() == 0;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_SITE_SET_BASE_HH
