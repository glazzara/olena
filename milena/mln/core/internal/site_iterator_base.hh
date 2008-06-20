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
 *
 * \todo Make p_ private and add a p() method that can be overridden
 * so that a subclass can use some other info than this attribute.
 * See for instance start_ and next_ in pset_if_piter.hh: we have
 * both pi_ and p_ to designate the current site. 
 */

# include <mln/core/concept/site_iterator.hh>
# include <mln/core/concept/pseudo_site.hh> // Use of if_possible::change_target.

// site_iterator_base<S>  where S is a Site_Set
// {
//   s_ : const S*
// }




namespace mln
{

  namespace internal
  {

    /*! \internal A base class for site iterators.
     *
     * NEVER DIRECTLY DERIVE FROM THIS CLASS.
     *
     * Instead derive from EITHER site_set_iterator_base OR
     * site_relative_iterator_base.
     *
     * Parameter \c S is the targeted site set type.
     */
    template <typename S, typename E>
    struct site_iterator_base : Site_Iterator<E>,

                                proxy_impl< mln_psite(S), E>,

                                site_impl< false, // Constant access to site / subject.
					   mln_site(S),
					   E >
    {
      /// The associated site type (as a Site_Proxy).
      typedef mln_site(S) site;

      /// Return the site it points to (as a Site_Proxy).
      const mln_site(S)& to_site() const;

      /*! \brief Conversion towards the site it designates (as a Site_Proxy).
       *
       * \warning This is a final method; iterator classes should not
       * re-defined this method.
       *
       * \pre The iterator is valid.
       */ 
      operator mln_site(S)() const;

      /// The associated subject type (as a Proxy).
      typedef mln_psite(S) subject;

      /// The associated q_subject type (as a Proxy).
      typedef const mln_psite(S)& q_subject;

      /// Give the subject (required by the Proxy interface).
      const mln_psite(S)& unproxy() const;

      /// Give the target address.  It might be 0.
      const S*& target_();

      /// Change the iterator target.
      void change_target(const S& s);

            mln_psite(S)& p()        { return exact(this)->current_p_(); }
      const mln_psite(S)& p() const  { return exact(this)->current_p_(); }

            mln_psite(S)& current_p_()       { return p_; }
      const mln_psite(S)& current_p_() const { return p_; }
      
    protected:

      site_iterator_base();

      /// The target.
      const S* s_;

    private:
      
      /// The psite designated by this iterator.
      mln_psite(S) p_;
    };


#ifndef MLN_INCLUDE_ONLY

    template <typename S, typename E>
    inline
    site_iterator_base<S, E>::site_iterator_base()
    {
    }

    template <typename S, typename E>
    inline
    site_iterator_base<S, E>::operator mln_site(S)() const
    {
      typedef proxy_impl<mln_psite(S), E> super;
      mln_precondition(exact(this)->is_valid());
      return this->super::operator site(); // Featured by internal::proxy_impl<*>.
    }

    template <typename S, typename E>
    inline
    const mln_site(S)&
    site_iterator_base<S, E>::to_site() const
    {
      mln_precondition(exact(*this).is_valid()); // FIXME: OK?
      return internal::to_site( p() );
    }

    template <typename S, typename E>
    inline
    const mln_psite(S)&
    site_iterator_base<S, E>::unproxy() const
    {
      return p();
    }

    template <typename S, typename E>
    inline
    const S*&
    site_iterator_base<S, E>::target_()
    {
      return s_;
    }

    template <typename S, typename E>
    inline
    void
    site_iterator_base<S, E>::change_target(const S& s)
    {
      s_ = & s;
      // p might be also updated since it can hold a pointer towards
      // the set it designates, so:
      if_possible::change_target(p(), s);
      // Last:
      this->invalidate();
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_SITE_ITERATOR_BASE_HH
