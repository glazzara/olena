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

#ifndef MLN_DEBUG_DRAW_GRAPH_HH
# define MLN_DEBUG_DRAW_GRAPH_HH

/// \file mln/debug/draw_graph.hh
///
/// \brief Draw an (classical) image from a mln::graph_image.
///
/// \todo write a version for graph images.

# include <mln/core/site_set/p_vertices.hh>
# include <mln/core/site_set/p_edges.hh>
# include <mln/draw/line.hh>
# include <mln/level/fill.hh>

namespace mln
{

  namespace debug
  {

    /// Draw an image \p ima from a mln::p_graph \p pg, with
    /// value \p vertex_v for vertices, value \p edge_v for edges and 0 for
    /// the background.
    ///
    /// \param[in,out] ima      The image to be drawn.
    /// \param[in]     pv       The p_vertices which contains vertices positions.
    /// \param[in]     vertex_v The value to assign to pixels which contains
    ///				vertices.
    /// \param[in]     edge_v   The value to assign to pixels which contains
    ///                         edges.
    template <typename I, typename G, typename F>
    void
    draw_graph(Image<I>& ima, const p_vertices<G, F>& pv,
	       mln_value(I) vertex_v, mln_value(I) edge_v);


    template <typename I, typename G, typename F, typename E>
    void
    draw_graph(Image<I>& ima,
	       const p_vertices<G, F>& pv, const Function<E>& fcolor);


# ifndef MLN_INCLUDE_ONLY



    // FIXME: Add assertions on the size of the image: it must be large
    // enough to hold the representation of the graph/graph_image.

    template <typename I, typename G, typename F>
    inline
    void
    draw_graph(Image<I>& ima,
	       const p_vertices<G, F>& pv,
	       mln_value(I) vertex_v,
	       mln_value(I) edge_v)
    {
      // Draw edges.
      const G& g = pv.graph();
      typedef p_vertices<G, F> pv_t;
      mln_edge_iter(G) ei(g);
      for_all(ei)
	draw::line(exact(ima), pv(ei.v1()), pv(ei.v2()), edge_v);

      // Draw vertices.
      mln_piter(pv_t) p(pv);
      for_all(p)
	if (exact(ima).has(p))
	  exact(ima)(p) = vertex_v;
    }

    // FIXME: Refactor + be more restrictive on the function type.
    template <typename I, typename G, typename F, typename E>
    inline
    void
    draw_graph(Image<I>& ima,
	       const p_vertices<G, F>& pv, const Function<E>& fcolor_)
    {
      const E& fcolor = exact(fcolor_);

      // Draw edges.
      const G& g = pv.graph();
      typedef p_vertices<G, F> pv_t;
      mln_edge_iter(G) ei(g);
      for_all(ei)
	draw::line(exact(ima), pv(ei.v1()), pv(ei.v2()), fcolor(ei));

      // Draw vertices.
      mln_piter(pv_t) p(pv);
      for_all(p)
	if (exact(ima).has(p))
	  exact(ima)(p) = fcolor(p.element());
    }

# endif // ! MLN_INCLUDE_ONLY

   } // end of namespace mln::debug

} // end of namespace mln

#endif // ! MLN_DEBUG_DRAW_GRAPH_HH
