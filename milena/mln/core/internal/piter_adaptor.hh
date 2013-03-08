// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_INTERNAL_PITER_ADAPTOR_HH
# define MLN_CORE_INTERNAL_PITER_ADAPTOR_HH

/// \file
///
/// \brief Definition of iterators on points of boxes.
///
/// \todo Rename as site_iterator_adaptor_base.
///
/// \todo Distinguish between adaptors of site_set_iterator,
/// site_relative_iterator, etc. (?)

# include <mln/core/internal/site_iterator_base.hh>


namespace mln
{

  namespace internal
  {

    /*!
      \internal
      \brief A base class for point iterator adaptors.

      Parameter \c Pi is the type of the point iterator adaptee;
      parameter E is the exact type.
    */
    template <typename Pi, typename S, typename E>
    class piter_adaptor_ : public internal::site_iterator_base< S, E >
    {
    public:

      /// Constructor without argument.
      piter_adaptor_();

      /// Constructor from a point iterator \p piter.
      piter_adaptor_(const Pi& piter);

      /// Test the iterator validity.
      bool is_valid_() const;

      /// Invalidate the iterator.
      void invalidate_();

      /// Start an iteration.
      void start_();

      /// Go to the next point.
      void next_();

      /// Hook to the current location.
      const mln_psite(S)& p_hook_() const;

      /// Change the site set targeted by this iterator.
      void change_target(const S& s);

      /// Change the site set targeted by pi_.  This default impl is a
      /// no-op.  This method might be overridden.
      void pi_change_target_(const S& s);

    protected:

      /// The adaptee site iterator.
      Pi pi_;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename Pi, typename S, typename E>
    inline
    piter_adaptor_<Pi,S,E>::piter_adaptor_()
    {
    }

    template <typename Pi, typename S, typename E>
    inline
    piter_adaptor_<Pi,S,E>::piter_adaptor_(const Pi& pi)
      : pi_(pi)
    {
      invalidate_();
    }

    template <typename Pi, typename S, typename E>
    inline
    bool
    piter_adaptor_<Pi,S,E>::is_valid_() const
    {
      return pi_.is_valid();
    }

    template <typename Pi, typename S, typename E>
    inline
    void
    piter_adaptor_<Pi,S,E>::invalidate_()
    {
      pi_.invalidate();
    }

    template <typename Pi, typename S, typename E>
    inline
    void
    piter_adaptor_<Pi,S,E>::start_()
    {
      pi_.start();
    }

    template <typename Pi, typename S, typename E>
    inline
    void
    piter_adaptor_<Pi,S,E>::next_()
    {
      pi_.next();
    }

    template <typename Pi, typename S, typename E>
    inline
    const mln_psite(S)&
    piter_adaptor_<Pi,S,E>::p_hook_() const
    {
      return pi_.p_hook_();
    }

    template <typename Pi, typename S, typename E>
    inline
    void
    piter_adaptor_<Pi,S,E>::change_target(const S& s)
    {
      this->s_ = & s;
      // p might be also updated since it can hold a pointer towards
      // the set it designates, so:
      exact(this)->pi_change_target_(s);
      // Last:
      this->invalidate();
    }

    template <typename Pi, typename S, typename E>
    inline
    void
    piter_adaptor_<Pi,S,E>::pi_change_target_(const S& s)
    {
      this->pi_.change_target(s);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_PITER_ADAPTOR_HH
