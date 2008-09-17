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

    /*! A base class for site iterators.
     *
     * Parameter \c S is the targeted site set type.
     *
     * IMPORTANT: Sub-classes have to define start_, next_,
     * is_valid_ and invalidate_.
     */
    template <typename S, typename E>
    class site_set_iterator_base : public site_iterator_base<S, E>
    {
    public:

      /// Give the site set that this iterator browses.
      const S& site_set() const;

      /// Hook to the current location.
      const mln_psite(S)& p_hook_() const;

      /// Change the site set targeted by this iterator.
      void change_target(const S& s);

    protected:

      /// The psite designated by this iterator.
      mln_psite(S) p_;

      /// Constructor without argument.
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

    template <typename S, typename E>
    inline
    void
    site_set_iterator_base<S, E>::change_target(const S& s)
    {
      this->s_ = & s;
      // p might be also updated since it can hold a pointer towards
      // the set it designates, so:
      if_possible::change_target(p_, s);
      // Last:
      this->invalidate();
    }

    template <typename S, typename E>
    inline
    const mln_psite(S)&
    site_set_iterator_base<S, E>::p_hook_() const
    {
      return p_;
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_SITE_SET_ITERATOR_BASE_HH
