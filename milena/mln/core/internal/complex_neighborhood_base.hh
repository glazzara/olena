// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_INTERNAL_COMPLEX_NEIGHBORHOOD_BASE_HH
# define MLN_CORE_INTERNAL_COMPLEX_NEIGHBORHOOD_BASE_HH

/// \file
/// \brief Definition of a generic neighborhood of the face of a
/// complex, based on a pair of (forward and backward) complex
/// iterators.

# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/complex_psite.hh>
# include <mln/core/image/complex_neighborhood_piter.hh>

# include <mln/core/internal/neighb_base.hh>


namespace mln
{
  // Forward declarations.
  template <typename I, typename G, typename N>
  class complex_neighborhood_fwd_piter;
  template <typename I, typename G, typename N>
  class complex_neighborhood_bkd_piter;

  namespace internal
  {
    template <typename W, typename E>
    class complex_neighborhood_base;
  }


  namespace internal
  {
    /*!
      \internal
      \brief Generic neighborhood centered on the face of a complex,
      based on an pair of (forward and backward) complex iterators.

      \tparam W The underlying window.
      \tparam E The exact type.
    */
    template <typename W, typename E>
    class complex_neighborhood_base : public Neighborhood<E>
    {
    public:
      /// The associated complex iterators.
      /// \{
      typedef typename W::complex_fwd_iter complex_fwd_iter;
      typedef typename W::complex_bkd_iter complex_bkd_iter;
      /// \}

    public:
      /// Associated types.
      /// \{
      /// The geometry of the complex.
      typedef mln_geom(W) G;
      /// The type of psite corresponding to the neighborhood.
      typedef mln_psite(W) psite;
      /// The type of site corresponding to the neighborhood.
      typedef mln_site(W) site;

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

      /// The window type corresponding to this neighborhood.
      typedef W window;
      /// \}

    public:
      /// Get the corresponding window.
      const W& win() const;

      /// Is this neighborhood valid?
      bool is_valid() const;

    private:
      /// The underlying window.
      W win_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename W, typename E>
    inline
    const W&
    complex_neighborhood_base<W, E>::win() const
    {
      return win_;
    }

    template <typename W, typename E>
    inline
    bool
    complex_neighborhood_base<W, E>::is_valid() const
    {
      return win().is_valid();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln

#endif // ! MLN_CORE_INTERNAL_COMPLEX_NEIGHBORHOOD_BASE_HH
