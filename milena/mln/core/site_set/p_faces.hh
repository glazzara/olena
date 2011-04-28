// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_CORE_SITE_SET_P_FACES_HH
# define MLN_CORE_SITE_SET_P_FACES_HH

/// \file
///
/// definition of a point set based on the set of n-faces of a
/// complex.

# include <mln/core/internal/site_set_base.hh>

# include <mln/topo/complex.hh>

# include <mln/core/faces_psite.hh>

// FIXME: Disabled (moved to the attic).
# if 0
#  include <mln/core/site_set/p_faces_piter.hh>
# endif

# include <mln/core/site_set/p_complex.hh>


namespace mln
{

  // Forward declarations.
  template <unsigned N, unsigned D, typename P> struct p_faces;

  template <unsigned N, unsigned D, typename P> class p_faces_fwd_piter_;
  template <unsigned N, unsigned D, typename P> class p_faces_bkd_piter_;


  namespace trait
  {
    template <unsigned N, unsigned D, typename P>
    struct site_set_< p_faces<N, D, P> >
    {
      typedef trait::site_set::nsites::known   nsites;
      // FIXME: Depends on P!
      typedef trait::site_set::bbox::unknown   bbox;
      typedef trait::site_set::contents::fixed contents;
      typedef trait::site_set::arity::unique   arity;
    };
  } // end of namespace mln::trait


  /// \brief A complex psite set based on a the N-faces of a complex of
  /// dimension D (a D-complex).
  ///
  /// \ingroup modsitesetgraph
  template <unsigned N, unsigned D, typename P>
  struct p_faces
    : public internal::site_set_base_< faces_psite<N, D, P>,
				       p_faces<N, D, P> >
  {
    typedef p_faces<N, D, P> self_;
    typedef internal::site_set_base_< faces_psite<N, D, P>, self_ > super_;

    /// \brief Construct a faces psite set from an mln::complex.
    ///
    /// \param cplx The complex upon which the complex psite set is built.
    p_faces(const topo::complex<D>& cplx);

    /// \brief Construct a faces psite set from an mln::p_complex.
    ///
    /// \param pc The complex upon which the complex psite set is built.
    ///
    /// \todo When available, get location information from \a pc.
    p_faces(const p_complex<D, P>& pc);

    /// Associated types.
    /// \{
    /// Element associated type.
    typedef mln_site(super_) element;

    /// Point_Site associated type.
    typedef faces_psite<N, D, P> psite;

    // FIXME: Fake.
    /// Forward Site_Iterator associated type.
    typedef p_faces_fwd_piter_<N, D, P> fwd_piter;

    // FIXME: Fake.
    /// Backward Site_Iterator associated type.
    typedef p_faces_bkd_piter_<N, D, P> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;
    /// \}

    /// \brief Return The number of sites of the set, i.e., the number
    /// of \em faces.
    ///
    /// (Required by the mln::Site_Set concept, since the property
    /// trait::site_set::nsites::known of this site set is set to
    /// `known'.)
    unsigned nsites() const;

    /// Return The number of faces in the complex.
    unsigned nfaces() const;

    // FIXME: This method is probably useless now.
    /// Is this site set valid?
    bool is_valid() const;

    bool has(const psite& p) const;

    // FIXME: Dummy.
    std::size_t memory_size() const;

    /// Accessors.
    /// \{
    /// Return the complex associated to the p_faces domain (const
    /// version).
    /* FIXME: Move back the const qualifier on this return type (see
       comment below on cplx_). */
    topo::complex<D>& cplx() const;
    /// Return the complex associated to the p_faces domain (mutable
    /// version).
    topo::complex<D>& cplx();
    /// \}

  private:
    /// The complex on which this pset is built.
    /* FIXME: Get rid of this `mutable' qualifier.  This is needed for
       compatiblity reasons with face_handle (see p_faces_piter).

       We should either

       - have an additional version of face_handle holding a const
         (not mutable) complex;

       - have face_handle and any_face_handle do not hold a reference
         on a complex, leading to a design of complexes similar to
         graphs, where vertex and edge handles (named `id's) are not
         tied to a specific graph.  */
    mutable topo::complex<D> cplx_;
  };


  /// \brief Comparison between two mln::p_faces's.
  ///
  /// Two mln::p_faces's are considered equal if they share the
  /// same complex.
  template <unsigned N, unsigned D, typename P>
  bool
  operator==(const p_faces<N, D, P>& lhs, const p_faces<N, D, P>& rhs);

  /// \brief Inclusion of a mln::p_faces in another one.
  ///
  /// This inclusion relation is very strict for the moment, since our
  /// infrastrure for complexs is simple: a mln::p_faces is included
  /// in another one if their are equal.
  ///
  /// \todo Refine this later, when we are able to express subcomplex
  /// relations.
  template <unsigned N, unsigned D, typename P>
  bool
  operator<=(const p_faces<N, D, P>& lhs, const p_faces<N, D, P>& rhs);



# ifndef MLN_INCLUDE_ONLY

  template <unsigned N, unsigned D, typename P>
  inline
  p_faces<N, D, P>::p_faces(const topo::complex<D>& cplx)
    : cplx_(cplx)
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  p_faces<N, D, P>::p_faces(const p_complex<D, P>& pc)
    : cplx_(pc.cplx())
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  unsigned
  p_faces<N, D, P>::nsites() const
  {
    return nfaces();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  unsigned
  p_faces<N, D, P>::nfaces() const
  {
    return cplx_.template nfaces_of_static_dim<N>();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  bool
  p_faces<N, D, P>::is_valid() const
  {
    return true;
  }

  template <unsigned N, unsigned D, typename P>
  inline
  bool
  p_faces<N, D, P>::has(const psite& p) const
  {
    mln_precondition(is_valid());
    return
      // Check whether P's complex is compatible with this pset's complex.
      (p.site_set() == *this) &&
      // Check whether P is valid.
      (p.is_valid());
  }

  template <unsigned N, unsigned D, typename P>
  inline
  std::size_t
  p_faces<N, D, P>::memory_size() const
  {
    // FIXME: Dummy; implement (see other site sets).
    abort();
    return 0;
  }

  template <unsigned N, unsigned D, typename P>
  topo::complex<D>&
  p_faces<N, D, P>::cplx() const
  {
    mln_precondition(is_valid());
    return cplx_;
  }

  template <unsigned N, unsigned D, typename P>
  topo::complex<D>&
  p_faces<N, D, P>::cplx()
  {
    mln_precondition(is_valid());
    return cplx_;
  }


  /*--------------.
  | Comparisons.  |
  `--------------*/

  template <unsigned N, unsigned D, typename P>
  bool
  operator==(const p_faces<N, D, P>& lhs, const p_faces<N, D, P>& rhs)
  {
    /* FIXME: When actual location data is attached to a p_faces,
       check also the equality w.r.t. to these data.  */
    return lhs.cplx() == rhs.cplx();
  }

  template <unsigned N, unsigned D, typename P>
  bool
  operator<=(const p_faces<N, D, P>& lhs, const p_faces<N, D, P>& rhs)
  {
    /* FIXME: When actual location data is attached to a p_faces,
       check also the equality w.r.t. to these data.  */
    return lhs == rhs;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln

#endif // ! MLN_CORE_SITE_SET_P_FACES_HH
