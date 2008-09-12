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

# include <mln/core/internal/complex_iter_base.hh>

namespace mln
{

  /*------------------------.
  | faces_fwd_iter_<N, D>.  |
  `------------------------*/

  /// \brief Forward iterator on all the faces of a mln::complex<D>.
  ///
  /// \arg \p N The dimension of the face associated to this iterator.
  /// \arg \p D The dimension of the complex this iterator belongs to.
  template <unsigned N, unsigned D>
  class faces_fwd_iter_
    : public internal::complex_iter_base_< face_handle<N, D>,
					   faces_fwd_iter_<N, D> >
  {
  public:
    typedef face_handle<N, D> face;

  private:
    typedef faces_fwd_iter_<N, D> self_;
    typedef internal::complex_iter_base_< face, self_ > super_;

  public:
    using super_::is_valid;
    using super_::invalidate;

  public:
    /// Construction and assignment.
    /// \{
    // FIXME: See above (internal::complex_iter_base_'s default ctor).
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
    : public internal::complex_iter_base_< face_handle<N, D>,
					   faces_bkd_iter_<N, D> >
  {
  public:
    typedef face_handle<N, D> face;

  private:
    typedef faces_bkd_iter_<N, D> self_;
    typedef internal::complex_iter_base_< face, self_ > super_;

  public:
    using super_::is_valid;
    using super_::invalidate;

  public:
    /// Construction and assignment.
    /// \{
    // FIXME: See above (internal::complex_iter_base_'s default ctor).
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
