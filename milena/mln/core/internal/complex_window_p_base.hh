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

#ifndef MLN_CORE_IMAGE_COMPLEX_WINDOW_P_BASE_HH
# define MLN_CORE_IMAGE_COMPLEX_WINDOW_P_BASE_HH

/// \file mln/core/image/complex_window_p_base.hh
/// \brief Definition of a generic window centered on the face
/// of a complex, based on a pair of (forward and backward) complex
/// iterators.  The center (site) is part of the window.

# include <mln/core/concept/window.hh>
# include <mln/core/site_set/complex_psite.hh>

# include <mln/topo/adj_lower_face_iter.hh>

// FIXME: Factor with mln::internal::complex_neighborhood_base.


namespace mln
{
  // Forward declarations.
  template <typename I, typename G, typename W>
  class complex_window_fwd_piter;
  template <typename I, typename G, typename W>
  class complex_window_bkd_piter;

  namespace internal
  {
    template <unsigned D, typename G, typename F, typename B, typename E>
    class complex_window_p_base;
  }


  namespace trait
  {

    template <unsigned D, typename G, typename F, typename B, typename E>
    struct window_< mln::internal::complex_window_p_base<D, G, F, B, E> >
    {
      typedef trait::window::size::unknown       size;
      typedef trait::window::support::irregular  support;
      typedef trait::window::definition::varying definition;
    };

  } // end of namespace mln::trait


  namespace internal
  {
    /** \brief Generic window centered on the face of a complex, based
	on an pair of (forward and backward) complex iterators.  The
	center (site) is part of the window.

	\tparam D The dimension of the complex.
	\tparam G The geometry functor of the complex.
	\tparam F The underlying forward iterator.
	\tparam B The underlying backward iterator.
	\tparam E The exact type.  */
    template <unsigned D, typename G, typename F, typename B, typename E>
    class complex_window_p_base : public Window<E>
    {
      /// The complex iterators <em>on the adjacent faces only</em>
      /// (without the center point).
      /// \{
      typedef F adj_only_fwd_iter_;
      typedef B adj_only_bkd_iter_;
      /// \}

    public:
      /// The associated complex iterators.
      /// \{
      typedef topo::centered_fwd_iter_adapter<D, adj_only_fwd_iter_>
      complex_fwd_iter;

      typedef topo::centered_bkd_iter_adapter<D, adj_only_bkd_iter_>
      complex_bkd_iter;
      /// \}

    public:
      /// Associated types.
      /// \{
      /// The type of psite corresponding to the window.
      typedef complex_psite<D, G> psite;
      /// The type of site corresponding to the window.
      typedef mln_site(psite) site;

      // FIXME: This is a dummy value.
      typedef void dpsite;

      /// \brief Site_Iterator type to browse the psites of the window
      /// w.r.t. the ordering of vertices.
      typedef
      complex_window_fwd_piter<complex_fwd_iter, G, E> fwd_qiter;

      /// \brief Site_Iterator type to browse the psites of the window
      /// w.r.t. the reverse ordering of vertices.
      typedef
      complex_window_bkd_piter<complex_bkd_iter, G, E> bkd_qiter;

      /// The default qiter type.
      typedef fwd_qiter qiter;
      /// \}

    public:
      /// Services.
      /// \{
      /* FIXME: mln::morpho::dilation requires these method from models
	 of concept Window, but Window does not list them in its
	 requirements.  Who's guilty: morpho::dilation or Window?  */
      /// Is this window empty?  (Always returns \c false).
      bool is_empty() const;
      /// Is this window centered?  (Always returns \c true).
      bool is_centered() const;
      /// \}
    };



# ifndef MLN_INCLUDE_ONLY

    template <unsigned D, typename G, typename F, typename B, typename E>
    bool
    complex_window_p_base<D, G, F, B, E>::is_empty() const
    {
      return false;
    }

    template <unsigned D, typename G, typename F, typename B, typename E>
    bool
    complex_window_p_base<D, G, F, B, E>::is_centered() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY
    
  } // end of namespace mln::internal

} // end of namespace mln

#endif // ! MLN_CORE_IMAGE_COMPLEX_WINDOW_P_BASE_HH
