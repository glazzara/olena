// Copyright (C) 2008, 2009, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_SITE_SET_COMPLEX_PSITE_HH
# define MLN_CORE_SITE_SET_COMPLEX_PSITE_HH

/// \file
///
/// \brief Definition of a complex-based point site.

# include <mln/core/internal/pseudo_site_base.hh>

# include <mln/topo/complex.hh>

// FIXME: There's a circular dependency issue between complex_psite
// and p_complex (likewise for faces_psite and p_faces): they have to
// know their interfaces one another.  I have disabled some
// preconditions and invariants to have the code compile, but we must
// find a real solution.

// FIXME: Factor complex_psite and faces_psite?

// FIXME: Rename complex_psite as p_complex_psite, and move this file to
// core/site_set.


namespace mln
{
  // Forward declaration.
  template <unsigned D, typename G> class p_complex;


  /// \brief Point site associated to a mln::p_complex.
  ///
  /// \tparam D The dimension of the complex this psite belongs to.
  /// \tparam G The geometry of the complex.
  template <unsigned D, typename G>
  class complex_psite
    : public internal::pseudo_site_base_< const mln_site(G)&, complex_psite<D, G> >
  {
  public:
    // This associated type is important to know that this particular
    // pseudo site knows the site set it refers to.
    typedef p_complex<D, G> target;

    typedef p_complex<D, G> target_t; // To please g++-2.95.

    // FIXME: Document.
    /// Construction and assignment.
    /// \{
    complex_psite();
    /// \pre pc.cplx() == face.cplx().
    complex_psite(const p_complex<D, G>& pc,
		  const topo::face<D>& face);
    complex_psite(const p_complex<D, G>& pc, unsigned n, unsigned face_id);
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
    /// \brief Return the mln::p_complex this site is built on.
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

    /// \cond INTERNAL_API
    /// Proxy manipulators.
    /// \{
    /// Return the site corresponding to this psite.
    const mln_site(G)& subj_();
    /// \}
    /// \endcond

    /// Face handle manipulators.
    /// \{
    /// Return the face handle of this point site.
    const topo::face<D>& face() const;

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
    mln_site(G) p_;
    /// \}

    /* FIXME: Attributes pc_ and face_ share a common information: the
       address of their complex.

       This is both a loss of space and time (we must ensure
       synchronization), but this design issue is not trivial: we
       actually introduced (any-)face handles to pack together the
       location information (n, face_id) with the support (the
       complex), to avoid what we did with graphs --- where location
       (edge id or vertex id) is separated from the support (the
       graph).

       Think about it, and adjust faces_psite as well.  */
  private:
    /// Complex-related members.
    /// \{
    /// The mln::p_faces this point site belongs to.
    const target* pc_;
    /// The handle of the face this psite is pointing towards.
    topo::face<D> face_;
    /// \}
  };


  /// Comparison of two instances of mln::complex_psite.
  /// \{
  /* FIXME: Shouldn't those comparisons be part of a much general
     mechanism?  */

  /// \brief Is \a lhs equal to \a rhs?
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::p_complex.
  /// \relates mln::complex_psite<D,G>
  /* FIXME: We probably want to relax this precondition: p_complex
     equality is too strong; prefer complex equality.  */
  template <unsigned D, typename G>
  bool
  operator==(const complex_psite<D, G>& lhs,
	     const complex_psite<D, G>& rhs);

  /// \brief Is \a lhs not equal to \a rhs?
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::p_complex.
  /// \relates mln:complex_psite<D,G>
  /* FIXME: We probably want to relax this precondition: p_complex
     equality is too strong; prefer complex equality.  */
  template <unsigned D, typename G>
  bool
  operator!=(const complex_psite<D, G>& lhs,
	     const complex_psite<D, G>& rhs);

  /// \brief Is \a lhs ``less'' than \a rhs?
  ///
  /// This comparison is required by algorithms sorting psites.
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::p_complex.
  /// \relates mln::complex_psite<D,G>
  /* FIXME: We probably want to relax this precondition: p_complex
     equality is too strong; prefer complex equality.  */
  template <unsigned D, typename G>
  bool
  operator< (const complex_psite<D, G>& lhs,
	     const complex_psite<D, G>& rhs);
  /// \}

  /// \relates mln::complex_psite<D,G>
  template <unsigned D, typename G>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const complex_psite<D, G>& p);



