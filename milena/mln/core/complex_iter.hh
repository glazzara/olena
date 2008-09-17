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

#ifndef MLN_CORE_COMPLEX_ITER_HH
# define MLN_CORE_COMPLEX_ITER_HH

/// \file mln/core/complex_iter.hh
/// \brief Definition of forward and backward iterators on complexes.


# include <limits>

# include <mln/core/concept/iterator.hh>
# include <mln/core/complex.hh>


// Factor complex_fwd_iter_<D> and complex_bkd_iter_<D>?

namespace mln
{

  // Fwd decls.
  template <unsigned D> class complex;


  /*-----------------------.
  | complex_fwd_iter_<D>.  |
  `-----------------------*/

  /// \brief Forward iterator on all the faces of mln::complex<D>.
  template <unsigned D>
  class complex_fwd_iter_ : public Iterator< complex_fwd_iter_<D> >
  {
    typedef complex_fwd_iter_<D> self_;

  public:
    typedef any_face_handle<D> face;

    /// Construction and assignment.
    /// \{
    complex_fwd_iter_();
    /* FIXME: Keep this non-const?  See a (big) comment about this in
       milena/tests/complex_image.cc.   */
    complex_fwd_iter_(complex<D>& c);
    complex_fwd_iter_(const self_& rhs);
    self_& operator= (const self_& rhs);
    /// \}

    /// Manipulation.
    /// \{
    /// Change the target complex.
    // FIXME: Same comment as the ctor above.
    void set_cplx(complex<D>& c);

    /// Test if the iterator is valid.
    bool is_valid() const;
    /// Invalidate the iterator.
    void invalidate();
    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();
    /// \}

    /// Conversion and accessors.
    /// \{
    /// Reference to the corresponding any-face handle.
    const face& to_face () const;
    /// Convert the iterator into an any-face handle.
    operator face() const;
    /// \}

  private:
    /// The face handle this iterator is pointing to.
    face face_;

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
  template <unsigned D>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const complex_fwd_iter_<D>& p);


  /*-----------------------.
  | complex_bkd_iter_<D>.  |
  `-----------------------*/

  /// \brief Backward iterator on all the faces of mln::complex<D>.
  template <unsigned D>
  class complex_bkd_iter_ : public Iterator< complex_bkd_iter_<D> >
  {
    typedef complex_bkd_iter_<D> self_;

  public:
    typedef any_face_handle<D> face;

    /// Construction and assignment.
    /// \{
    complex_bkd_iter_();
    /* FIXME: Keep this non-const?  See a (big) comment about this in
       milena/tests/complex_image.cc.   */
    complex_bkd_iter_(complex<D>& c);
    complex_bkd_iter_(const self_& rhs);
    self_& operator= (const self_& rhs);
    /// \}

    /// Manipulation.
    /// \{
    /// Change the target complex.
    // FIXME: Same comment as the ctor above.
    void set_cplx(complex<D>& c);

    /// Test if the iterator is valid.
    bool is_valid() const;
    /// Invalidate the iterator.
    void invalidate();
    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();
    /// \}

    /// Conversion and accessors.
    /// \{
    /// Reference to the corresponding any-face handle.
    const face& to_face () const;
    /// Convert the iterator into an any-face handle.
    operator face() const;
    /// \}

  private:
    /// The face handle this iterator is pointing to.
    face face_;

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
  template <unsigned D>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const complex_bkd_iter_<D>& p);



# ifndef MLN_INCLUDE_ONLY

  /*-----------------------.
  | complex_fwd_iter_<D>.  |
  `-----------------------*/

  template <unsigned D>
  inline
  complex_fwd_iter_<D>::complex_fwd_iter_()
  {
    invalidate();
  }

  template <unsigned D>
  inline
  complex_fwd_iter_<D>::complex_fwd_iter_(complex<D>& c)
  {
    set_cplx(c);
    mln_postcondition(!is_valid());
  }

  template <unsigned D>
  inline
  complex_fwd_iter_<D>::complex_fwd_iter_(const complex_fwd_iter_<D>& rhs)
    : face_(rhs.face_)
  {
  }

