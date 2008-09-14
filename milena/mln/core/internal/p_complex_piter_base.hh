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

# include <limits>

# include <mln/core/internal/point_iterator_base.hh>
# include <mln/core/p_complex.hh>
# include <mln/core/complex_psite.hh>
# include <mln/core/complex_iter.hh>

namespace mln
{
  /* FIXME: Get rid of P?  */

  // Forward declarations.
  template <unsigned D, typename P> class p_complex;
  template <unsigned D, typename P> class complex_psite;

  namespace internal
  {

    /*---------------------------------.
    | p_complex_piter_base_<I, P, E>.  |
    `---------------------------------*/

    /// \brief Factoring class for iterators on mln::p_complex.
    ///
    /// \arg \p I The type of the underlying complex iterator.
    /// \arg \p P The associated point type.
    /// \arg \p E The type exact type of the iterator.
    template <typename I, typename P, typename E>
    class p_complex_piter_base_ : public point_iterator_base_< P, E >
    {
      typedef p_complex_piter_base_<I, P, E> self_;
      typedef point_iterator_base_< P, E > super_;

      /// The type of the underlying complex iterator.
      typedef I iter;

    public:
      typedef p_complex<iter::complex_dim, P> pset;
      typedef complex_psite<iter::complex_dim, P> psite;
      typedef P point;
      typedef mln_coord(point) coord;

      /// Construction and assignment.
      /// \{
      p_complex_piter_base_(const pset& pc);
      p_complex_piter_base_(const self_& rhs);
      self_& operator= (const self_& rhs);
      /// \}

      /// Manipulation.
      /// \{
      /// Test if the iterator is valid.
      bool is_valid() const;
      /// Invalidate the iterator.
      void invalidate();
      /// Start an iteration.
      void start();

      /// Go to the next point.
      void next_();
      /// Update the internal data of the iterator.
      void update_();
      /// \}

      /// Conversion and accessors.
      /// \{
      /// Reference to the corresponding point.
      // FIXME: Don't use this method (dummy value).
      const point& to_point () const;
      /// Reference to the corresponding point site.
      const psite& to_psite () const;
      /// Convert the iterator into a line graph psite.
      operator psite() const;

      /// Read-only access to the \a i-th coordinate.
      // FIXME: Don't use this operator (dummy value).
      coord operator[](unsigned i) const;
      /// \}

    private:
      /// The underlying complex iterator.
      iter iter_;
      /// The psite corresponding to this iterator.
      psite psite_;
      /// \brief The point associated to this psite.
      // FIXME: Actually, this is a dummy value!
      point p_;
    };


    /* FIXME: This hand-made delegation is painful.  We should rely on
       the general mechanism provided by Point_Site.  But then again, we
       need to refine/adjust the interface of Point_Site w.r.t. the
       mandatory conversions to points.  */
    template <typename I, typename P, typename E>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const p_complex_piter_base_<I, P, E>& p);




# ifndef MLN_INCLUDE_ONLY

    /*---------------------------------.
    | p_complex_piter_base_<I, P, E>.  |
    `---------------------------------*/

    template <typename I, typename P, typename E>
    inline
    p_complex_piter_base_<I, P, E>::p_complex_piter_base_(const pset& pc)
      // Initialize psite_ and p_ a dummy values.
      : psite_(),
	p_()
    {
      iter_.set_cplx(pc.cplx());
      mln_postcondition(!is_valid());
    }

    template <typename I, typename P, typename E>
    inline
    p_complex_piter_base_<I, P, E>::p_complex_piter_base_(const p_complex_piter_base_<I, P, E>& rhs)
      : iter_(rhs.iter_),
	psite_(rhs.psite_),
	// Dummy value.
	p_()
    {
    }

    template <typename I, typename P, typename E>
    inline
    p_complex_piter_base_<I, P, E>&
    p_complex_piter_base_<I, P, E>::operator=(const p_complex_piter_base_<I, P, E>& rhs)
    {
      if (&rhs == this)
	return *this;
      iter_ = rhs.iter_;
      psite_ = rhs.psite_;
      return *this;
    }

    template <typename I, typename P, typename E>
    inline
    mln_coord(P)
    p_complex_piter_base_<I, P, E>::operator[](unsigned i) const
    {
      // Dummy value.
      return p_[i];
    }

    template <typename I, typename P, typename E>
    inline
    bool
    p_complex_piter_base_<I, P, E>::is_valid() const
    {
      return iter_.is_valid();
    }

    template <typename I, typename P, typename E>
    inline
    void
    p_complex_piter_base_<I, P, E>::invalidate()
    {
      iter_.invalidate();
    }

    template <typename I, typename P, typename E>
    inline
    void
    p_complex_piter_base_<I, P, E>::start()
    {
      iter_.start();
      update_();
    }

    template <typename I, typename P, typename E>
    inline
    void
    p_complex_piter_base_<I, P, E>::next_()
    {
      iter_.next_();
      if (is_valid())
	update_();
    }

    template <typename I, typename P, typename E>
    inline
    void
    p_complex_piter_base_<I, P, E>::update_()
    {
      // Update psite_.
      psite_ = psite(iter_);
    }

    template <typename I, typename P, typename E>
    inline
    const P&
    p_complex_piter_base_<I, P, E>::to_point() const
    {
      // Dummy value.
      return p_;
    }

    template <typename I, typename P, typename E>
    inline
    const typename p_complex_piter_base_<I, P, E>::psite&
    p_complex_piter_base_<I, P, E>::to_psite() const
    {
      /* We don't check whether the iterator is valid before returning
	 the value using

         mln_precondition(is_valid());

	 since this method may be called *before* the iterator is
	 actually initialized.  This is the case for instance when this
	 point iterator (say, P) is used to initialize another iterator
	 on window or neighborhood (say, Q); most of the time, for_all()
	 is responsible for the initialization of P, but it takes place
	 *after* the creation of Q.  */
      return psite_;
    }

    template <typename I, typename P, typename E>
    inline
    p_complex_piter_base_<I, P, E>::operator psite() const
    {
      mln_precondition(is_valid());
      return psite_;
    }


    template <typename I, typename P, typename E>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const p_complex_piter_base_<I, P, E>& p)
    {
      /* FIXME: We should use p.to_psite() here, but as it lacks the
	 precondition the conversion operator has, so we use the latter.

	 We should
	 - rename `to_psite' as `to_psite_';
	 - write a new `to_psite' routine checking the validity of the
         iterator;
	 - have the conversion operator to psite use this new `to_psite'
         routine;
	 - adjust former clients of `to_psite'

	 This is a general remark that applies to all point/psite
	 iterators of Milena.  */
      return ostr << static_cast< mln_psite(E) >(p);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of mln::internal

} // end of mln


#endif // ! MLN_CORE_INTERNAL_P_COMPLEX_PITER_BASE_HH
