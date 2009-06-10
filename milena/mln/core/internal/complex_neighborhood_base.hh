// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_INTERNAL_COMPLEX_NEIGHBORHOOD_BASE_HH
# define MLN_CORE_INTERNAL_COMPLEX_NEIGHBORHOOD_BASE_HH

/// \file
/// \brief Definition of a generic neighborhood centered on the face
/// of a complex, based on a pair of (forward and backward) complex
/// iterators.

# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/complex_psite.hh>
# include <mln/core/image/complex_neighborhood_piter.hh>

// FIXME: Factor with mln::internal::complex_window_base.


namespace mln
{
  // Forward declarations.
  template <typename I, typename G, typename N>
  class complex_neighborhood_fwd_piter;
  template <typename I, typename G, typename N>
  class complex_neighborhood_bkd_piter;

  namespace internal
  {
    template <unsigned D, typename G, typename F, typename B, typename E>
    class complex_neighborhood_base;
  }


  namespace internal
  {
    /** \brief Generic neighborhood centered on the face of a complex,
	based on an pair of (forward and backward) complex iterators.
	
	\tparam D The dimension of the complex.
	\tparam G The type of the geometry functor of the complex.
	\tparam F The underlying forward iterator type.
	\tparam B The underlying backward iterator type.
	\tparam E The exact type.  */
  template <unsigned D, typename G, typename F, typename B, typename E>
  class complex_neighborhood_base : public Neighborhood<E>
  {
  public:
    /// The associated complex iterators.
    /// \{
    typedef F complex_fwd_iter;
    typedef B complex_bkd_iter;
    /// \}

  public:
    /// Associated types.
    /// \{
    /// The type of psite corresponding to the neighborhood.
    typedef complex_psite<D, G> psite;
    /// The type of site corresponding to the neighborhood.
    typedef mln_site(psite) site;

    /// \brief Site_Iterator type to browse the psites of the neighborhood
    /// w.r.t. the ordering of vertices.
    typedef
    complex_neighborhood_fwd_piter<complex_fwd_iter, G, E> fwd_niter;

    /// \brief Site_Iterator type to browse the psites of the neighborhood
    /// w.r.t. the reverse ordering of vertices.
    typedef
    complex_neighborhood_bkd_piter<complex_bkd_iter, G, E> bkd_niter;

    /// The default niter type.
    typedef fwd_niter niter;
    /// \}

    /// Conversions.
    /// \{
    /// The window type corresponding to this neighborhood.
    // FIXME: Dummy.
    typedef E window;
    /// Create a window corresponding to this neighborhood.
    const window& win() const;
    /// \}

    /// Return true by default.
    bool is_valid() const;
  };



# ifndef MLN_INCLUDE_ONLY

  // FIXME: Dummy.
  template <unsigned D, typename G, typename F, typename B, typename E>
  inline
  // FIXME: Change (dummy) type.
  const typename complex_neighborhood_base<D, G, F, B, E>::window&
  complex_neighborhood_base<D, G, F, B, E>::win() const
  {
    // FIXME: Dummy.
    return exact(*this);
  }

  template <unsigned D, typename G, typename F, typename B, typename E>
  inline
  bool
  complex_neighborhood_base<D, G, F, B, E>::is_valid() const
  {
    return true;
  }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln

#endif // ! MLN_CORE_INTERNAL_COMPLEX_NEIGHBORHOOD_BASE_HH
