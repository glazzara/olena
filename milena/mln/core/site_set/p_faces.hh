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

#ifndef MLN_CORE_SITE_SET_P_FACES_HH
# define MLN_CORE_SITE_SET_P_FACES_HH

/// \file mln/core/site_set/p_faces.hh
/// \brief Definition of a point set based on the set of n-faces of a
/// complex.

# include <mln/core/internal/point_set_base.hh>

# include <mln/accu/bbox.hh>
# include <mln/util/tracked_ptr.hh>
# include <mln/core/complex.hh>

# include <mln/core/faces_psite.hh>


namespace mln
{

  /* FIXME: For compatibility reasons with mln::Point_Set, a point
     type \P is attached to this complex-based pset (but it is not
     used actually).  We should either:

     - use it, and make it mandatory (good solution for the moment);
     - use it, and make it optional (better solution, but implies
       changes in mln::Point_Set;
     - remove it (easy and bad solution).  */

  /// A complex psite set based on a the \N-faces of a complex of
  /// dimension \p D (a \p D-complex).
  template <unsigned N, unsigned D, typename P>
  struct p_faces
    : public internal::point_set_base_< faces_psite<N, D, P>,
					p_faces<N, D, P> >
  {
    /// \brief Construct a complex psite set from a complex.
    ///
    /// \param gr The complex upon which the complex psite set is built.
    ///
    /// \a gr is \em copied internally, so that the complex psite set is
    /// still valid after the initial complex has been removed.
    p_faces (const complex<D>& cplx);

    /// Point_Site associated type.
    typedef faces_psite<N, D, P> psite;

    // FIXME: Fake.
    typedef void fwd_piter;
    typedef void bkd_piter;

    /// \brief Return The number of points (sites) of the set, i.e., the
    /// number of \em faces.
    ///
    /// Required by the mln::Point_Set concept.
    std::size_t nsites() const;

    /// Return The number of faces in the complex.
    std::size_t nfaces() const;

    /// Give the exact bounding box.
    const box<P>& bbox() const;

    bool has(const psite& p) const;

    /// Return the complex associated to the p_faces domain.
    const complex<D>& cplx() const;

  private:
    /// The complex on which this pset is built.
    util::tracked_ptr< complex<D> > cplx_;
    // FIXME: Remove as soon as bbox become optional.
    box<P> bb_;
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
  p_faces<N, D, P>::p_faces(const complex<D>& cplx)
    // Create a deep, managed copy of CPLX.
    : cplx_(new complex<D>(cplx))
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();

    // FIXME: Dummy initialization.
    accu::bbox<P> a;
    for (unsigned i = 0; i < nsites(); ++i)
      a.take(P());
    bb_ = a.to_result();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  std::size_t
  p_faces<N, D, P>::nsites() const
  {
    return nfaces();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  std::size_t
  p_faces<N, D, P>::nfaces() const
  {
    return cplx_->template nfaces<N>();
  }

  template <unsigned N, unsigned D, typename P>
  inline
  const box<P>&
  p_faces<N, D, P>::bbox() const
  {
    // FIXME: Dummy value.
    return bb_;
  }

  template <unsigned N, unsigned D, typename P>
  inline
  bool
  p_faces<N, D, P>::has(const psite& p) const
  {
    return
      // Check whether P's complex is compatible with this pset's complex.
      &p.cplx() == &cplx() &&
      // Check whether the complex has the face associated to P.
      p.face().is_valid();
  }


  template <unsigned N, unsigned D, typename P>
  const complex<D>&
  p_faces<N, D, P>::cplx() const
  {
    mln_precondition(cplx_);
    return *cplx_.ptr_;
  }


  template <unsigned N, unsigned D, typename P>
  bool
  operator==(const p_faces<N, D, P>& lhs, const p_faces<N, D, P>& rhs)
  {
    return lhs.cplx_.ptr_ == rhs.cplx_.ptr_;
  }

  template <unsigned N, unsigned D, typename P>
  bool
  operator<=(const p_faces<N, D, P>& lhs, const p_faces<N, D, P>& rhs)
  {
    return lhs == rhs;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // MLN_CORE_SITE_SET_P_FACES_HH
