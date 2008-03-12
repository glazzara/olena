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

#ifndef MLN_DRAW_GRAPH_HH
# define MLN_DRAW_GRAPH_HH

/// \file mln/draw/graph.hh
/// \brief Draw an (classical) image from a mln::graph_image.

# include <mln/pw/image.hh>
# include <mln/level/fill.hh>
# include <mln/draw/line.hh>
# include <mln/core/p_graph.hh>
# include <mln/core/graph_image.hh>

namespace mln
{

  namespace draw
  {
    /* FIXME: `draw::graph' is not a good name.  These functions do not
       actually _draw_ the graph/graph_image; it *converts* it to a
       printable image.  These functions should be put elsewhere
       (e.g., in debug::).  */

    /*! \brief Draw an image \p ima from a mln::p_graph \p pg, with
     *  value \p node_v for nodes, value \p link_v for links and 0 for
     *  the background.
     *
     * \param[in,out] ima      The image to be drawn.
     * \param[in]     pg       The p_graph which contains nodes and links
     *                         positions.
     * \param[in]     node_v   The value to assign to pixels which contains
     *                         nodes.
     * \param[in]     link_v   The value to assign to pixels which contains
     *                         links.
     */
    template <typename I, typename P>
    void
    graph(Image<I>& ima, const p_graph<P>& pg,
	  mln_value(I) node_v, mln_value(I) link_v);

    /*! \brief Draw an image \p ima from a mln::graph_image \p gi.
     *
     * The background is filled with value 0.
     *
     * \param[in,out] ima     The image to be drawn.
     * \param[in]     gi      The graph_image which contains the nodes, links
     *                        positions and the values of it.
     * \param[in]     link_v  The value to assign to pixels which contains
     *                        links, defaulting to 1.
     */
    // FIXME: The type of the last argument cannot always been
    // constructed from `int'!  We should remove this last argument.
    template <typename I,  typename P, typename V>
    void
    graph(Image<I>& ima, const graph_image<P, V>& gi,
	 mln_value(I) link_v = 1);

# ifndef MLN_INCLUDE_ONLY

    // FIXME: Add assertions on the size of the image: it must be large
    // enough to hold the representation of the graph/graph_image.

    template <typename I, typename P>
    inline
    void
    graph(Image<I>& ima, const p_graph<P>& pg,
	  mln_value(I) node_v, mln_value(I) link_v)
    {
      // Draw the background.
      level::fill(ima, 0);
      // Draw the lines (edges).
      for (size_t l = 0; l < pg.nlines(); ++l)
	line (exact(ima),
	      // FIXME: Too low-level.  See similar remarks
	      // in mln/core/graph_image.hh
	      pg.gr_.node_data(pg.gr_.edge(l).n1()),
	      pg.gr_.node_data(pg.gr_.edge(l).n2()),
	      link_v);
      // Draw the points (nodes).
      for (size_t p = 0; p < pg.npoints(); ++p)
 	exact(ima)(pg.gr_.node_data(p)) = node_v;
    }

    template <typename I,  typename P, typename V>
    inline
    void
    graph(Image<I>& ima, const graph_image<P, V>& gi,
	  mln_value(I) link_v)
    {
      // Draw the background.
      level::fill(ima, 0);
      // Draw the lines (edges).
      for (size_t l = 0; l < gi.domain().nedges(); ++l)
	line (exact(ima), gi.node1(l), gi.node2(l), link_v);
      // Draw the points (nodes).
      for (size_t p = 0; p < gi.domain().nnodes(); ++p)
 	exact(ima)(gi.domain().point_from_id(p)) = gi.node_values()[p];
    }

# endif // ! MLN_INCLUDE_ONLY

   } // end of namespace mln::draw

} // end of namespace mln

#endif // MLN_GRAPH_PSITE_HH
