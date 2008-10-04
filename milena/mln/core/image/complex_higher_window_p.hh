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

#ifndef MLN_CORE_IMAGE_COMPLEX_HIGHER_WINDOW_P_HH
# define MLN_CORE_IMAGE_COMPLEX_HIGHER_WINDOW_P_HH

/// \file mln/core/complex_higher_window_p.hh
/// \brief Definition of a window centered on a n-face of complex
/// returning its adjacent (n+1)-faces as well as the center n-face.

# include <mln/core/concept/window.hh>

# include <mln/core/site_set/complex_psite.hh>

# include <mln/topo/centered_iter_adapter.hh>
# include <mln/topo/adj_higher_face_iter.hh>


namespace mln
{
  // Forward declarations.
  template <unsigned D, typename P> class complex_higher_window_p;
  template <typename I, typename P, typename W> class complex_window_fwd_piter;
  template <typename I, typename P, typename W> class complex_window_bkd_piter;


  namespace trait
  {

    template <unsigned D, typename P>
    struct window_< mln::complex_higher_window_p<D,P> >
    {
      typedef trait::window::size::unknown       size;
      typedef trait::window::support::irregular  support;
      typedef trait::window::definition::varying definition;
    };

  } // end of namespace mln::trait


  /// \brief Window centered on a n-face of complex returning its
  /// adjacent (n+1)-faces as well as the center n-face.
  template <unsigned D, typename P>
  class complex_higher_window_p
    : public Window< complex_higher_window_p<D, P> >
  {
    typedef complex_higher_window_p<D, P> self_;
    /// The complex iterators on the <em>adjacent</em> faces only
    /// (without the center point).
    /// \{
    typedef topo::adj_higher_face_fwd_iter<D> adj_fwd_iter_;
    typedef topo::adj_higher_face_bkd_iter<D> adj_bkd_iter_;
    /// \}

  public:
    /// The associated complex iterators.
    /// \{
    typedef topo::centered_fwd_iter_adapter<D, adj_fwd_iter_> complex_fwd_iter;
    typedef topo::centered_bkd_iter_adapter<D, adj_bkd_iter_> complex_bkd_iter;
    /// \}

  public:
    /// Associated types.
    /// \{
    /// The type of psite corresponding to the window.
    typedef complex_psite<D, P> psite;
    /// The type of site corresponding to the window.
    typedef mln_site(psite) site;

    // FIXME: This is a dummy value.
    typedef void dpsite;

    /// \brief Site_Iterator type to browse the psites of the window
    /// w.r.t. the ordering of vertices.
    typedef
    complex_window_fwd_piter<complex_fwd_iter, P, self_> fwd_qiter;

    /// \brief Site_Iterator type to browse the psites of the window
    /// w.r.t. the reverse ordering of vertices.
    typedef
    complex_window_bkd_piter<complex_bkd_iter, P, self_> bkd_qiter;

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

  template <unsigned D, typename P>
  bool
  complex_higher_window_p<D, P>::is_empty() const
  {
    return false;
  }

  template <unsigned D, typename P>
  bool
  complex_higher_window_p<D, P>::is_centered() const
  {
    return true;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_IMAGE_COMPLEX_HIGHER_WINDOW_P_HH
