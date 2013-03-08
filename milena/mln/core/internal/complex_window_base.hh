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

#ifndef MLN_CORE_INTERNAL_COMPLEX_WINDOW_BASE_HH
# define MLN_CORE_INTERNAL_COMPLEX_WINDOW_BASE_HH

/// \file
/// \brief Definition of a generic window of the face of a complex,
/// based on a pair of (forward and backward) complex iterators.

# include <mln/core/concept/window.hh>
# include <mln/core/site_set/complex_psite.hh>
# include <mln/core/image/complex_window_piter.hh>

# include <mln/topo/centered_iter_adapter.hh>

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
    class complex_window_base;
  }


  namespace trait
  {

    template <unsigned D, typename G, typename F, typename B, typename E>
    struct window_< mln::internal::complex_window_base<D, G, F, B, E> >
    {
      typedef trait::window::size::unknown       size;
      typedef trait::window::support::irregular  support;
      typedef trait::window::definition::varying definition;
    };

  } // end of namespace mln::trait


  namespace internal
  {
    /*!
      \internal
      \brief Generic window centered on the face of a complex, based
      on an pair of (forward and backward) complex iterators.  The
      center (site) is part of the window.

      \tparam D The dimension of the complex.
      \tparam G The type of the geometry functor of the complex.
      \tparam F The underlying forward iterator type.
      \tparam B The underlying backward iterator type.
      \tparam E The exact type.
    */
    template <unsigned D, typename G, typename F, typename B, typename E>
    class complex_window_base : public Window<E>
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
      /// The geometry of the complex.
      typedef G geom;
      /// The type of psite corresponding to the window.
      typedef complex_psite<D, G> psite;
      /// The type of site corresponding to the window.
      typedef mln_site(psite) site;

      // FIXME: This is a dummy value.
      typedef void dpsite;

      /* FIXME: Ideally, the `is_centered' information should be
         fetched from the iterators, but that's not a straighforward
         task.  */
      complex_window_base(bool is_centered = false);

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
      /// Is this window centered?
      bool is_centered() const;

      /// Is this window valid ? (Return true by default.)
      bool is_valid() const;
      /// \}

    private:
      bool is_centered_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <unsigned D, typename G, typename F, typename B, typename E>
    inline
    complex_window_base<D, G, F, B, E>::complex_window_base(bool is_centered)
      : is_centered_(is_centered)
    {
    }


    template <unsigned D, typename G, typename F, typename B, typename E>
    inline
    bool
    complex_window_base<D, G, F, B, E>::is_empty() const
    {
      return false;
    }

    template <unsigned D, typename G, typename F, typename B, typename E>
    inline
    bool
    complex_window_base<D, G, F, B, E>::is_centered() const
    {
      return is_centered_;
    }

    template <unsigned D, typename G, typename F, typename B, typename E>
    inline
    bool
    complex_window_base<D, G, F, B, E>::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln

#endif // ! MLN_CORE_INTERNAL_COMPLEX_WINDOW_BASE_HH
