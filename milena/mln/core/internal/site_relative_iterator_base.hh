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

#ifndef MLN_CORE_SITE_RELATIVE_ITERATOR_BASE_HH
# define MLN_CORE_SITE_RELATIVE_ITERATOR_BASE_HH

/*! \file mln/core/internal/site_relative_iterator_base.hh
 *
 * \brief Definition of forward and backward mln::dpoint_ based
 * iterators.
 *
 * \todo Add a method to get the site set (if the center is defined) or
 * the site set at a given center.
 */

# include <vector>
# include <mln/core/internal/site_iterator_base.hh>


namespace mln
{

  namespace internal
  {

    /*! \brief A generic forward iterator on points of windows and of
     *  neighborhoods.
     *
     * Parameter \c S is the targeted "site set definition" type.  It
     * can be either a Window, or a Neighborhood.
     *
     * IMPORTANT: Sub-classes have to define do_start_, do_next_, and
     * compute_p_.  They shall define NEITHER start_ NOR next_.
     */
    template <typename S, typename E>
    class site_relative_iterator_base : public site_iterator_base< S, E >
    {
    public:

      /// Constructor witout argument.
      site_relative_iterator_base();

      template <typename P>
      void center_at(const P& c);

      /// Start an iteration.
      void start_();

      /// Go to the next point.
      void next_();

      /// The psite around which this iterator moves.
      const mln_psite(S)& center() const;

      /// This overriding is very useful: it adds a test to prevent
      /// getting an invalid iterator when its center has moved.  Some
      /// sub-classes provide an update() method for the client to say
      /// that she really want to read the iterator just after the
      /// center has changed.
      const mln_psite(S)& subj_();

      /// Hook to the current location.
      const mln_psite(S)& p_hook_() const;

      /// Change the site set targeted by this iterator. 
      void change_target(const S& s);

      /// Warning: this method is an advanced tool provided for very
      /// special occasions; if you are a regular user, you should
      /// never use it!  Calling this method forces this relative
      /// iterator to synchronize with its center when it has just
      /// moved and when the relative iterator still makes sense.
      E& update();

    protected:

      /// A pointer to the center psite around which this iterator
      /// moves.
      const mln_psite(S)* c_;

    private:
      
      /// The psite designated by this iterator.
      mln_psite(S) p_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename S, typename E>
    inline
    site_relative_iterator_base<S,E>::site_relative_iterator_base()
      : c_(0)
    {
      void (E::*m1)() = & E::do_start_;
      m1 = 0;
      void (E::*m2)() = & E::do_next_;
      m2 = 0;
      mln_psite(S) (E::*m3)() const = & E::compute_p_;
      m3 = 0;
    }

    template <typename S, typename E>
    template <typename P>
    inline
    void
    site_relative_iterator_base<S,E>::center_at(const P& c)
    {
      internal::get_adr(c_, c);
      this->invalidate();
    }

    template <typename S, typename E>
    inline
    void
    site_relative_iterator_base<S,E>::start_()
    {
      exact(this)->do_start_();
      if (this->is_valid())
	p_ = exact(this)->compute_p_();
    }

    template <typename S, typename E>
    inline
    void
    site_relative_iterator_base<S,E>::next_()
    {
      exact(this)->do_next_();
      if (this->is_valid())
	p_ = exact(this)->compute_p_();
    }

    template <typename S, typename E>
    inline
    const mln_psite(S)&
    site_relative_iterator_base<S,E>::center() const
    {
      mln_precondition(c_ != 0);
      return *c_;
    }

    template <typename S, typename E>
    inline
    const mln_psite(S)&
    site_relative_iterator_base<S,E>::subj_()
    {
      mln_psite(S) p_now = exact(this)->compute_p_();
      mln_assertion(p_now == p_);
      return p_;
    }

    template <typename S, typename E>
    inline
    const mln_psite(S)&
    site_relative_iterator_base<S,E>::p_hook_() const
    {
      return p_;
    }

    template <typename S, typename E>
    inline
    void
    site_relative_iterator_base<S,E>::change_target(const S& s)
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
    E&
    site_relative_iterator_base<S,E>::update()
    {
      mln_precondition(this->s_ && c_);
      p_ = exact(this)->compute_p_();
      mln_postcondition(this->is_valid());
      return exact(*this);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_SITE_RELATIVE_ITERATOR_BASE_HH
