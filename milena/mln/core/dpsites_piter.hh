// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
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

#ifndef MLN_CORE_DPSITES_PITER_HH
# define MLN_CORE_DPSITES_PITER_HH

/// \file
///
/// Definition of forward and backward mln::dpoint_ based
/// iterators.

# include <vector>
# include <mln/core/internal/site_relative_iterator_base.hh>


namespace mln
{

  /*!
    \internal
    \brief A generic forward iterator on points of windows and of
    neighborhoods.

    The parameter \c V is the type of std::vector enclosing
    structure.
  */
  template <typename V>
  class dpsites_fwd_piter
    : public internal::site_relative_iterator_base< V, dpsites_fwd_piter<V> >
  {
  public:

    /// Constructor.
    ///
    /// \param[in] v   Object that can provide an array of delta-points.
    /// \param[in] c Center point to iterate around.
    template <typename P>
    dpsites_fwd_piter(const V& v, const P& c);

    /// \cond INTERNAL_API

    /// Delayed initialization.
    template <typename P>
    void init_(const V& v, const P& c);
    /// \endcond

    /// Constructor without argument.
    dpsites_fwd_piter();

    /// \cond INTERNAL_API
    /// Test the iterator validity.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void do_start_();

    /// Go to the next point.
    void do_next_();

    /// Compute the current psite.
    mln_psite(V) compute_p_() const;

  protected:

    unsigned i_;

    /// \endcond
  };


  /*!
    \internal
    \brief A generic backward iterator on points of windows and of
    neighborhoods.

    The parameter \c V is the type of std::vector enclosing
    structure.
  */
  template <typename V>
  class dpsites_bkd_piter :
    public internal::site_relative_iterator_base< V, dpsites_bkd_piter<V> >
  {
  public:

    /// Constructor.
    ///
    /// \param[in] v   Object that can provide an array of delta-points.
    /// \param[in] c   Center point to iterate around.
    template <typename P>
    dpsites_bkd_piter(const V& v, const P& c);

    /// Constructor without argument.
    dpsites_bkd_piter();

    /// \cond INTERNAL_API

    /// Delayed initialization.
    template <typename P>
    void init_(const V& v, const P& c);

    /// Test the iterator validity.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void do_start_();

    /// Go to the next point.
    void do_next_();

    /// Compute the current psite.
    mln_psite(V) compute_p_() const;

  protected:

    int i_;

    /// \endcond
  };



# ifndef MLN_INCLUDE_ONLY


  // Forward.

  template <typename V>
  inline
  dpsites_fwd_piter<V>::dpsites_fwd_piter()
  {
  }

  template <typename V>
  template <typename P>
  inline
  dpsites_fwd_piter<V>::dpsites_fwd_piter(const V& v, const P& c)
  {
    init_(v, c);
  }

  template <typename V>
  template <typename P>
  inline
  void
  dpsites_fwd_piter<V>::init_(const V& v, const P& c)
  {
    this->change_target(v);
    this->center_at(c);
  }


  template <typename V>
  inline
  bool
  dpsites_fwd_piter<V>::is_valid_() const
  {
    return i_ != this->s_->std_vector().size();
  }

  template <typename V>
  inline
  void
  dpsites_fwd_piter<V>::invalidate_()
  {
    i_ = this->s_->std_vector().size();
  }

  template <typename V>
  inline
  void
  dpsites_fwd_piter<V>::do_start_()
  {
    i_ = 0;
  }

  template <typename V>
  inline
  void
  dpsites_fwd_piter<V>::do_next_()
  {
    ++i_;
  }

  template <typename V>
  inline
  mln_psite(V)
  dpsites_fwd_piter<V>::compute_p_() const
  {
    return *this->c_ + this->s_->std_vector()[i_];
  }


  // Backward.

  template <typename V>
  inline
  dpsites_bkd_piter<V>::dpsites_bkd_piter()
  {
  }

  template <typename V>
  template <typename P>
  inline
  dpsites_bkd_piter<V>::dpsites_bkd_piter(const V& v, const P& c)
  {
    init_(v, c);
  }

  template <typename V>
  template <typename P>
  inline
  void
  dpsites_bkd_piter<V>::init_(const V& v, const P& c)
  {
    this->change_target(v);
    this->center_at(c);
  }


  template <typename V>
  inline
  bool
  dpsites_bkd_piter<V>::is_valid_() const
  {
    return i_ != -1;
  }

  template <typename V>
  inline
  void
  dpsites_bkd_piter<V>::invalidate_()
  {
    i_ = -1;
  }

  template <typename V>
  inline
  void
  dpsites_bkd_piter<V>::do_start_()
  {
    i_ = this->s_->std_vector().size() - 1;
  }

  template <typename V>
  inline
  void
  dpsites_bkd_piter<V>::do_next_()
  {
    --i_;
  }

  template <typename V>
  inline
  mln_psite(V)
  dpsites_bkd_piter<V>::compute_p_() const
  {
    return *this->c_ + this->s_->std_vector()[i_];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_DPSITES_PITER_HH
