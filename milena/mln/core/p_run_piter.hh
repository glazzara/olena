// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_P_RUN_PITER_HH
# define MLN_CORE_P_RUN_PITER_HH

/*! \file mln/core/p_run_piter.hh
 *
 * \brief Definition of point iterators on mln::p_run.
 */

# include <mln/core/p_run.hh>


namespace mln
{

  /*! \brief Forward iterator on points of a p_run<P>.
   *
   */
  template <typename P>
  struct p_run_fwd_piter_ : public internal::site_iterator_base_< P, p_run_fwd_piter_<P> >
  {
    typedef p_run_fwd_piter_<P> self_;
    typedef internal::site_iterator_base_< P, self_ > super_;
  public:
    
    // Make definitions from super class available.
    enum { dim = super_::dim };

    /// Constructor without arguments.
    p_run_fwd_piter_();

    /// Coordinate associated type.
    p_run_fwd_piter_(const p_run<P>& pr);

    /// Assign a new run to iterate.
    void assign_run(const p_run<P>& pr);

    /// Reference of the corresponding point.
    const P& to_point() const;

    /// Read-only access to the \p i-th coordinate.
    mln_coord(P) operator[](unsigned i) const;

    /// Test if the iterator is valid.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();

    /// Get the index of the point in the run.
    unsigned ind() const;

    /// Convert the iterator into a point.
    operator P() const;

  protected:
    const p_run<P>* run_;
    bool is_valid_;
    unsigned i_;
    P p_;
  };



  /*! \brief Backward iterator on points of a p_run<P>.
   *
   */
  template <typename P>
  struct p_run_bkd_piter_ : public internal::site_iterator_base_< P, p_run_bkd_piter_<P> >
  {
    typedef p_run_bkd_piter_<P> self_;
    typedef internal::site_iterator_base_< P, self_ > super_;
  public:
    
    // Make definitions from super class available.
    enum { dim = super_::dim };

    /// Constructor without arguments.
    p_run_bkd_piter_();

    /// Coordinate associated type.
    p_run_bkd_piter_(const p_run<P>& pr);

    /// Assign a new run to iterate.
    void assign_run(const p_run<P>& pr);

    /// Reference of the corresponding point.
    const P& to_point() const;

    /// Read-only access to the \p i-th coordinate.
    mln_coord(P) operator[](unsigned i) const;

    /// Test if the iterator is valid.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();

    /// Get the index of the point in the run.
    unsigned ind() const;

    /// Convert the iterator into a point.
    operator P() const;

  protected:
    const p_run<P>* run_;
    bool is_valid_;
    unsigned i_;
    P p_;
  };



# ifndef MLN_INCLUDE_ONLY

  // p_run_fwd_piter_<P>

  template <typename P>
  inline
  p_run_fwd_piter_<P>::p_run_fwd_piter_()
    : run_ (0),
      is_valid_(false)
  {
  }

  template <typename P>
  inline
  p_run_fwd_piter_<P>::p_run_fwd_piter_(const p_run<P>& pr)
    : run_(&pr),
      is_valid_(false)
  {
  }

  template <typename P>
  inline
  void
  p_run_fwd_piter_<P>::assign_run(const p_run<P>& pr)
  {
    run_ = &pr;
    is_valid_ = false;
  }

  template <typename P>
  inline
  const P&
  p_run_fwd_piter_<P>::to_point() const
  {
    mln_precondition(is_valid());
    mln_assertion(p_[P::dim - 1] - run_->first()[P::dim - 1] == signed(i_));
    return p_;
  }

  template <typename P>
  inline
  mln_coord(P)
  p_run_fwd_piter_<P>::operator[](unsigned i) const
  {
    mln_precondition(i < dim);
    mln_precondition(is_valid());
    mln_assertion(p_[P::dim - 1] - run_->first()[P::dim - 1] == signed(i_));
    return p_[i];
  }

  template <typename P>
  inline
  bool
  p_run_fwd_piter_<P>::is_valid() const
  {
    return is_valid_;
  }

  template <typename P>
  inline
  void
  p_run_fwd_piter_<P>::invalidate()
  {
    is_valid_ = false;
  }

  template <typename P>
  inline
  void
  p_run_fwd_piter_<P>::start()
  {
    p_ = run_->first();
    i_ = 0;
    is_valid_ = true;
  }

  template <typename P>
  inline
  void
  p_run_fwd_piter_<P>::next_()
  {
    mln_assertion(p_[P::dim - 1] - run_->first()[P::dim - 1] == signed(i_));
    p_[dim - 1]++;
    ++i_;
    is_valid_ = p_[dim - 1] - run_->first()[dim - 1] < (signed)run_->length();
  }

  template <typename P>
  inline
  unsigned
  p_run_fwd_piter_<P>::ind() const
  {
    mln_assertion(p_[P::dim - 1] - run_->first()[P::dim - 1] == signed(i_));
    return i_;
  }

  template <typename P>
  inline
  p_run_fwd_piter_<P>::operator P() const
  {
    mln_precondition(is_valid());
    mln_assertion(p_[P::dim - 1] - run_->first()[P::dim - 1] == signed(i_));
    return p_;
  }


  // p_run_bkd_piter_<P>

  template <typename P>
  inline
  p_run_bkd_piter_<P>::p_run_bkd_piter_()
    : run_ (0)
  {
  }

  template <typename P>
  inline
  p_run_bkd_piter_<P>::p_run_bkd_piter_(const p_run<P>& pr)
    : run_(&pr),
      is_valid_(false)
  {
  }

  template <typename P>
  inline
  void
  p_run_bkd_piter_<P>::assign_run(const p_run<P>& pr)
  {
    run_ = &pr;
    is_valid_ = false;
  }

  template <typename P>
  inline
  const P&
  p_run_bkd_piter_<P>::to_point() const
  {
    mln_precondition(is_valid());
    mln_assertion(p_[P::dim - 1] - run_->first()[P::dim - 1] == signed(i_));
    return p_;
  }

  template <typename P>
  inline
  mln_coord(P)
  p_run_bkd_piter_<P>::operator[](unsigned i) const
  {
    mln_precondition(i < dim);
    mln_precondition(is_valid());
    mln_assertion(p_[P::dim - 1] - run_->first()[P::dim - 1] == signed(i_));
    return p_[i];
  }

  template <typename P>
  inline
  bool
  p_run_bkd_piter_<P>::is_valid() const
  {
    return is_valid_;
  }

  template <typename P>
  inline
  void
  p_run_bkd_piter_<P>::invalidate()
  {
    is_valid_ = false;
  }

  template <typename P>
  inline
  void
  p_run_bkd_piter_<P>::start()
  {
    p_ = (*run_)[run_->length() - 1];
    i_ = run_->length() - 1;
    is_valid_ = true;
}

  template <typename P>
  inline
  void
  p_run_bkd_piter_<P>::next_()
  {
    mln_assertion(p_[P::dim - 1] - run_->first()[P::dim - 1] == signed(i_));
    p_[dim - 1]--;
    --i_;
    is_valid_ = p_[dim - 1] - run_->first()[dim - 1] >= 0;
  }

  template <typename P>
  inline
  unsigned
  p_run_bkd_piter_<P>::ind() const
  {
    mln_assertion(p_[P::dim - 1] - run_->first()[P::dim - 1] == signed(i_));
    return i_;
  }

  template <typename P>
  inline
  p_run_bkd_piter_<P>::operator P() const
  {
    mln_precondition(is_valid());
    mln_assertion(p_[P::dim - 1] - run_->first()[P::dim - 1] == signed(i_));
    return p_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_P_RUN_PITER_HH