# ifndef MLN_INCLUDE_ONLY

  template <unsigned D, typename G>
  inline
  complex_psite<D, G>::complex_psite()
    : pc_(0)
  {
    invalidate();
  }

  template <unsigned D, typename G>
  inline
  complex_psite<D, G>::complex_psite(const p_complex<D, G>& pc,
				     const topo::face<D>& face)
    : pc_(&pc),
      face_(face)
  {
    // Check arguments consistency.
    // FIXME: Re-enable when the cyclic dependencies are fixed.
#if 0
    mln_precondition(pc.cplx() == face.cplx());
#endif
    if (is_valid())
      update_();
  }

  template <unsigned D, typename G>
  inline
  complex_psite<D, G>::complex_psite(const p_complex<D, G>& pc,
				     unsigned n, unsigned face_id)
    : pc_(&pc),
      face_(pc.cplx(), n, face_id)
  {
    if (is_valid())
      update_();
  }

  template <unsigned D, typename G>
  inline
  bool
  complex_psite<D, G>::is_valid() const
  {
    // FIXME: Re-enable when the cyclic dependencies are fixed.
#if 0
    mln_invariant(!pc_ || pc_.cplx() == face_.cplx());
#endif
    return face_.is_valid();
  }

  template <unsigned D, typename G>
  inline
  void
  complex_psite<D, G>::invalidate()
  {
    return face_.invalidate();
  }

  template <unsigned D, typename G>
  inline
  const p_complex<D, G>&
  complex_psite<D, G>::site_set() const
  {
    mln_precondition(target_());
    return *target_();
  }

  template <unsigned D, typename G>
  inline
  const p_complex<D, G>*
  complex_psite<D, G>::target_() const
  {
    // FIXME: Re-enable when the cyclic dependencies are fixed.
#if 0
    mln_invariant(!pc_ || pc_.cplx() == face_.cplx());
#endif
    return pc_;
  }

  template <unsigned D, typename G>
  inline
  void
  complex_psite<D, G>::change_target(const target& new_target)
  {
    // Update both pc_ and face_.
    pc_ = &new_target;
    face_.set_cplx(new_target.cplx());
    invalidate();
  }

  // FIXME: Write or extend a test to exercise this method (when the
  // handling of G is done, i.e., when update_ is complete).
  template <unsigned D, typename G>
  inline
  const mln_site(G)&
  complex_psite<D, G>::subj_()
  {
    return p_;
  }

  template <unsigned D, typename G>
  inline
  const topo::face<D>&
  complex_psite<D, G>::face() const
  {
    return face_;
  }

  template <unsigned D, typename G>
  inline
  unsigned
  complex_psite<D, G>::n() const
  {
    return face_.n();
  }

  template <unsigned D, typename G>
  inline
  unsigned
  complex_psite<D, G>::face_id() const
  {
    return face_.face_id();
  }

  template <unsigned D, typename G>
  inline
  void
  complex_psite<D, G>::update_()
  {
    mln_precondition(is_valid());
    // FIXME: Re-enable when the cyclic dependencies are fixed.
#if 0
    mln_invariant(!pc_ || pc_.cplx() == face_.cplx());
#endif
    // FIXME: Simplify? (I.e., add accessors to shorten the following
    // line?)
    p_ = site_set().geom()(face_);
  }


  /*--------------.
  | Comparisons.  |
  `--------------*/

  template <unsigned D, typename G>
  bool
  operator==(const complex_psite<D, G>& lhs,
	     const complex_psite<D, G>& rhs)
  {
    mln_precondition(&lhs.site_set() == &rhs.site_set());
    return lhs.face() == rhs.face();
  }

  template <unsigned D, typename G>
  bool
  operator!=(const complex_psite<D, G>& lhs,
	     const complex_psite<D, G>& rhs)
  {
    mln_precondition(&lhs.site_set() == &rhs.site_set());
    return lhs.face() != rhs.face();
  }

  template <unsigned D, typename G>
  bool
  operator< (const complex_psite<D, G>& lhs,
	     const complex_psite<D, G>& rhs)
  {
    mln_precondition(&lhs.site_set() == &rhs.site_set());
    return lhs.face() < rhs.face();
  }


  /*------------------.
  | Pretty-printing.  |
  `------------------*/

  template <unsigned D, typename G>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const complex_psite<D, G>& p)
  {
    return ostr << p.face();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln

#endif // ! MLN_CORE_SITE_SET_COMPLEX_PSITE_HH
