// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_INTERNAL_P_COMPLEX_PITER_BASE_HH
# define MLN_CORE_INTERNAL_P_COMPLEX_PITER_BASE_HH

/// \file mln/core/internal/p_complex_piter_base.hh
/// \brief Definition of an implementation (factoring) class for
/// iterators on mln::p_complex.

# include <mln/core/internal/site_set_iterator_base.hh>

/* FIXME: Rename internal::p_complex_piter_base_ to something else, as
   it is also used for p_faces piters.  Maybe
   internal::complex_piter_base_, but it is really close to
   internal::complex_iter_base_... */

namespace mln
{

  namespace internal
  {

    /*------------------------------------.
    | p_complex_piter_base_<I, S, P, E>.  |
    `------------------------------------*/

    // FIXME: P could probably be deduced from S.

    /// \brief Factoring class for iterators on mln::p_complex.
    ///
    /// \arg \p I The type of the underlying complex iterator.
    /// \arg \p S The associated site set type.
    /// \arg \p P The associated site type.
    /// \arg \p E The type exact type of the iterator.
    template <typename I, typename S, typename P, typename E>
    class p_complex_piter_base_
      : public internal::site_set_iterator_base< S,
						 p_complex_piter_base_<I, S, P, E> >
    {
      typedef p_complex_piter_base_<I, S, P, E> self_;
      typedef internal::site_set_iterator_base< S, self_ > super_;

      /// The type of the underlying complex iterator.
      typedef I iter;

    public:
      /// Construction and assignment.
      /// \{
      p_complex_piter_base_();
      p_complex_piter_base_(const S& pc);
      /// \}

      /// Manipulation.
      /// \{
    public:
      /// Test if the iterator is valid.
      bool is_valid_() const;
      /// Invalidate the iterator.
      void invalidate_();

      /// Start an iteration.
      void start_();
      /// Go to the next point.
      void next_();

    private:
      /// Update the psite.
      void update_();
      /// \}

    protected:
      /// The psite corresponding to this iterator.
      using super_::p_;
      /// The underlying complex iterator.
      iter iter_;
    };


    /// Print an mln::p_complex_piter_base_<I, S, P, E>.
    template <typename I, typename S, typename P, typename E>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const p_complex_piter_base_<I, S, P, E>& p);



# ifndef MLN_INCLUDE_ONLY

    /*------------------------------------.
    | p_complex_piter_base_<I, S, P, E>.  |
    `------------------------------------*/

    template <typename I, typename S, typename P, typename E>
    inline
    p_complex_piter_base_<I, S, P, E>::p_complex_piter_base_()
    {
      mln_postcondition(!this->is_valid());
    }

    template <typename I, typename S, typename P, typename E>
    inline
    p_complex_piter_base_<I, S, P, E>::p_complex_piter_base_(const S& pc)
    {
      this->change_target(pc);
      iter_.set_cplx(pc.cplx());
      mln_postcondition(!this->is_valid());
    }

    template <typename I, typename S, typename P, typename E>
    inline
    bool
    p_complex_piter_base_<I, S, P, E>::is_valid_() const
    {
      return iter_.is_valid();
    }

    template <typename I, typename S, typename P, typename E>
    inline
    void
    p_complex_piter_base_<I, S, P, E>::invalidate_()
    {
      iter_.invalidate();
    }

    template <typename I, typename S, typename P, typename E>
    inline
    void
    p_complex_piter_base_<I, S, P, E>::start_()
    {
      iter_.start();
      if (this->is_valid())
	update_();
    }

    template <typename I, typename S, typename P, typename E>
    inline
    void
    p_complex_piter_base_<I, S, P, E>::next_()
    {
      iter_.next_();
      if (this->is_valid())
	update_();
    }

    template <typename I, typename S, typename P, typename E>
    inline
    void
    p_complex_piter_base_<I, S, P, E>::update_()
    {
      mln_precondition(this->is_valid());
      // Update psite_.
      typedef mln_psite(S) psite;
      p_ = psite(exact(this)->site_set(), iter_);
    }


    template <typename I, typename S, typename P, typename E>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const p_complex_piter_base_<I, S, P, E>& p)
    {
      return ostr << p.unproxy_();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of mln::internal

} // end of mln


#endif // ! MLN_CORE_INTERNAL_P_COMPLEX_PITER_BASE_HH
