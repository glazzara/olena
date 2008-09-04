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

#ifndef MLN_CORE_SITE_SET_P_COMPLEX_PITER_HH
# define MLN_CORE_SITE_SET_P_COMPLEX_PITER_HH

# include <limits>

# include <mln/core/internal/point_iterator_base.hh>
# include <mln/core/site_set/p_complex.hh>
# include <mln/core/complex_psite.hh>

/// \file mln/core/site_set/p_complex_piter.hh
/// \brief Definition of point iterator on complex-based pset.

// Factor p_complex_fwd_piter_<D, P> and p_complex_bkd_piter_<D, P>

namespace mln
{
  /* FIXME: Get rid of P?  */

  // Fwd decls.
  template <unsigned D, typename P> class p_complex;
  template <unsigned D, typename P> class complex_psite;


  /*-----------------------------.
  | p_complex_fwd_piter_<D, P>.  |
  `-----------------------------*/

  /// \brief Forward iterator on point sites of a mln::p_complex<D, P>.
  template <unsigned D, typename P>
  class p_complex_fwd_piter_
    : public internal::point_iterator_base_< P, p_complex_fwd_piter_<D, P> >
  {
    typedef p_complex_fwd_piter_<D, P> self_;
    typedef internal::point_iterator_base_< P, self_ > super_;

  public:
    // Make definitions from super class available.
    // FIXME: Is it still meaningful for a complex?
    enum { dim = super_::dim };

    typedef complex_psite<D, P> psite;
    typedef P point;
    typedef mln_coord(point) coord;

    /// Construction and assignment.
    /// \{
    p_complex_fwd_piter_(const p_complex<D, P>& pc);
    p_complex_fwd_piter_(const self_& rhs);
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
    /// The face handle this iterator is pointing to.
    any_face_handle<D> face_;
    /// The psite corresponding to this iterator.
    psite psite_;
    /// \brief The point associated to this psite.
    // FIXME: Actually, this is a dummy value!
    point p_;

    /// \brief An invalid value for both the dimension and the id of
    /// the face.
    ///
    /// Use a function instead of a static constant, since `static'
    /// variables needs to be compiled once, which requires a compiled
    /// library to avoid duplicate symbols, which is something that
    /// was not really planned in Milena.  A function tagged `inlined'
    /// can appear multiple times in a program, and solves this
    /// problem.  We rely on the compiler to inline this call.
    ///
    /// Of course, we could have used UINT_MAX, but it is not very
    /// C++.
    unsigned invalid_unsigned_() const;
  };


  /* FIXME: This hand-made delegation is painful.  We should rely on
     the general mechanism provided by Point_Site.  But then again, we
     need to refine/adjust the interface of Point_Site w.r.t. the
     mandatory conversions to points.  */
  template <unsigned D, typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const p_complex_fwd_piter_<D, P>& p);


  /*-----------------------------.
  | p_complex_bkd_piter_<D, P>.  |
  `-----------------------------*/

  /// \brief Backward iterator on point sites of a mln::p_complex<D, P>.
  template <unsigned D, typename P>
  class p_complex_bkd_piter_
    : public internal::point_iterator_base_< P, p_complex_bkd_piter_<D, P> >
  {
    typedef p_complex_bkd_piter_<D, P> self_;
    typedef internal::point_iterator_base_< P, self_ > super_;

  public:
    // Make definitions from super class available.
    // FIXME: Is it still meaningful for a complex?
    enum { dim = super_::dim };

    typedef complex_psite<D, P> psite;
    typedef P point;
    typedef mln_coord(point) coord;

    /// Construction and assignment.
    /// \{
    p_complex_bkd_piter_(const p_complex<D, P>& pc);
    p_complex_bkd_piter_(const self_& rhs);
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
    /// The face handle this iterator is pointing to.
    any_face_handle<D> face_;
    /// The psite corresponding to this iterator.
    psite psite_;
    /// \brief The point associated to this psite.
    // FIXME: Actually, this is a dummy value!
    point p_;

    /// \brief An invalid value for both the dimension and the id of
    /// the face.
    ///
    /// Use a function instead of a static constant, since `static'
    /// variables needs to be compiled once, which requires a compiled
    /// library to avoid duplicate symbols, which is something that
    /// was not really planned in Milena.  A function tagged `inlined'
    /// can appear multiple times in a program, and solves this
    /// problem.  We rely on the compiler to inline this call.
    ///
    /// Of course, we could have used UINT_MAX, but it is not very
    /// C++.
    unsigned invalid_unsigned_() const;
  };


  /* FIXME: This hand-made delegation is painful.  We should rely on
     the general mechanism provided by Point_Site.  But then again, we
     need to refine/adjust the interface of Point_Site w.r.t. the
     mandatory conversions to points.  */
  template <unsigned D, typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const p_complex_bkd_piter_<D, P>& p);



