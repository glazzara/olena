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

#ifndef MLN_CORE_FACES_PSITE_HH
# define MLN_CORE_FACES_PSITE_HH

/// \file mln/core/faces_psite.hh
/// \brief Definition of a point site based on the n-faces of a
/// complex.

# include <mln/core/concept/point_site.hh>

# include <mln/core/complex.hh>


namespace mln
{
  /* FIXME: Currently, P and N are free variables; we might want to
     relate them, e.g., have P::dim == N.  Or even get rid of P.  */

  /// \brief Point site associated to a mln::p_faces.
  ///
  /// \arg \p N The dimension of the face associated to this psite.
  /// \arg \p D The dimension of the complex this psite belongs to.
  /// \arg \p P The type of point associated to this psite.
  template <unsigned N, unsigned D, typename P>
  class faces_psite : public Point_Site< faces_psite<N, D, P> >
  {
    typedef faces_psite<N, D, P> self_;
    typedef Point_Site<self_> super_;

  public:
    typedef mln_mesh(P) mesh;
    enum { dim = P::dim };
    typedef P point;
    typedef mln_dpoint(P) dpoint;
    typedef mln_coord(P) coord;

    /// Construction and assignment.
    /// \{
    faces_psite();
    faces_psite(const face_handle<N, D>& face);
    faces_psite(const self_& rhs);
    self_& operator= (const self_& rhs);
    /// \}

    /// Access to psite.
    const self_& to_psite() const;

    /* FIXME: Should be removed as soon as ``point sets'' become
       ``site sets''.  */
    /// Access to point.
    /// \{
    const point& to_point() const;
    coord operator[](unsigned face) const;
    /// \}

    /// Return the face handle of this point site.
    face_handle<N, D> face() const;
    /// Return the complex on which this site is built.
    const complex<D>& cplx() const;

    /// Is this psite valid?
    bool is_valid() const;

  private:
    /// The handle of the face this psite is pointing towards.
    face_handle<N, D> face_;
    // FIXME: Actually, this is a dummy value!
    point p_;
  };


  /// Comparison of two instances of mln::faces_psite.
  /// \{
  /* FIXME: Shouldn't those comparisons be part of a much general
     mechanism?  */

  /// \brief Is \a lhs equal to \a rhs?
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::complex.
  template <unsigned N, unsigned D, typename P>
  bool
  operator==(const faces_psite<N, D, P>& lhs,
	     const faces_psite<N, D, P>& rhs);

  /// \brief Is \a lhs ``less'' than \a rhs?
  ///
  /// This comparison is required by algorithms sorting psites.
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::complex.
  template <unsigned N, unsigned D, typename P>
  bool
  operator< (const faces_psite<N, D, P>& lhs,
	     const faces_psite<N, D, P>& rhs);
  /// \}



# ifndef MLN_INCLUDE_ONLY

  template <unsigned N, unsigned D, typename P>
  inline
  faces_psite<N, D, P>::faces_psite()
    : super_(),
      // Dummy initializations.
      face_(), p_()
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  faces_psite<N, D, P>::faces_psite(const face_handle<N, D>& face)
    : super_(),
      face_(face), p_()
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  faces_psite<N, D, P>::faces_psite(const faces_psite<N, D, P>& rhs)
    : super_(rhs),
      face_(rhs.face_), p_()
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  faces_psite<N, D, P>&
  faces_psite<N, D, P>::operator= (const faces_psite<N, D, P>& rhs)
  {
    if (&rhs == this)
      return *this;
    face_ = rhs.face_;
    return *this;
  }

  template <unsigned N, unsigned D, typename P>
  inline
  bool
  faces_psite<N, D, P>::is_valid() const
  {
    return face_.is_valid();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  const faces_psite<N, D, P>&
  faces_psite<N, D, P>::to_psite() const
  {
    return *this;
  }

  template <unsigned N, unsigned D, typename P>
  inline
  const P&
  faces_psite<N, D, P>::to_point() const
  {
    // FIXME: Dummy value.
    return p_;
  }

  template <unsigned N, unsigned D, typename P>
  inline
  mln_coord(P)
  faces_psite<N, D, P>::operator[](unsigned i) const
  {
    mln_precondition(is_valid());
    return to_point()[i];
  }

  template <unsigned N, unsigned D, typename P>
  inline
  face_handle<N, D>
  faces_psite<N, D, P>::face() const
  {
    return face_;
  }

  template <unsigned N, unsigned D, typename P>
  inline
  const complex<D>&
  faces_psite<N, D, P>::cplx() const
  {
    return face_.cplx();
  }

  /*--------------.
  | Comparisons.  |
  `--------------*/

  template <unsigned N, unsigned D, typename P>
  bool
  operator==(const faces_psite<N, D, P>& lhs,
	     const faces_psite<N, D, P>& rhs)
  {
    mln_precondition(&lhs.cplx() == &rhs.cplx());
    return lhs.face() == rhs.face();
  }

  template <unsigned N, unsigned D, typename P>
  bool
  operator< (const faces_psite<N, D, P>& lhs,
	     const faces_psite<N, D, P>& rhs)
  {
    mln_precondition(&lhs.cplx() == &rhs.cplx());
    return lhs.face() < rhs.face();
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of mln

#endif // MLN_CORE_FACES_PSITE_HH
