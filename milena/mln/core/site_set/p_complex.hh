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

#ifndef MLN_CORE_SITE_SET_P_COMPLEX_HH
# define MLN_CORE_SITE_SET_P_COMPLEX_HH

/// \file mln/core/site_set/p_complex.hh
/// \brief Definition of a point set based on a complex.

# include <mln/core/internal/point_set_base.hh>

# include <mln/accu/bbox.hh>
# include <mln/util/tracked_ptr.hh>
# include <mln/core/complex.hh>

# include <mln/core/complex_psite.hh>
# include <mln/core/site_set/p_complex_piter.hh>


namespace mln
{

  // FIXME: Rename as p_faces.

  /* FIXME: For compatibility reasons with mln::Point_Set, a point
     type \P is attached to this complex-based pset (but it is not
     used actually).  We should either:

     - use it, and make it mandatory (good solution for the moment);
     - use it, and make it optional (better solution, but implies
       changes in mln::Point_Set;
     - remove it (easy and bad solution).  */

  /// A complex psite set based on a the \N-faces of a complex of
  /// dimension \p D (a \p D-complex).
  template <unsigned D, typename P>
  struct p_complex
    : public internal::point_set_base_< complex_psite<D, P>,
					p_complex<D, P> >
  {
    /// \brief Construct a complex psite set from a complex.
    ///
    /// \param gr The complex upon which the complex psite set is built.
    ///
    /// \a gr is \em copied internally, so that the complex psite set is
    /// still valid after the initial complex has been removed.
    p_complex (const complex<D>& cplx);

    /// Point_Site associated type.
    typedef complex_psite<D, P> psite;

    /// Forward Point_Iterator associated type.
    typedef p_complex_fwd_piter_<D, P> fwd_piter;
    /// Backward Point_Iterator associated type.
    typedef p_complex_bkd_piter_<D, P> bkd_piter;

    /// \brief Return The number of points (sites) of the set, i.e., the
    /// number of \em faces.
    ///
    /// Required by the mln::Point_Set concept.
    std::size_t npoints() const;

    /// Return The number of faces in the complex.
    std::size_t nfaces() const;

    // FIXME: Add nfaces(unsigned) routines?

    bool has(const psite& p) const;

    /// Accessors.
    /// \{
    /// Return the complex associated to the p_complex domain (const
    /// version)
    /* FIXME: Move back the const qualifier on this return type (see
       comment below on cplx_). */
    complex<D>& cplx() const;
    /// Return the complex associated to the p_complex domain (mutable
    /// version).
    complex<D>& cplx();

    /// Give the exact bounding box.
    const box_<P>& bbox() const;
    /// \}

  private:
    /// The complex on which this pset is built.
    /* FIXME:Get rid of this `mutable' qualifier.  This is needed for
       compatiblity reasons with any_face_handle (see p_complex_piter)

       We should either

       - do not use any_face_handle in the implementation of
         p_complex_piter;

       - have an additional version of any_face_handles holding a
         const (not mutable) complex;

       - or even have face_handle and any_face_handle do not hold a
         reference on a complex, leading to a design of complexes
         similar to graphs, where vertex and edge handles (named `id's)
         are not tied to a specific graph.  */
    mutable util::tracked_ptr< complex<D> > cplx_;
    // FIXME: Remove as soon as bbox become optional.
    box_<P> bb_;
  };


  /// \brief Comparison between two mln::p_complex's.
  ///
  /// Two mln::p_complex's are considered equal if they share the
  /// same complex.
  template <unsigned D, typename P>
  bool
  operator==(const p_complex<D, P>& lhs, const p_complex<D, P>& rhs);

  /// \brief Inclusion of a mln::p_complex in another one.
  ///
  /// This inclusion relation is very strict for the moment, since our
  /// infrastrure for complexs is simple: a mln::p_complex is included
  /// in another one if their are equal.
  ///
  /// \todo Refine this later, when we are able to express subcomplex
  /// relations.
  template <unsigned D, typename P>
  bool
  operator<=(const p_complex<D, P>& lhs, const p_complex<D, P>& rhs);



# ifndef MLN_INCLUDE_ONLY

  template <unsigned D, typename P>
  inline
  p_complex<D, P>::p_complex(const complex<D>& cplx)
    // Create a deep, managed copy of CPLX.
    : cplx_(new complex<D>(cplx))
  {
    // FIXME: Dummy initialization.
    accu::bbox<P> a;
    for (unsigned i = 0; i < npoints(); ++i)
      a.take(P());
    bb_ = a.to_result();
  }

  template <unsigned D, typename P>
  inline
  std::size_t
  p_complex<D, P>::npoints() const
  {
    return nfaces();
  }

  template <unsigned D, typename P>
  inline
  std::size_t
  p_complex<D, P>::nfaces() const
  {
    return cplx_->nfaces();
  }

  template <unsigned D, typename P>
  inline
  bool
  p_complex<D, P>::has(const psite& p) const
  {
    return
      // Check whether P's complex is compatible with this pset's complex.
      &p.cplx() == &cplx() &&
      // Check whether the complex has the face associated to P.
      p.face().is_valid();
  }

  template <unsigned D, typename P>
  complex<D>&
  p_complex<D, P>::cplx() const
  {
    mln_precondition(cplx_);
    return *cplx_.ptr_;
  }

  template <unsigned D, typename P>
  complex<D>&
  p_complex<D, P>::cplx()
  {
    mln_precondition(cplx_);
    return *cplx_.ptr_;
  }

  template <unsigned D, typename P>
  inline
  const box_<P>&
  p_complex<D, P>::bbox() const
  {
    // FIXME: Dummy value.
    return bb_;
  }


  template <unsigned D, typename P>
  bool
  operator==(const p_complex<D, P>& lhs, const p_complex<D, P>& rhs)
  {
    return lhs.cplx_.ptr_ == rhs.cplx_.ptr_;
  }

  template <unsigned D, typename P>
  bool
  operator<=(const p_complex<D, P>& lhs, const p_complex<D, P>& rhs)
  {
    return lhs == rhs;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // MLN_CORE_SITE_SET_P_COMPLEX_HH
