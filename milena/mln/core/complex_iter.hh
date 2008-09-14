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

# include <mln/core/internal/complex_iter_base.hh>

namespace mln
{

  // Forward declaration.
  template <unsigned D> class complex;

  /*-----------------------.
  | complex_fwd_iter_<D>.  |
  `-----------------------*/

  /// \brief Forward iterator on all the faces of a mln::complex<D>.
  ///
  /// \arg \p D The dimension of the complex this iterator belongs to.
  template <unsigned D>
  class complex_fwd_iter_
    : public internal::complex_iter_base_< any_face_handle<D>,
					   complex_fwd_iter_<D> >
  {
  public:
    typedef any_face_handle<D> face;
    static const unsigned complex_dim = D;

  private:
    typedef complex_fwd_iter_<D> self_;
    typedef internal::complex_iter_base_< face, self_ > super_;

  public:
    using super_::is_valid;
    using super_::invalidate;

  public:
    /// Construction and assignment.
    /// \{
    complex_fwd_iter_();
    // FIXME: See above (internal::complex_iter_base_'s default ctor).
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
    void start();
    /// Go to the next point.
    void next_();
    /// \}

  private:
    using super_::face_;
  };


  /*-----------------------.
  | complex_bkd_iter_<D>.  |
  `-----------------------*/

  /// \brief Backward iterator on all the faces of a mln::complex<D>.
  ///
  /// \arg \p D The dimension of the complex this iterator belongs to.
  template <unsigned D>
  class complex_bkd_iter_
    : public internal::complex_iter_base_< any_face_handle<D>,
					   complex_bkd_iter_<D> >
  {
  public:
    typedef any_face_handle<D> face;
    static const unsigned complex_dim = D;

  private:
    typedef complex_bkd_iter_<D> self_;
    typedef internal::complex_iter_base_< face, self_ > super_;

  public:
    using super_::is_valid;
    using super_::invalidate;

  public:
    /// Construction and assignment.
    /// \{
    complex_bkd_iter_();
    // FIXME: See above (internal::complex_iter_base_'s default ctor).
    complex_bkd_iter_(complex<D>& c);
    complex_bkd_iter_(const self_& rhs);
    self_& operator= (const self_& rhs);
    /// \}

    /// Manipulation.
    /// \{
    /// Change the target complex.
    // FIXME: Same comment as the ctor above.
    void set_cplx(complex<D>& c);

    /// Start an iteration.
    void start();
    /// Go to the next point.
    void next_();
    /// \}

  private:
    using super_::face_;
  };



# ifndef MLN_INCLUDE_ONLY

  /*-----------------------.
  | complex_fwd_iter_<D>.  |
  `-----------------------*/

  template <unsigned D>
  inline
  complex_fwd_iter_<D>::complex_fwd_iter_()
    : super_()
  {
  }

  template <unsigned D>
  inline
  complex_fwd_iter_<D>::complex_fwd_iter_(complex<D>& c)
    : super_(c)
  {
    set_cplx(c);
    mln_postcondition(!is_valid());
  }

  template <unsigned D>
  inline
  complex_fwd_iter_<D>::complex_fwd_iter_(const complex_fwd_iter_<D>& rhs)
    : super_(rhs)
  {
  }

  template <unsigned D>
  inline
  complex_fwd_iter_<D>&
  complex_fwd_iter_<D>::operator=(const complex_fwd_iter_<D>& rhs)
  {
    if (&rhs == this)
      return *this;
    super_::operator=(rhs);
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


  /*-----------------------.
  | complex_bkd_iter_<D>.  |
  `-----------------------*/

  template <unsigned D>
  inline
  complex_bkd_iter_<D>::complex_bkd_iter_()
    : super_()
  {
  }

  template <unsigned D>
  inline
  complex_bkd_iter_<D>::complex_bkd_iter_(complex<D>& c)
    : super_(c)
  {
    set_cplx(c);
    mln_postcondition(!is_valid());
  }

  template <unsigned D>
  inline
  complex_bkd_iter_<D>::complex_bkd_iter_(const complex_bkd_iter_<D>& rhs)
    : super_(rhs)
  {
  }

  template <unsigned D>
  inline
  complex_bkd_iter_<D>&
  complex_bkd_iter_<D>::operator=(const complex_bkd_iter_<D>& rhs)
  {
    if (&rhs == this)
      return *this;
    super_::operator=(rhs);
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

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // ! MLN_CORE_COMPLEX_ITER_HH
