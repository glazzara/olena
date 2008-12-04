// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_IMAGE_GRAPH_ELT_WINDOW_HH
# define MLN_CORE_IMAGE_GRAPH_ELT_WINDOW_HH

/// \file mln/core/image/graph_elt_window.hh
///
/// Definition of the elementary ``window'' on a graph.

# include <mln/core/concept/window.hh>
# include <mln/core/internal/neighborhood_base.hh>
# include <mln/core/internal/graph_window_base.hh>
# include <mln/core/image/graph_window_piter.hh>
# include <mln/core/site_set/p_vertices.hh>


namespace mln
{

  /// Forward declaration
  template <typename G, typename F> class graph_elt_window;

  namespace internal
  {

    template <typename G, typename F, typename E>
    struct neighborhood_impl<graph_elt_window<G,F>,E>
      : public neighborhood_extra_impl<graph_elt_window<G,F>,E>
    {
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename G, typename F>
    struct window_< mln::graph_elt_window<G, F> >
    {
      typedef trait::window::size::unknown       size;
      typedef trait::window::support::irregular  support;
      typedef trait::window::definition::varying definition;
    };

  } // end of namespace mln::trait


  /// \brief Elementary window on graph class.
  template <typename G, typename F>
  class graph_elt_window : public graph_window_base<mln_result(F),
						    graph_elt_window<G,F> >

  {
    typedef graph_elt_window<G, F> self_;
    typedef mln_vertex_nbh_vertex_fwd_iter(G) nbh_fwd_iter_;
    typedef mln_vertex_nbh_vertex_bkd_iter(G) nbh_bkd_iter_;


  public:
    /// Associated types.
    /// \{
    typedef p_vertices<G,F> S;
    /// The type of psite corresponding to the window.
    typedef mln_psite(S) psite;

    /// Site_Iterator type to browse the psites of the window
    /// w.r.t. the ordering of vertices.
    typedef graph_window_piter<S,self_,nbh_fwd_iter_> fwd_qiter;

    /// Site_Iterator type to browse the psites of the window
    /// w.r.t. the reverse ordering of vertices.
    typedef graph_window_piter<S,self_,nbh_bkd_iter_> bkd_qiter;

    /// The default qiter type.
    typedef fwd_qiter qiter;
    /// \}

  };

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_GRAPH_ELT_WINDOW_HH
