// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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
/// \brief Draw an (classical) image from a mln::graph_image.

# include <mln/pw/image.hh>
# include <mln/level/fill.hh>
# include <mln/draw/line.hh>
# include <mln/core/site_set/p_vertices.hh>

namespace mln
{

  namespace debug
  {

    /*! \brief Draw an image \p ima from a mln::p_graph \p pg, with
     *  value \p vertex_v for vertices, value \p edge_v for edges and 0 for
     *  the background.
     *
     * \param[in,out] ima      The image to be drawn.
     * \param[in]     pv       The p_vertices which contains vertices positions.
     * \param[in]     vertex_v   The value to assign to pixels which contains
     *                         vertices.
     * \param[in]     edge_v   The value to assign to pixels which contains
     *                         edges.
     */
    template <typename I, typename G, typename F>
    void
    draw_graph(Image<I>& ima, const p_vertices<G, F>& pv,
	  mln_value(I) vertex_v, mln_value(I) edge_v);

    /*! \brief Draw an image \p ima from a mln::graph_image \p gi.
     *
     * The background is filled with value 0.
     *
     * \param[in,out] ima     The image to be drawn.
     * \param[in]     gi      The graph_image which contains the vertices,
     *                        edges positions and the values of it.
     * \param[in]     edge_v  The value to assign to pixels which contains
     *                        edges, defaulting to 1.
     */
    // FIXME: The type of the last argument cannot always been
    // constructed from `int'!  We should remove this last argument.
//    template <typename I,  typename P, typename V>
//    void
//    draw_graph(Image<I>& ima, const graph_image<P, V>& gi,
//	 mln_value(I) edge_v = 1);

# ifndef MLN_INCLUDE_ONLY

    // FIXME: Add assertions on the size of the image: it must be large
    // enough to hold the representation of the graph/graph_image.

    template <typename I, typename G, typename F>
    inline
    void
    draw_graph(Image<I>& ima, const p_vertices<G, F>& pv,
	  mln_value(I) vertex_v, mln_value(I) edge_v)
    {
      // Debug the background.
      level::fill(ima, 0);

      // Debug the lines (edges).
      const G& g = pv.graph();
      typedef p_vertices<G, F> pv_t;
      mln_edge_iter(G) ei(g);
      for_all(ei)
	draw::line(exact(ima), pv(ei.v1()), pv(ei.v2()), edge_v);

      // Debug the points (vertices).
      mln_piter(pv_t) p(pv);
      for_all(p)
	exact(ima)(p) = vertex_v;
    }

/*
    template <typename I,  typename P, typename V>
    inline
    void
    draw_graph(Image<I>& ima, const graph_image<P, V>& gi,
	  mln_value(I) edge_v)
    {
      // Debug the background.
      level::fill(ima, 0);
      // Debug the lines (edges).
      for (unsigned l = 0; l < gi.domain().nedges(); ++l)
	line (exact(ima), gi.vertex1(l), gi.vertex2(l), edge_v);
      // Debug the points (vertices).
      for (unsigned p = 0; p < gi.domain().nvertices(); ++p)
	exact(ima)(gi.domain().point_from_id(p)) = gi.vertex_values()[p];
    }
*/

# endif // ! MLN_INCLUDE_ONLY

   } // end of namespace mln::debug

} // end of namespace mln

#endif // MLN_DEBUG_DRAW_GRAPH_HH
