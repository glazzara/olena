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

#ifndef MLN_CORE_FACES_ITER_HH
# define MLN_CORE_FACES_ITER_HH

/// \file mln/core/faces_iter.hh
/// \brief Definition of forward and backward iterators on the n-faces
/// of a complex.

# include <limits>

# include <mln/core/concept/iterator.hh>
# include <mln/core/complex.hh>

namespace mln
{

  // Fwd decls.
  template <unsigned D> class complex;

  /*---------------------------------.
  | internal::faces_iter_<N, D, E>.  |
  `---------------------------------*/

  // FIXME: Maybe we should have a `face' parameter instead of N & D.
  // This way, we could merge this class and
  // internal::complex_iter_x<D, E>.

  namespace internal
  {
    /// \brief Factoring classe for iterators on all the \p N-faces of
    /// a mln::complex<D>.
    ///
    /// \arg \p N The dimension of the face associated to this iterator.
    /// \arg \p D The dimension of the complex this iterator belongs to.
    /// \arg \p E The type exact type of the iterator.
    template <unsigned N, unsigned D, typename E>
    class faces_iter_ : public Iterator<E>
    {
      typedef faces_iter_<N, D, E> self_;

    public:
      typedef face_handle<N, D> face;

      /// Construction and assignment.
      /// \{
      /* FIXME: Keep this non-const?  See a (big) comment about this in
	 milena/tests/complex_image.cc.   */
      faces_iter_(complex<D>& c);
      faces_iter_(const self_& rhs);
      self_& operator= (const self_& rhs);
      /// \}

      /// Manipulation.
      /// \{
      /// Test if the iterator is valid.
      bool is_valid() const;
      /// Invalidate the iterator.
      void invalidate();
      /// \}

      /// Conversion and accessors.
      /// \{
      /// Reference to the corresponding face handle.
      const face& to_face () const;
      /// Convert the iterator into an face handle.
      operator face() const;
      /// \}

    protected:
      /// The face handle this iterator is pointing to.
      face face_;

    private:
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
    template <unsigned N, unsigned D, typename E>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const faces_iter_<N, D, E>& p);

  } // end of mln::internal


  /*------------------------.
  | faces_fwd_iter_<N, D>.  |
  `------------------------*/

  /// \brief Forward iterator on all the faces of a mln::complex<D>.
  ///
  /// \arg \p N The dimension of the face associated to this iterator.
  /// \arg \p D The dimension of the complex this iterator belongs to.
  template <unsigned N, unsigned D>
  class faces_fwd_iter_
    : public internal::faces_iter_< N, D, faces_fwd_iter_<N, D> >
  {
    typedef faces_fwd_iter_<N, D> self_;
    typedef internal::faces_iter_< N, D, self_ > super_;

  public:
    using super_::is_valid;
    using super_::invalidate;

  public:
    /// Construction and assignment.
    /// \{
    // FIXME: See above (internal::faces_iter_'s default ctor).
    faces_fwd_iter_(complex<D>& c);
    faces_fwd_iter_(const self_& rhs);
    self_& operator= (const self_& rhs);
    /// \}

    /// Manipulation.
    /// \{
    /// Test if the iterator is valid.
    void start();
    /// Go to the next point.
    void next_();
    /// \}

  private:
    using super_::face_;
  };


  /*------------------------.
  | faces_bkd_iter_<N, D>.  |
  `------------------------*/

  /// \brief Backward iterator on all the faces of a mln::complex<D>.
  ///
  /// \arg \p N The dimension of the face associated to this iterator.
  /// \arg \p D The dimension of the complex this iterator belongs to.
  template <unsigned N, unsigned D>
  class faces_bkd_iter_
    : public internal::faces_iter_< N, D, faces_bkd_iter_<N, D> >
  {
    typedef faces_bkd_iter_<N, D> self_;
    typedef internal::faces_iter_< N, D, self_ > super_;

  public:
    using super_::is_valid;
    using super_::invalidate;

  public:
    /// Construction and assignment.
    /// \{
    // FIXME: See above (internal::faces_iter_'s default ctor).
    faces_bkd_iter_(complex<D>& c);
    faces_bkd_iter_(const self_& rhs);
    self_& operator= (const self_& rhs);
    /// \}

    /// Manipulation.
    /// \{
    /// Start an iteration.
    void start();
    /// Go to the next point.
    void next_();
    /// \}

  private:
    using super_::face_;
  };



# ifndef MLN_INCLUDE_ONLY

  /*---------------------------------.
  | internal::faces_iter_<N, D, E>.  |
  `---------------------------------*/

  namespace internal
  {

    template <unsigned N, unsigned D, typename E>
    inline
    faces_iter_<N, D, E>::faces_iter_(complex<D>& c)
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();

      face_.set_cplx(c);
      // Invalidate face_.
      invalidate();
    }

