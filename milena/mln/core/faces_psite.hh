// Copyright (C) 2008, 2009, 2011, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_FACES_PSITE_HH
# define MLN_CORE_FACES_PSITE_HH

/// \file
/// \brief Definition of a point site based on the n-faces of a
/// complex.

# include <cstdlib>

# include <mln/core/internal/pseudo_site_base.hh>

# include <mln/topo/complex.hh>

// FIXME: Factor complex_psite and faces_psite?

// FIXME: Rename faces_psite as p_faces_psite, and move this file to
// core/site_set.

namespace mln
{
  // Forward declaration.
  template <unsigned N, unsigned D, typename P> struct p_faces;


  /// \brief Point site associated to a mln::p_faces.
  ///
  /// \tparam N The dimension of the face associated to this psite.
  /// \tparam D The dimension of the complex this psite belongs to.
  /// \tparam P The type of point associated to this psite.
  template <unsigned N, unsigned D, typename P>
  class faces_psite
    : public internal::pseudo_site_base_< const P&,
					  faces_psite<N, D, P> >
  {
  public:
    // This associated type is important to know that this particular
    // pseudo site knows the site set it refers to.
    typedef p_faces<N, D, P> target;

    /// Construction and assignment.
    /// \{
    faces_psite();
    /// \pre pf.cplx() == face.cplx().
    faces_psite(const p_faces<N, D, P>& pf, const topo::n_face<N, D>& face);
    faces_psite(const p_faces<N, D, P>& pf, unsigned face_id);
    /// \}

    /// Psite manipulators.
    /// \{
    /// Is this psite valid?
    bool is_valid() const;
    /// Invalidate this psite.
    void invalidate();
    /// \}

    /// Site set manipulators.
    /// \{
    /// \brief Return the p_faces this site is built on.
    /// (shortcut for *target()).
    /// \pre Member face_ is valid.
    const target& site_set() const;

    /// \cond INTERNAL_API
    /// Get a pointer to the target site_set.
    const target* target_() const;
    /// \endcond

    /// Set the target site_set.
    void change_target(const target& new_target);
    /// \}

    /// Proxy manipulators.
    /// \{
    /// \cond INTERNAL_API
    /// Return the site corresponding to this psite.
    const P& subj_();
    /// \endcond
    /// \}

    /// Face handle manipulators.
    /// \{
    /// Return the face handle of this point site.
    topo::n_face<N, D> face() const;

    /// Return the dimension of the face of this psite.
    unsigned n() const;
    /// Return the id of the face of this psite.
    unsigned face_id() const;
    /// \}

  private:
    /// Site-related members.
    /// \{
    /// Update the site corresponding to this psite.
    void update_();
    // The site corresponding to this psite.
    P p_;
    /// \}

    /* FIXME: Attributes pf_ and face_ share a common information: the
       address of their complex.

       This is both a loss of space and time (we must ensure
       synchronization), but this design issue is not trivial: we
       actually introduced the face handles to pack together the
       location information (face_id) with the support (the complex),
       to avoid what we did with graphs --- where location (edge id or
       vertex id) is separated from the support (the graph).

       Think about it, and adjust complex_psite as well.  */
  private:
    /// Complex-related members.
    /// \{
    /// The mln::p_faces this point site belongs to.
    const target* pf_;
    /// The handle of the face this psite is pointing towards.
    topo::n_face<N, D> face_;
    /// \}
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


  /// \brief Is \a lhs equal to \a rhs?
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::complex.
  template <unsigned N, unsigned D, typename P>
  bool
  operator!=(const faces_psite<N, D, P>& lhs,
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


  template <unsigned N, unsigned D, typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const faces_psite<N, D, P>& p);



# ifndef MLN_INCLUDE_ONLY

  template <unsigned N, unsigned D, typename P>
  inline
  faces_psite<N, D, P>::faces_psite()
    : pf_(0)
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();

    invalidate();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  faces_psite<N, D, P>::faces_psite(const p_faces<N, D, P>& pf,
				    const topo::n_face<N, D>& face)
    : pf_(&pf),
      face_(face)
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();
    // Check arguments consistency.
//     mln_precondition(pf.cplx() == face.cplx());

    update_();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  faces_psite<N, D, P>::faces_psite(const p_faces<N, D, P>& pf,
				    unsigned face_id)
    : pf_(&pf),
      face_(pf.cplx(), face_id)
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();

    update_();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  bool
  faces_psite<N, D, P>::is_valid() const
  {
//     mln_invariant(!pf_ || pf_.cplx() == face_.cplx());
    return face_.is_valid();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  void
  faces_psite<N, D, P>::invalidate()
  {
    return face_.invalidate();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  const p_faces<N, D, P>&
  faces_psite<N, D, P>::site_set() const
  {
    mln_precondition(target_());
    return *target_();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  const p_faces<N, D, P>*
  faces_psite<N, D, P>::target_() const
  {
//     mln_invariant(!pf_ || pf_.cplx() == face_.cplx());
    return pf_;
  }

  template <unsigned N, unsigned D, typename P>
  inline
  void
  faces_psite<N, D, P>::change_target(const target& new_target)
  {
    // Update both pc_ and face_.
    pf_ = &new_target;
    face_.set_cplx(new_target.cplx());
    invalidate();
  }

  // FIXME: Write or extend a test to exercise this method (when the
  // handling of P is done, i.e., when update_ is complete).
  template <unsigned N, unsigned D, typename P>
  inline
  const P&
  faces_psite<N, D, P>::subj_()
  {
    // FIXME: Member p_ is not updated correctly yet; do not use this
    // method for now.
    abort();
    return p_;
  }

  template <unsigned N, unsigned D, typename P>
  inline
  topo::n_face<N, D>
  faces_psite<N, D, P>::face() const
  {
    return face_;
  }

  template <unsigned N, unsigned D, typename P>
  inline
  unsigned
  faces_psite<N, D, P>::n() const
  {
    return face_.n();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  unsigned
  faces_psite<N, D, P>::face_id() const
  {
    return face_.face_id();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  void
  faces_psite<N, D, P>::update_()
  {
    mln_precondition(is_valid());
//     mln_invariant(!pf_ || pf_.cplx() == face_.cplx());
    // FIXME: Implement (update p_).
  }


  /*--------------.
  | Comparisons.  |
  `--------------*/

  template <unsigned N, unsigned D, typename P>
  bool
  operator==(const faces_psite<N, D, P>& lhs,
	     const faces_psite<N, D, P>& rhs)
  {
    mln_precondition(&lhs.site_set() == &rhs.site_set());
    return lhs.face() == rhs.face();
  }

  template <unsigned N, unsigned D, typename P>
  bool
  operator!=(const faces_psite<N, D, P>& lhs,
	     const faces_psite<N, D, P>& rhs)
  {
    mln_precondition(&lhs.site_set() == &rhs.site_set());
    return lhs.face() != rhs.face();
  }

  template <unsigned N, unsigned D, typename P>
  bool
  operator< (const faces_psite<N, D, P>& lhs,
	     const faces_psite<N, D, P>& rhs)
  {
    mln_precondition(&lhs.site_set() == &rhs.site_set());
    return lhs.face() < rhs.face();
  }


  /*------------------.
  | Pretty-printing.  |
  `------------------*/

  template <unsigned N, unsigned D, typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const faces_psite<N, D, P>& p)
  {
    return ostr << "(dim = " << p.n() << ", id = " << p.face_id() << ')';
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln

#endif // ! MLN_CORE_FACES_PSITE_HH
