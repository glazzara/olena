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

#ifndef MLN_CORE_COMPLEX_PSITE_HH
# define MLN_CORE_COMPLEX_PSITE_HH

/// \file mln/core/complex_psite.hh
/// \brief Definition of a complex-based point site.

# include <mln/core/concept/point_site.hh>

# include <mln/core/complex.hh>


namespace mln
{
  /* FIXME: Get rid of P?  */

  /// \brief Point site associated to a mln::p_complex.
  ///
  /// \arg \p D The dimension of the complex this psite belongs to.
  /// \arg \p P The type of point associated to this psite.
  template <unsigned D, typename P>
  class complex_psite : public Point_Site< complex_psite<D, P> >
  {
    typedef complex_psite<D, P> self_;
    typedef Point_Site<self_> super_;

  public:
    typedef mln_mesh(P) mesh;
    enum { dim = P::dim };
    typedef P point;
    typedef mln_dpoint(P) dpoint;
    typedef mln_coord(P) coord;

    /// Construction and assignment.
    /// \{
    complex_psite();
    complex_psite(const any_face_handle<D>& face);
    complex_psite(const self_& rhs);
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

    /// Accessors
    /// \{
    /// Return the face handle of this point site.
    any_face_handle<D> face() const;
    /// Return the complex on which this site is built.
    const complex<D>& cplx() const;

    /// Return the dimension of the face of this psite.
    unsigned n() const;
    /// Return the id of the face of this psite.
    unsigned face_id() const;
    /// \}

    /// Is this psite valid?
    bool is_valid() const;

  private:
    /// The handle of the face this psite is pointing towards.
    any_face_handle<D> face_;
    // FIXME: Actually, this is a dummy value!
    point p_;
  };


  /// Comparison of two instances of mln::complex_psite.
  /// \{
  /* FIXME: Shouldn't those comparisons be part of a much general
     mechanism?  */

  /// \brief Is \a lhs equal to \a rhs?
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::complex.
  template <unsigned D, typename P>
  bool
  operator==(const complex_psite<D, P>& lhs,
	     const complex_psite<D, P>& rhs);

  /// \brief Is \a lhs ``less'' than \a rhs?
  ///
  /// This comparison is required by algorithms sorting psites.
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::complex.
  template <unsigned D, typename P>
  bool
  operator< (const complex_psite<D, P>& lhs,
	     const complex_psite<D, P>& rhs);
  /// \}


  /* FIXME: This hand-made delegation is painful.  We should rely on
     the general mechanism provided by Point_Site.  But then again, we
     need to refine/adjust the interface of Point_Site w.r.t. the
     mandatory conversions to points.  */
  template <unsigned D, typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const complex_psite<D, P>& p);



# ifndef MLN_INCLUDE_ONLY

  template <unsigned D, typename P>
  inline
  complex_psite<D, P>::complex_psite()
    : super_(),
      // Dummy initializations.
      face_(), p_()
  {
  }

  template <unsigned D, typename P>
  inline
  complex_psite<D, P>::complex_psite(const any_face_handle<D>& face)
    : super_(),
      face_(face), p_()
  {
  }

  template <unsigned D, typename P>
  inline
  complex_psite<D, P>::complex_psite(const complex_psite<D, P>& rhs)
    : super_(rhs),
      face_(rhs.face_), p_()
  {
  }

  template <unsigned D, typename P>
  inline
  complex_psite<D, P>&
  complex_psite<D, P>::operator= (const complex_psite<D, P>& rhs)
  {
    if (&rhs == this)
      return *this;
    face_ = rhs.face_;
    return *this;
  }

  template <unsigned D, typename P>
  inline
  bool
  complex_psite<D, P>::is_valid() const
  {
    return face_.is_valid();
  }

  template <unsigned D, typename P>
  inline
  const complex_psite<D, P>&
  complex_psite<D, P>::to_psite() const
  {
    return *this;
  }

  template <unsigned D, typename P>
  inline
  const P&
  complex_psite<D, P>::to_point() const
  {
    // FIXME: Dummy value.
    return p_;
  }

  template <unsigned D, typename P>
  inline
  mln_coord(P)
  complex_psite<D, P>::operator[](unsigned i) const
  {
    mln_precondition(is_valid());
    return to_point()[i];
  }

  template <unsigned D, typename P>
  inline
  any_face_handle<D>
  complex_psite<D, P>::face() const
  {
    return face_;
  }

  template <unsigned D, typename P>
  inline
  const complex<D>&
  complex_psite<D, P>::cplx() const
  {
    return face_.cplx();
  }

  template <unsigned D, typename P>
  inline
  unsigned
  complex_psite<D, P>::n() const
  {
    return face_.n();
  }

  template <unsigned D, typename P>
  inline
  unsigned
  complex_psite<D, P>::face_id() const
  {
    return face_.face_id();
  }

  /*--------------.
  | Comparisons.  |
  `--------------*/

  template <unsigned D, typename P>
  bool
  operator==(const complex_psite<D, P>& lhs,
	     const complex_psite<D, P>& rhs)
  {
    mln_precondition(&lhs.cplx() == &rhs.cplx());
    return lhs.face() == rhs.face();
  }

  template <unsigned D, typename P>
  bool
  operator< (const complex_psite<D, P>& lhs,
	     const complex_psite<D, P>& rhs)
  {
    mln_precondition(&lhs.cplx() == &rhs.cplx());
    return lhs.face() < rhs.face();
  }

  /*------------------.
  | Pretty-printing.  |
  `------------------*/

  template <unsigned D, typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const complex_psite<D, P>& p)
  {
    return ostr << "(dim = " << p.n() << ", id = " << p.face_id() << ')';
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of mln

#endif // MLN_CORE_COMPLEX_PSITE_HH
