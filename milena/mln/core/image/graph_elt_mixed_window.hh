// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_IMAGE_GRAPH_ELT_MIXED_WINDOW_HH
# define MLN_CORE_IMAGE_GRAPH_ELT_MIXED_WINDOW_HH

/// \file
///
/// Definition of the elementary ``window'' on a graph where elements
/// are in a different site set from the center's.

# include <mln/core/concept/window.hh>
# include <mln/core/internal/neighborhood_base.hh>
# include <mln/core/internal/graph_window_base.hh>
# include <mln/core/image/graph_window_piter.hh>


namespace mln
{

  /// Forward declaration
  template <typename G, typename S, typename S2> class graph_elt_mixed_window;
  template <typename G, typename F> class p_edges;
  template <typename G, typename F> class p_vertices;
  namespace util
  {
    template <typename G> class edge;
    template <typename G> class vertex;
  };


  namespace internal
  {

    template <typename G, typename S, typename S2, typename E>
    struct neighborhood_impl<graph_elt_mixed_window<G,S,S2>,E>
      : public neighborhood_extra_impl<graph_elt_mixed_window<G,S,S2>,E>
    {
    };


    /// Default
    /// The given site set parameter is not supported yet!
    ///
    /// \p G is the graph type.
    /// \p S is an image site set from where the center is extracted.
    /// \p S2 is an image site set from where the neighbors are
    /// extracted.
    //
    template <typename G, typename S, typename S2>
    struct graph_mixed_window_iter_dispatch;

    template <typename G, typename F, typename F2>
    struct graph_mixed_window_iter_dispatch<G, p_vertices<G,F>, p_edges<G,F2> >
    {
      typedef mln_vertex_nbh_edge_fwd_iter(G) nbh_fwd_iter_;
      typedef mln_vertex_nbh_edge_bkd_iter(G) nbh_bkd_iter_;

      typedef p_edges<G,F2> target;
    };


  } // end of namespace mln::internal


  namespace trait
  {

    template <typename G, typename S, typename S2>
    struct window_< mln::graph_elt_mixed_window<G,S,S2> >
    {
      typedef trait::window::size::unknown       size;
      typedef trait::window::support::irregular  support;
      typedef trait::window::definition::varying definition;
    };

  } // end of namespace mln::trait


  /// Elementary window on graph class.
  /// \p G is the graph type.
  /// \p S is an image site set from where the center is extracted.
  /// \p S2 is an image site set from where the neighbors are
  /// extracted.
  //
  template <typename G, typename S, typename S2>
  class graph_elt_mixed_window
    : public graph_window_base<mln_result(S2::fun_t),
			       graph_elt_mixed_window<G,S,S2> >,
      public internal::graph_mixed_window_iter_dispatch<G,S,S2>
  {
    typedef graph_elt_mixed_window<G,S,S2> self_;
    typedef internal::graph_mixed_window_iter_dispatch<G,S,S2> super_;

    typedef typename super_::nbh_fwd_iter_ nbh_fwd_iter_;
    typedef typename super_::nbh_bkd_iter_ nbh_bkd_iter_;

  public:
    /// Associated types.
    /// \{
    typedef typename super_::target target;
    /// The type of psite corresponding to the window.
    typedef mln_psite(target) psite;

    /// Type of the window center element.
    typedef mln_psite(S) center_t;

    /// Type of the graph element pointed by this iterator.
    typedef mln_graph_element(target) graph_element;

    /// Site_Iterator type to browse the psites of the window
    /// w.r.t. the ordering of vertices.
    typedef graph_window_piter<target,self_,nbh_fwd_iter_> fwd_qiter;

    /// Site_Iterator type to browse the psites of the window
    /// w.r.t. the reverse ordering of vertices.
    typedef graph_window_piter<target,self_,nbh_bkd_iter_> bkd_qiter;

    /// The default qiter type.
    typedef fwd_qiter qiter;
    /// \}

  };

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_GRAPH_ELT_MIXED_WINDOW_HH
