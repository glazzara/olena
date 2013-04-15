// Copyright (C) 2008, 2009, 2011, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_INTERNAL_SITE_RELATIVE_ITERATOR_BASE_HH
# define MLN_CORE_INTERNAL_SITE_RELATIVE_ITERATOR_BASE_HH

/// \file
///
/// Definition of forward and backward mln::dpoint_ based
/// iterators.
///
/// \todo Add a method to get the site set (if the center is defined) or
/// the site set at a given center.


# include <vector>
# include <mln/core/internal/site_iterator_base.hh>
# include <mln/metal/converts_to.hh>


namespace mln
{

  namespace internal
  {

    /*!
      \brief A generic iterator on points of windows and of
      neighborhoods.

      Parameter \c S is the targeted "site set definition" type.  It
      can be either a Window, or a Neighborhood.

      IMPORTANT: Sub-classes have to define center_at_, do_start_, do_next_,
      is_valid_, invalidate_ and compute_p_.  They shall define
      NEITHER start_ NOR next_.
    */
    template <typename S, typename E, typename C = mln_psite(S)>
    class site_relative_iterator_base : public site_iterator_base< S, E >
    {
    public:

      /// Constructor witout argument.
      site_relative_iterator_base();

      template <typename P>
      void center_at(const P& c);

      /// \cond INTERNAL_API
      /// Start an iteration.
      void start_();

      /// Go to the next point.
      void next_();
      /// \endcond

      /// Give the site set (neighborhood or window) that this
      /// iterator browses.
      const S& site_set() const;

      /// The psite around which this iterator moves.
      const C& center() const;

      /// \cond INTERNAL_API
      /// This overriding is very useful: it adds a test to prevent
      /// getting an invalid iterator when its center has moved.  Some
      /// sub-classes provide an update() method for the client to say
      /// that she really want to read the iterator just after the
      /// center has changed.
      const mln_psite(S)& subj_();

      /// Hook to the current location.
      const mln_psite(S)& p_hook_() const;
      /// \endcond

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
      const C* c_;

    private:

      // Allows inherited classes to do extra work while centering.
      // Default implementation.
      template <typename P>
      void center_at_(const P& c);

      /// The psite designated by this iterator.
      mln_psite(S) p_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename S, typename E, typename C>
    inline
    site_relative_iterator_base<S,E,C>::site_relative_iterator_base()
      : c_(0)
    {
      void (E::*m1)() = & E::do_start_;
      (void) m1;
      void (E::*m2)() = & E::do_next_;
      (void) m2;
      mln_psite(S) (E::*m3)() const = & E::compute_p_;
      (void) m3;
    }

    template <typename S, typename E, typename C>
    template <typename P>
    inline
    void
    site_relative_iterator_base<S,E,C>::center_at(const P& c)
    {
      mlc_converts_to(P, const C&)::check();
      c_ = & static_cast< const C& >(c);
      exact(this)->center_at_(c); // May call some extra code.
      this->invalidate();
    }

    template <typename S, typename E, typename C>
    inline
    void
    site_relative_iterator_base<S,E,C>::start_()
    {
      exact(this)->do_start_();
      if (this->is_valid())
	p_ = exact(this)->compute_p_();
    }

    template <typename S, typename E, typename C>
    inline
    void
    site_relative_iterator_base<S,E,C>::next_()
    {
      exact(this)->do_next_();
      if (this->is_valid())
	p_ = exact(this)->compute_p_();
    }

    template <typename S, typename E, typename C>
    inline
    const C&
    site_relative_iterator_base<S,E,C>::center() const
    {
      mln_precondition(c_ != 0);
      return *c_;
    }

    template <typename S, typename E, typename C>
    inline
    const S&
    site_relative_iterator_base<S,E,C>::site_set() const
    {
      mln_precondition(this->s_ != 0);
      return *this->s_;
    }

    template <typename S, typename E, typename C>
    inline
    const mln_psite(S)&
    site_relative_iterator_base<S,E,C>::subj_()
    {
      mln_assertion(exact(this)->compute_p_() == p_);
      return p_;
    }

    template <typename S, typename E, typename C>
    inline
    const mln_psite(S)&
    site_relative_iterator_base<S,E,C>::p_hook_() const
    {
      return p_;
    }

    template <typename S, typename E, typename C>
    inline
    void
    site_relative_iterator_base<S,E,C>::change_target(const S& s)
    {
      this->s_ = & s;
      // p might be also updated since it can hold a pointer towards
      // the set it designates, so:
      if_possible::change_target(p_, s);
      // Last:
      this->invalidate();
    }

    template <typename S, typename E, typename C>
    inline
    E&
    site_relative_iterator_base<S,E,C>::update()
    {
      mln_precondition(this->s_ && c_);
      p_ = exact(this)->compute_p_();
      mln_postcondition(this->is_valid());
      return exact(*this);
    }

    template <typename S, typename E, typename C>
    template <typename P>
    inline
    void
    site_relative_iterator_base<S,E,C>::center_at_(const P&)
    {
      // Default is no-op, meaning "no extra code".
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_SITE_RELATIVE_ITERATOR_BASE_HH