  template <unsigned D>
  inline
  complex_fwd_iter_<D>&
  complex_fwd_iter_<D>::operator=(const complex_fwd_iter_<D>& rhs)
  {
    if (&rhs == this)
      return *this;
    face_ = rhs.face_;
    return *this;
  }

  template <unsigned D>
  inline
  void
  complex_fwd_iter_<D>::set_cplx(complex<D>& c)
  {
    face_.set_cplx(c);
    // Invalidate face_.
    invalidate();
  }

  template <unsigned D>
  inline
  bool
  complex_fwd_iter_<D>::is_valid() const
  {
    return face_.is_valid();
  }

  template <unsigned D>
  inline
  void
  complex_fwd_iter_<D>::invalidate()
  {
    face_.set_n(invalid_unsigned_());
    face_.set_face_id(invalid_unsigned_());
  }

  template <unsigned D>
  inline
  void 
  complex_fwd_iter_<D>::start()
  {
    face_.set_n(0u);
    face_.set_face_id(0u);
  }

  template <unsigned D>
  inline
  void 
  complex_fwd_iter_<D>::next_()
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
  }

  template <unsigned D>
  inline
  const any_face_handle<D>&
  complex_fwd_iter_<D>::to_face() const
  {
    return face_;
  }

  template <unsigned D>
  inline
  complex_fwd_iter_<D>::operator any_face_handle<D>() const
  {
    mln_precondition(is_valid());
    return face_;
  }

  template <unsigned D>
  unsigned
  complex_fwd_iter_<D>::invalid_unsigned_() const
  {
    return std::numeric_limits<unsigned>::max();
  }


  template <unsigned D>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const complex_fwd_iter_<D>& p)
  {
    return ostr << "(dim = " << p.to_face().n()
		<< ", id = " << p.to_face().face_id() << ')';
  }


  /*-----------------------.
  | complex_bkd_iter_<D>.  |
  `-----------------------*/

  template <unsigned D>
  inline
  complex_bkd_iter_<D>::complex_bkd_iter_()
  {
    invalidate();
  }

  template <unsigned D>
  inline
  complex_bkd_iter_<D>::complex_bkd_iter_(complex<D>& c)
  {
    set_cplx(c);
    mln_postcondition(!is_valid());
  }

  template <unsigned D>
  inline
  complex_bkd_iter_<D>::complex_bkd_iter_(const complex_bkd_iter_<D>& rhs)
    : face_(rhs.face_)
  {
  }

  template <unsigned D>
  inline
  complex_bkd_iter_<D>&
  complex_bkd_iter_<D>::operator=(const complex_bkd_iter_<D>& rhs)
  {
    if (&rhs == this)
      return *this;
    face_ = rhs.face_;
    return *this;
  }

  template <unsigned D>
  inline
  void
  complex_bkd_iter_<D>::set_cplx(complex<D>& c)
  {
    face_.set_cplx(c);
    // Invalidate face_.
    invalidate();
  }

  template <unsigned D>
  inline
  bool
  complex_bkd_iter_<D>::is_valid() const
  {
    return face_.is_valid();
  }

  template <unsigned D>
  inline
  void
  complex_bkd_iter_<D>::invalidate()
  {
    face_.set_n(invalid_unsigned_());
    face_.set_face_id(invalid_unsigned_());
  }

  template <unsigned D>
  inline
  void 
  complex_bkd_iter_<D>::start()
  {
    face_.set_n(D);
    face_.set_face_id(face_.cplx().template nfaces<D>() - 1);
  }

  template <unsigned D>
  inline
  void 
  complex_bkd_iter_<D>::next_()
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
  }

  template <unsigned D>
  inline
  const any_face_handle<D>&
  complex_bkd_iter_<D>::to_face() const
  {
    return face_;
  }

  template <unsigned D>
  inline
  complex_bkd_iter_<D>::operator any_face_handle<D>() const
  {
    mln_precondition(is_valid());
    return face_;
  }

  template <unsigned D>
  unsigned
  complex_bkd_iter_<D>::invalid_unsigned_() const
  {
    return std::numeric_limits<unsigned>::max();
  }


  template <unsigned D>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const complex_bkd_iter_<D>& p)
  {
    return ostr << "(dim = " << p.to_face().n()
		<< ", id = " << p.to_face().face_id() << ')';
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // ! MLN_CORE_COMPLEX_ITER_HH
