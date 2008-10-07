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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_IMAGE_COMPLEX_HIGHER_NEIGHBORHOOD_HH
# define MLN_CORE_IMAGE_COMPLEX_HIGHER_NEIGHBORHOOD_HH

/// \file mln/core/image/complex_higher_neighborhood.hh
/// \brief A neighborhood centered on a n-face of complex returning its
/// adjacent (n+1)-faces.

# include <mln/core/concept/neighborhood.hh>

# include <mln/core/site_set/complex_psite.hh>

# include <mln/topo/adj_higher_face_iter.hh>

// FIXME: Factor with complex_lower_neighborhood.


namespace mln
{
  // Forward declarations.
  template <typename I, typename P, typename N>
  class complex_neighborhood_fwd_piter;
  template <typename I, typename P, typename N>
  class complex_neighborhood_bkd_piter;


  /// \brief Neighborhood centered on a n-face of complex returning its
  /// adjacent (n-1)-faces.
  template <unsigned D, typename P>
  class complex_higher_neighborhood
    : public Neighborhood< complex_higher_neighborhood<D, P> >
  {
    typedef complex_higher_neighborhood<D, P> self_;

  public:
    /// The associated complex iterators.
    /// \{
    typedef topo::adj_higher_face_fwd_iter<D> complex_fwd_iter;
    typedef topo::adj_higher_face_bkd_iter<D> complex_bkd_iter;
    /// \}

  public:
    /// Associated types.
    /// \{
    /// The type of psite corresponding to the neighborhood.
    typedef complex_psite<D, P> psite;
    /// The type of site corresponding to the neighborhood.
    typedef mln_site(psite) site;

    /// \brief Site_Iterator type to browse the psites of the neighborhood
    /// w.r.t. the ordering of vertices.
    typedef
    complex_neighborhood_fwd_piter<complex_fwd_iter, P, self_> fwd_niter;

    /// \brief Site_Iterator type to browse the psites of the neighborhood
    /// w.r.t. the reverse ordering of vertices.
    typedef
    complex_neighborhood_bkd_piter<complex_bkd_iter, P, self_> bkd_niter;

    /// The default niter type.
    typedef fwd_niter niter;
    /// \}

    /// Conversions.
    /// \{
    /// The window type corresponding to this neighborhood.
    // FIXME: Dummy.
    typedef self_ window;
    /// Create a window corresponding to this neighborhood.
    const window& win() const;
    /// \}
  };



# ifndef MLN_INCLUDE_ONLY

  // FIXME: Dummy.
  template <unsigned D, typename P>
  inline
  // FIXME: Change (dummy) type.
  const typename complex_higher_neighborhood<D, P>::window&
  complex_higher_neighborhood<D, P>::win() const
  {
    // FIXME: Dummy.
    return *this;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_IMAGE_COMPLEX_HIGHER_NEIGHBORHOOD_HH