# ifndef MLN_INCLUDE_ONLY

  /*-----------------------------.
  | p_complex_fwd_piter_<D, P>.  |
  `-----------------------------*/

  template <unsigned D, typename P>
  inline
  p_complex_fwd_piter_<D, P>::p_complex_fwd_piter_(const p_complex<D, P>& pc)
    // Initialize psite_ and p_ a dummy values.
    : psite_(),
      p_()
  {
    face_.set_cplx(pc.cplx());
    // Invalidate face_.
    invalidate();
  }

  template <unsigned D, typename P>
  inline
  p_complex_fwd_piter_<D, P>::p_complex_fwd_piter_(const p_complex_fwd_piter_<D, P>& rhs)
    : face_(rhs.face_),
      psite_(rhs.psite_),
      // Dummy value.
      p_()
  {
  }

  template <unsigned D, typename P>
  inline
  p_complex_fwd_piter_<D, P>&
  p_complex_fwd_piter_<D, P>::operator=(const p_complex_fwd_piter_<D, P>& rhs)
  {
    if (&rhs == this)
      return *this;
    face_ = rhs.face_;
    psite_ = rhs.psite_;
    return *this;
  }

  template <unsigned D, typename P>
  inline
  mln_coord(P)
  p_complex_fwd_piter_<D, P>::operator[](unsigned i) const
  {
    // Dummy value.
    return p_[i];
  }

  template <unsigned D, typename P>
  inline
  bool
  p_complex_fwd_piter_<D, P>::is_valid() const
  {
    return face_.is_valid();
  }

  template <unsigned D, typename P>
  inline
  void
  p_complex_fwd_piter_<D, P>::invalidate()
  {
    face_.set_n(invalid_unsigned_());
    face_.set_face_id(invalid_unsigned_());
  }

  template <unsigned D, typename P>
  inline
  void 
  p_complex_fwd_piter_<D, P>::start()
  {
    face_.set_n(0u);
    face_.set_face_id(0u);
    update_();
  }

  template <unsigned D, typename P>
  inline
  void 
  p_complex_fwd_piter_<D, P>::next_()
  {
    if (is_valid())
      {
	unsigned n = face_.n();
	unsigned face_id = face_.face_id();

	if (face_id + 1 < face_.cplx().nfaces(n))
	  /* FIXME: Provide accessor any_face_handle::n() returning
	     a mutable reference?  This way, we could just write
	 
	       ++face_.face_id();
	     
	     instead of the following.  */
	  face_.set_face_id(face_id + 1);
	else
	  // Start to iterate on the faces of the next dimension if
	  // possible.
	  if (n <= D)
	    {
	      // FIXME: Same remark as above.
	      face_.set_n(n + 1);
	      face_.set_face_id(0u);
	    }
	  else
	    invalidate();
      }
    if (is_valid())
      update_();
  }

  template <unsigned D, typename P>
  inline
  void
  p_complex_fwd_piter_<D, P>::update_()
  {
    // Update psite_.
    psite_ = complex_psite<D, P>(face_);
  }

  template <unsigned D, typename P>
  inline
  const P&
  p_complex_fwd_piter_<D, P>::to_point() const
  {
    // Dummy value.
    return p_;
  }

  template <unsigned D, typename P>
  inline
  const complex_psite<D, P>&
  p_complex_fwd_piter_<D, P>::to_psite() const
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

  template <unsigned D, typename P>
  inline
  p_complex_fwd_piter_<D, P>::operator complex_psite<D, P>() const
  {
    mln_precondition(is_valid());
    return psite_;
  }

  template <unsigned D, typename P>
  unsigned
  p_complex_fwd_piter_<D, P>::invalid_unsigned_() const
  {
    return std::numeric_limits<unsigned>::max();
  }


  template <unsigned D, typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const p_complex_fwd_piter_<D, P>& p)
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
    return ostr << static_cast< complex_psite<D, P> >(p);
  }


  /*-----------------------------.
  | p_complex_bkd_piter_<D, P>.  |
  `-----------------------------*/

  template <unsigned D, typename P>
  inline
  p_complex_bkd_piter_<D, P>::p_complex_bkd_piter_(const p_complex<D, P>& pc)
    // Initialize psite_ and p_ a dummy values.
    : psite_(),
      p_()
  {
    face_.set_cplx(pc.cplx());
    // Invalidate face_.
    invalidate();
  }

  template <unsigned D, typename P>
  inline
  p_complex_bkd_piter_<D, P>::p_complex_bkd_piter_(const p_complex_bkd_piter_<D, P>& rhs)
    : face_(rhs.face_),
      psite_(rhs.psite_),
      // Dummy value.
      p_()
  {
  }

  template <unsigned D, typename P>
  inline
  p_complex_bkd_piter_<D, P>&
  p_complex_bkd_piter_<D, P>::operator=(const p_complex_bkd_piter_<D, P>& rhs)
  {
    if (&rhs == this)
      return *this;
    face_ = rhs.face_;
    psite_ = rhs.psite_;
    return *this;
  }

  template <unsigned D, typename P>
  inline
  mln_coord(P)
  p_complex_bkd_piter_<D, P>::operator[](unsigned i) const
  {
    // Dummy value.
    return p_[i];
  }

  template <unsigned D, typename P>
  inline
  bool
  p_complex_bkd_piter_<D, P>::is_valid() const
  {
    return face_.is_valid();
  }

  template <unsigned D, typename P>
  inline
  void
  p_complex_bkd_piter_<D, P>::invalidate()
  {
    face_.set_n(invalid_unsigned_());
    face_.set_face_id(invalid_unsigned_());
  }

  template <unsigned D, typename P>
  inline
  void 
  p_complex_bkd_piter_<D, P>::start()
  {
    face_.set_n(D);
    face_.set_face_id(face_.cplx().template nfaces<D>() - 1);
    update_();
  }

  template <unsigned D, typename P>
  inline
  void 
  p_complex_bkd_piter_<D, P>::next_()
  {
    if (is_valid())
      {
	unsigned n = face_.n();
	unsigned face_id = face_.face_id();

	if (face_id > 0)
	  /* FIXME: Provide accessor any_face_handle::n() returning
	     a mutable reference?  This way, we could just write
	 
	       ++face_.face_id();
	     
	     instead of the following.  */
	  face_.set_face_id(face_id - 1);
	else
	  // Start to iterate on the faces of the previous dimension
	  // if it exists.
	  if (n > 0)
	    {
	      // FIXME: Same remark as above.
	      face_.set_n(n - 1);
	      face_.set_face_id(face_.cplx().nfaces(n - 1) - 1);
	    }
	  else
	    invalidate();
      }
    if (is_valid())
      update_();
  }

  template <unsigned D, typename P>
  inline
  void
  p_complex_bkd_piter_<D, P>::update_()
  {
    // Update psite_.
    psite_ = complex_psite<D, P>(face_);
  }

  template <unsigned D, typename P>
  inline
  const P&
  p_complex_bkd_piter_<D, P>::to_point() const
  {
    // Dummy value.
    return p_;
  }

  template <unsigned D, typename P>
  inline
  const complex_psite<D, P>&
  p_complex_bkd_piter_<D, P>::to_psite() const
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

  template <unsigned D, typename P>
  inline
  p_complex_bkd_piter_<D, P>::operator complex_psite<D, P>() const
  {
    mln_precondition(is_valid());
    return psite_;
  }

  template <unsigned D, typename P>
  unsigned
  p_complex_bkd_piter_<D, P>::invalid_unsigned_() const
  {
    return std::numeric_limits<unsigned>::max();
  }


  template <unsigned D, typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const p_complex_bkd_piter_<D, P>& p)
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
    return ostr << static_cast< complex_psite<D, P> >(p);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // ! MLN_CORE_SITE_SET_P_COMPLEX_PITER_HH
