// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file
///
/// \brief Definition of a point set based on a complex.

# include <mln/core/internal/site_set_base.hh>

# include <mln/topo/complex.hh>

# include <mln/core/site_set/complex_psite.hh>
# include <mln/core/site_set/p_complex_piter.hh>
# include <mln/core/site_set/p_n_faces_piter.hh>
// FIXME: Disabled (moved to the attic).
# if 0
#  include <mln/core/site_set/p_complex_faces_piter.hh>
# endif

# include <mln/geom/complex_geometry.hh>

// FIXME: Have G default to mln::geom::complex_geom<D, P>?  But we
// don't know P...


namespace mln
{

  // Forward declarations.
  template <unsigned D, typename G> class p_complex;

  template <unsigned D, typename G> class p_complex_fwd_piter_;
  template <unsigned D, typename G> class p_complex_bkd_piter_;

// FIXME: Disabled (moved to the attic).
# if 0
  template <unsigned N, unsigned D, typename G>
  class p_complex_faces_fwd_piter_;
  template <unsigned N, unsigned D, typename G>
  class p_complex_faces_bkd_piter_;
# endif

  namespace trait
  {
    template <unsigned D, typename G>
    struct site_set_< p_complex<D, G> >
    {
      typedef trait::site_set::nsites::known   nsites;
      // FIXME: Depends on G!
      typedef trait::site_set::bbox::unknown   bbox;
      typedef trait::site_set::contents::fixed contents;
      typedef trait::site_set::arity::unique   arity;
    };
  } // end of namespace mln::trait


  /* FIXME: We should decide was G represents:

     - a unique site type for all faces of all dimensions?
         (Acceptable for a first implementation -- the one currently
         chosen.)

     - the site type associated to 0-faces only (site types of n-faces
       with n > 1 will be deduced from this one)?
         (Better, but not really flexible.)

     - a type list of the site types associated faces of each
       dimensions, e.g.

         mln_type_list(point2d,
           mln_type_list(util::site_pair<point2d>,
             mln_type_list(site_set<point2d>,  // or site_triplet<point2d>
               mln_empty_list)))

       for a 2-complex?
         (The best solution so far, but requires more work.)  */

  /* FIXME: Aggregate site data (location).  */

  /** A complex psite set based on the N-faces of a complex
      of dimension \tparam D (a \p D-complex).

      \arg \p D The dimension of the complex.
      \arg \p G A function object type, associating localization
                information (geometry) to each face of the complex.
                \see mln::geom::complex_geometry.  */
  /// \brief A complex psite set based on the N-faces of a complex.
  ///
  /// \ingroup modsitesetcomplex
  template <unsigned D, typename G>
  class p_complex
    : public internal::site_set_base_< complex_psite<D, G>, p_complex<D, G> >
  {
    typedef p_complex<D, G> self_;
    typedef internal::site_set_base_< complex_psite<D, G>, self_ > super_;

  public:
    /// Construct a complex psite set from a complex.
    ///
    /// \param cplx The complex upon which the complex psite set is built.
    /// \param geom FIXME
    p_complex(const topo::complex<D>& cplx, const G& geom);

    /// Associated types.
    /// \{
    /// Element associated type.
    typedef mln_site(super_) element;

    /// Point_Site associated type.
    typedef complex_psite<D, G> psite;

    /// Forward Site_Iterator associated type.
    typedef p_complex_fwd_piter_<D, G> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_complex_bkd_piter_<D, G> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;
    /// \}

    /// Return The number of sites of the set, i.e., the number
    /// of \em faces.
    ///
    /// (Required by the mln::Site_Set concept, since the property
    /// trait::site_set::nsites::known of this site set is set to
    /// `known'.)
    unsigned nsites() const;

    /// Return the number of faces in the complex.
    unsigned nfaces() const;
    /// Return the number of \a n-faces in the complex.
    unsigned nfaces_of_dim(unsigned n) const;

    // FIXME: This method is probably useless now.
    /// Is this site set valid?
    bool is_valid() const;

    /// Does this site set has \a p?
    bool has(const psite& p) const;

    // FIXME: Dummy.
    std::size_t memory_size() const;

    /// Accessors.
    /// \{
    /// Return the complex associated to the p_complex domain (const
    /// version)
    /* FIXME: Move back the const qualifier on this return type (see
       comment below on cplx_). */
    topo::complex<D>& cplx() const;
    /// Return the complex associated to the p_complex domain (mutable
    /// version).
    topo::complex<D>& cplx();

    /// Return the geometry of the complex.
    const G& geom() const;
    /// \}

  private:
    /// The complex on which this pset is built.
    /* FIXME: Get rid of this `mutable' qualifier.  This is needed for
       compatiblity reasons with topo::face (see p_complex_piter).

       We should either

       - have an additional version of topo::face holding a const (not
         mutable) complex;

       - have face_handle and topo::face do not hold a reference on a
         complex, leading to a design of complexes similar to graphs,
         where vertex and edge handles (named `id's) are not tied to a
         specific graph.  */
    mutable topo::complex<D> cplx_;
    /// Geometry of the complex.
    G geom_;
  };


