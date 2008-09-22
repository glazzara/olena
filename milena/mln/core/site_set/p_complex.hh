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

# include <mln/core/internal/site_set_base.hh>

# include <mln/util/tracked_ptr.hh>
# include <mln/core/complex.hh>

# include <mln/core/complex_psite.hh>
# include <mln/core/site_set/p_complex_piter.hh>


namespace mln
{

  // Forward declarations.
  template <unsigned D, typename P> class p_complex_fwd_piter_;
  template <unsigned D, typename P> class p_complex_bkd_piter_;


  namespace trait
  {
    template <unsigned D, typename P>
    struct site_set_< p_complex<D, P> >
    {
      typedef trait::site_set::nsites::known   nsites;
      // FIXME: Depends on P!
      typedef trait::site_set::bbox::unknown   bbox;
      typedef trait::site_set::contents::fixed contents;
      typedef trait::site_set::arity::unique   arity;
    };
  } // end of namespace mln::trait


  /* FIXME: We should decide was P represents:

     - a unique site type for all faces of all dimensions?
         (Acceptable for a first implementation -- the one currently
         chosen.)

     - the site type associated to 0-faces only (site types of n-faces
       with n > 1 will be deduced from this one)?
         (Better, but not really flexible.)

     - a type list of the site types associated faces of each
       dimensions, e.g.

         mln_type_list(point2d,
           mln_type_list(site_pair<point2d>,
             mln_type_list(site_set<point2d>,  // or site_triplet<point2d>
               mln_empty_list)))

       for a 2-complex?
         (The best solution so far, but requires more work.)  */

  /* FIXME: Aggregate site data (location).  */

  /// A complex psite set based on a the \N-faces of a complex of
  /// dimension \p D (a \p D-complex).
  template <unsigned D, typename P>
  class p_complex
    : public internal::site_set_base_< complex_psite<D, P>, p_complex<D, P> >
  {
    typedef p_complex<D, P> self_;
    typedef internal::site_set_base_< complex_psite<D, P>, self_ > super_;

  public:
    /// \brief Construct a complex psite set from a complex.
    ///
    /// \param gr The complex upon which the complex psite set is built.
    ///
    /// \a gr is \em copied internally, so that the complex psite set is
    /// still valid after the initial complex has been removed.
    p_complex (const complex<D>& cplx);

    /// Associated types.
    /// \{
    /// Element associated type.
    typedef mln_site(super_) element;

    /// Point_Site associated type.
    typedef complex_psite<D, P> psite;

    /// Forward Site_Iterator associated type.
    typedef p_complex_fwd_piter_<D, P> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_complex_bkd_piter_<D, P> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;
    /// \}

    /// \brief Return The number of sites of the set, i.e., the number
    /// of \em faces.
    ///
    /// (Required by the mln::Site_Set concept, since the property
    /// trait::site_set::nsites::known of this site set is set to
    /// `known'.)
    /* FIXME: Return type should be std::size_t (see
       mln/core/concept/site_set.hh). */
    unsigned nsites() const;

    /// Return The number of faces in the complex.
    std::size_t nfaces() const;

    // FIXME: Add nfaces(unsigned) routines?  Yes, if this can
    // simplify (and lighten) the implementation of piters, psites,
    // etc.

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
    complex<D>& cplx() const;
    /// Return the complex associated to the p_complex domain (mutable
    /// version).
    complex<D>& cplx();
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

    // FIXME: Remove as soon as the tracked_ptr is move into the
    // complex itself.
    template <unsigned D_, typename P_>
    friend
    bool operator==(const p_complex<D_, P_>& lhs,
		    const p_complex<D_, P_>& rhs);
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
  }

  template <unsigned D, typename P>
  inline
  unsigned
  p_complex<D, P>::nsites() const
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
  p_complex<D, P>::is_valid() const
  {
    // FIXME: Might be too low-level, again.
    return (cplx_.ptr_);
  }

  template <unsigned D, typename P>
  inline
  bool
  p_complex<D, P>::has(const psite& p) const
  {
    mln_precondition(is_valid());
    return
      // Check whether P's complex is compatible with this pset's complex.
      (p.site_set() == *this) &&
      // Check whether the complex has the face associated to P.
      (p.is_valid());
  }

  template <unsigned D, typename P>
  inline
  std::size_t
  p_complex<D, P>::memory_size() const
  {
    // FIXME: Dummy; implement (see other site sets). 
    abort();
    return 0;
  }

  template <unsigned D, typename P>
  complex<D>&
  p_complex<D, P>::cplx() const
  {
    mln_precondition(is_valid());
    return *cplx_.ptr_;
  }

  template <unsigned D, typename P>
  complex<D>&
  p_complex<D, P>::cplx()
  {
    mln_precondition(is_valid());
    return *cplx_.ptr_;
  }


  /*--------------.
  | Comparisons.  |
  `--------------*/

  template <unsigned D, typename P>
  bool
  operator==(const p_complex<D, P>& lhs, const p_complex<D, P>& rhs)
  {
    /* FIXME: We should not rely on pointer equality here, as graph
       will soon become shells using (shared) tracked pointers to
       actual data.  So, delegate the equality test to the graphs
       themselves.  */
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
