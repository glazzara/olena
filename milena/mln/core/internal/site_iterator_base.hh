// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file mln/core/internal/site_iterator_base.hh
///
/// Base class to factor code for site iterator classes.


# include <mln/core/concept/site_iterator.hh>
# include <mln/core/concept/pseudo_site.hh> // Use of if_possible::change_target.



namespace mln
{

  namespace internal
  {

    /// A base class for site iterators.
    ///
    /// NEVER DIRECTLY DERIVE FROM THIS CLASS.
    ///
    /// Instead derive EITHER from site_set_iterator_base OR from
    /// site_relative_iterator_base OR from piter_adaptor_.
    ///
    /// Parameter \c S is the targeted "site set definition" type.  It
    /// can be either a Site_Set, a Window, or a Neighborhood.
    ///
    template <typename S, typename E>
    struct site_iterator_base : Site_Iterator<E>,
                                proxy_impl< const mln_psite(S)&, E>
    {
      /// The associated target type.
      typedef S target;

      /// The associated site type (as a Site_Proxy).
      typedef mln_site(S) site;

      /// Return the site it points to (as a Site_Proxy).
      const mln_site(S)& to_site() const;


      // Technical note:
      //
      //   A conversion operator towards the site it designates is
      //   not required since it is inherited from 'proxy_impl'.
      //   Defining '::operator mln_site(S)() const' here can lead to
      //   ambiguities in very special cases since we inherit a couple
      //   of conversions (const and non-const)


      /// Give the subject (required by the Proxy interface).
      const mln_psite(S)& subj_();

      /// Give the target address.  It might be 0.
      const S*& target_();

    protected:

      site_iterator_base();

      /// The target.
      const S* s_;

    private:

      typedef proxy_impl< const mln_psite(S)&, E> super_;
    };


#ifndef MLN_INCLUDE_ONLY

    template <typename S, typename E>
    inline
    site_iterator_base<S, E>::site_iterator_base()
      : s_(0)
    {
    }

    template <typename S, typename E>
    inline
    const mln_site(S)&
    site_iterator_base<S, E>::to_site() const
    {
      mln_precondition(exact(*this).is_valid());
      return this->get_subject();
    }

    template <typename S, typename E>
    inline
    const mln_psite(S)&
    site_iterator_base<S, E>::subj_()
    {
      return exact(this)->p_hook_();
    }

    template <typename S, typename E>
    inline
    const S*&
    site_iterator_base<S, E>::target_()
    {
      return s_;
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_SITE_ITERATOR_BASE_HH