  /// Comparison between two mln::p_complex's.
  ///
  /// Two mln::p_complex's are considered equal if they share the
  /// same complex.
  template <unsigned D, typename G>
  bool
  operator==(const p_complex<D, G>& lhs, const p_complex<D, G>& rhs);

  /// Inclusion of a mln::p_complex in another one.
  ///
  /// This inclusion relation is very strict for the moment, since our
  /// infrastrure for complexs is simple: a mln::p_complex is included
  /// in another one if their are equal.
  ///
  /// \todo Refine this later, when we are able to express subcomplex
  /// relations.
  template <unsigned D, typename G>
  bool
  operator<=(const p_complex<D, G>& lhs, const p_complex<D, G>& rhs);



# ifndef MLN_INCLUDE_ONLY

  template <unsigned D, typename G>
  inline
  p_complex<D, G>::p_complex(const topo::complex<D>& cplx, const G& geom)
    : cplx_(cplx), geom_(geom)
  {
  }

  template <unsigned D, typename G>
  inline
  unsigned
  p_complex<D, G>::nsites() const
  {
    return nfaces();
  }

  template <unsigned D, typename G>
  inline
  unsigned
  p_complex<D, G>::nfaces() const
  {
    return cplx_.nfaces();
  }

  template <unsigned D, typename G>
  inline
  unsigned
  p_complex<D, G>::nfaces_of_dim(unsigned n) const
  {
    return cplx_.nfaces_of_dim(n);
  }

  template <unsigned D, typename G>
  inline
  bool
  p_complex<D, G>::is_valid() const
  {
    return true;
  }

  template <unsigned D, typename G>
  inline
  bool
  p_complex<D, G>::has(const psite& p) const
  {
    mln_precondition(is_valid());
    return
      // Check whether P's complex is compatible with this pset's complex.
      (p.site_set() == *this) &&
      // Check whether P is valid.
      (p.is_valid());
  }

  template <unsigned D, typename G>
  inline
  std::size_t
  p_complex<D, G>::memory_size() const
  {
    // FIXME: Dummy; implement (see other site sets). 
    abort();
    return 0;
  }

  template <unsigned D, typename G>
  topo::complex<D>&
  p_complex<D, G>::cplx() const
  {
    mln_precondition(is_valid());
    return cplx_;
  }

  template <unsigned D, typename G>
  topo::complex<D>&
  p_complex<D, G>::cplx()
  {
    mln_precondition(is_valid());
    return cplx_;
  }

  template <unsigned D, typename G>
  const G&
  p_complex<D, G>::geom() const
  {
    return geom_;
  }


  /*--------------.
  | Comparisons.  |
  `--------------*/

  template <unsigned D, typename G>
  bool
  operator==(const p_complex<D, G>& lhs, const p_complex<D, G>& rhs)
  {
    /* FIXME: When actual location data is attached to a p_complex,
       check also the equlity w.r.t. to these data.  */
    return lhs.cplx() == rhs.cplx();
  }

  template <unsigned D, typename G>
  bool
  operator<=(const p_complex<D, G>& lhs, const p_complex<D, G>& rhs)
  {
    /* FIXME: When actual location data is attached to a p_complex,
       check also the equality w.r.t. to these data.  */
    return lhs == rhs;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln

#endif // MLN_CORE_SITE_SET_P_COMPLEX_HH