    template <unsigned N, unsigned D, typename E>
    inline
    faces_iter_<N, D, E>::faces_iter_(const faces_iter_<N, D, E>& rhs)
      : face_(rhs.face_)
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();
    }

    template <unsigned N, unsigned D, typename E>
    inline
    faces_iter_<N, D, E>&
    faces_iter_<N, D, E>::operator=(const faces_iter_<N, D, E>& rhs)
    {
      if (&rhs == this)
	return *this;
      face_ = rhs.face_;
      return *this;
    }

    template <unsigned N, unsigned D, typename E>
    inline
    bool
    faces_iter_<N, D, E>::is_valid() const
    {
      return face_.is_valid();
    }

    template <unsigned N, unsigned D, typename E>
    inline
    void
    faces_iter_<N, D, E>::invalidate()
    {
      face_.set_face_id(invalid_unsigned_());
    }

    template <unsigned N, unsigned D, typename E>
    inline
    const face_handle<N, D>&
    faces_iter_<N, D, E>::to_face() const
    {
      return face_;
    }

    template <unsigned N, unsigned D, typename E>
    inline
    faces_iter_<N, D, E>::operator face_handle<N, D>() const
    {
      mln_precondition(is_valid());
      return face_;
    }

    template <unsigned N, unsigned D, typename E>
    unsigned
    faces_iter_<N, D, E>::invalid_unsigned_() const
    {
      return std::numeric_limits<unsigned>::max();
    }

    template <unsigned N, unsigned D, typename E>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const faces_iter_<N, D, E>& p)
    {
    /* FIXME: We should use p.to_face() here, but as it lacks the
       precondition the conversion operator has, so we use the latter.

       We should
       - rename `to_face' as `to_face_';
       - write a new `to_face' routine checking the validity of the
         iterator;
       - have the conversion operator to face use this new `to_face'
         routine;
       - adjust former clients of `to_face'

       This is a general remark that applies to all iterators of
       Milena.  */
      typename faces_iter_<N, D, E>::face f = p;
      return ostr << "(dim = " << N << ", id = " << f.face_id() << ')';
    }

  } // end of mln::internal


  /*------------------------.
  | faces_fwd_iter_<N, D>.  |
  `------------------------*/

  template <unsigned N, unsigned D>
  inline
  faces_fwd_iter_<N, D>::faces_fwd_iter_(complex<D>& c)
    : super_(c)
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();
  }

  template <unsigned N, unsigned D>
  inline
  faces_fwd_iter_<N, D>::faces_fwd_iter_(const faces_fwd_iter_<N, D>& rhs)
    : super_(rhs)
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();
  }

  template <unsigned N, unsigned D>
  inline
  faces_fwd_iter_<N, D>&
  faces_fwd_iter_<N, D>::operator=(const faces_fwd_iter_<N, D>& rhs)
  {
    if (&rhs == this)
      return *this;
    super_::operator=(rhs);
    return *this;
  }

  template <unsigned N, unsigned D>
  inline
  void 
  faces_fwd_iter_<N, D>::start()
  {
    face_.set_face_id(0u);
  }

  template <unsigned N, unsigned D>
  inline
  void 
  faces_fwd_iter_<N, D>::next_()
  {
    if (is_valid())
      {
	unsigned face_id = face_.face_id();
	if (face_id + 1 < face_.cplx().template nfaces<N>())
	  /* FIXME: Provide accessor face_handle::n() returning
	     a mutable reference?  This way, we could just write
	 
	       ++face_.face_id();
	     
	     instead of the following.  */
	  face_.set_face_id(face_id + 1);
	else
	  invalidate();
      }
  }


  /*------------------------.
  | faces_bkd_iter_<N, D>.  |
  `------------------------*/

  // FIXME: Resume here.

  template <unsigned N, unsigned D>
  inline
  faces_bkd_iter_<N, D>::faces_bkd_iter_(complex<D>& c)
    : super_(c)
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();
  }

  template <unsigned N, unsigned D>
  inline
  faces_bkd_iter_<N, D>::faces_bkd_iter_(const faces_bkd_iter_<N, D>& rhs)
    : super_(rhs)
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();
  }

  template <unsigned N, unsigned D>
  inline
  faces_bkd_iter_<N, D>&
  faces_bkd_iter_<N, D>::operator=(const faces_bkd_iter_<N, D>& rhs)
  {
    if (&rhs == this)
      return *this;
    super_::operator=(rhs);
    return *this;
  }

  template <unsigned N, unsigned D>
  inline
  void 
  faces_bkd_iter_<N, D>::start()
  {
    face_.set_face_id(face_.cplx().template nfaces<N>() - 1);
  }

  template <unsigned N, unsigned D>
  inline
  void 
  faces_bkd_iter_<N, D>::next_()
  {
    if (is_valid())
      {
	unsigned face_id = face_.face_id();
	if (face_id > 0)
	  /* FIXME: Provide accessor face_handle::n() returning
	     a mutable reference?  This way, we could just write
	 
	       ++face_.face_id();
	     
	     instead of the following.  */
	  face_.set_face_id(face_id - 1);
	else
	  invalidate();
      }
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // ! MLN_CORE_FACES_ITER_HH
