// Copyright (C) 2007, 2008, 2009, 2011, 2012 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_DEBUG_DRAW_GRAPH_HH
# define MLN_DEBUG_DRAW_GRAPH_HH

/// \file
///
/// \brief Draw an (classical) image from a mln::graph_image.
///
/// \todo write a version for graph images.

# include <mln/core/site_set/p_vertices.hh>
# include <mln/core/site_set/p_edges.hh>
# include <mln/util/line_graph.hh>
# include <mln/util/site_pair.hh>
# include <mln/draw/line.hh>
# include <mln/draw/box_plain.hh>
# include <mln/data/fill.hh>
# include <mln/metal/equal.hh>

namespace mln
{

  namespace debug
  {

    /*! \brief Draw an image \p ima from a mln::p_vertices \p pv.

      Colors for vertices are defined through \p vcolor. Colors for
      edges are defined though \p ecolor. Background is set to 0.

       \param[in,out] ima      The image to be drawn.
       \param[in]     pv       The p_vertices which contains vertices positions.
       \param[in]     vcolor The value to assign to pixels which contains
      				vertices.
       \param[in]     ecolor   The value to assign to pixels which contains
                               edges.

       \ingroup mlndebug
     */
    template <typename I, typename G, typename F>
    void
    draw_graph(Image<I>& ima, const p_vertices<G, F>& pv,
	       mln_value(I) vcolor, mln_value(I) ecolor);


    /*! \brief Draw an image \p ima from a mln::p_vertices \p pv.

	Colors for vertices are defined through \p vcolor_f_. Colors
        for edges are defined though \p ecolor_f_. Vertices are drawn
        with squares of width \p vwidth.

       \param[in,out] ima       The image to be drawn.
       \param[in]     pv        The p_vertices which contains vertices positions.
       \param[in]     vcolor_f  A function returning a color value for vertices.
       \param[in]     ecolor_f  A function returning a color value for edges.
       \param[in]     vwidth    Width of the squares representing the vertices.

       \ingroup mlndebug
     */
    template <typename I, typename G, typename F, typename V, typename E>
    void
    draw_graph(Image<I>& ima,
	       const p_vertices<G, F>& pv,
	       const Function<V>& vcolor_f, const Function<E>& ecolor_f,
	       unsigned vwidth);


    /// \overload
    template <typename I, typename G, typename F, typename V, typename E>
    inline
    void
    draw_graph(Image<I>& ima_,
	       const p_vertices<G, F>& pv,
	       const Function<V>& vcolor_f_, const Function<E>& ecolor_f_);


    /*! \brief Draw an image \p ima from a mln::p_vertices \p pv.

      Colors for vertices are defined through \p vcolor_f_. Colors for
      edges are defined though \p ecolor_f_.

       \param[in,out] ima       The image to be drawn.
       \param[in]     pv        The p_vertices which contains vertices positions.
       \param[in]     vcolor_f  A function returning a color value for vertices.
       \param[in]     ecolor_f  A function returning a color value for edges.

       \ingroup mlndebug
     */
    template <typename I, typename G, typename F, typename V, typename E>
    inline
    void
    draw_graph(Image<I>& ima,
	       const p_vertices<util::line_graph<G>, F>& pv,
	       const Function<V>& vcolor_f, const Function<E>& ecolor_f);


# ifndef MLN_INCLUDE_ONLY



    // FIXME: Add assertions on the size of the image: it must be large
    // enough to hold the representation of the graph/graph_image.

    template <typename I, typename G, typename F>
    inline
    void
    draw_graph(Image<I>& ima_,
	       const p_edges<G, F>& pe,
	       mln_value(I) vcolor,
	       mln_value(I) ecolor)
    {
      mln_trace("debug::draw_graph");

      I& ima = exact(ima_);
      mln_precondition(ima.is_valid());

      // Draw edges.
      typedef p_edges<G, F> pe_t;
      mln_piter(pe_t) p(pe);
      for_all(p)
      {
	if (ima.has(p.first()) && ima.has(p.second()))
	  draw::line(ima, p.first(), p.second(), ecolor);
	if (ima.has(p.first()))
	  ima(p.first()) = vcolor;
	if (ima.has(p.second()))
	  ima(p.second()) = vcolor;
      }

    }


    template <typename I, typename G, typename F>
    inline
    void
    draw_graph(Image<I>& ima_,
	       const p_vertices<G, F>& pv,
	       mln_value(I) vcolor,
	       mln_value(I) ecolor)
    {
      mln_trace("debug::draw_graph");

      I& ima = exact(ima_);
      mln_precondition(ima.is_valid());

      // Draw edges.
      const G& g = pv.graph();
      typedef p_vertices<G, F> pv_t;
      mln_edge_iter(G) ei(g);
      for_all(ei)
	draw::line(ima, pv(ei.v1()), pv(ei.v2()), ecolor);

      // Draw vertices.
      mln_piter(pv_t) p(pv);
      for_all(p)
	if (ima.has(p))
	  ima(p) = vcolor;

    }


    // FIXME: Refactor + be more restrictive on the function type.
    template <typename I, typename G, typename F, typename V, typename E>
    inline
    void
    draw_graph(Image<I>& ima_,
	       const p_vertices<G, F>& pv,
	       const Function<V>& vcolor_f_, const Function<E>& ecolor_f_,
	       unsigned vwidth)
    {
      mln_trace("debug::draw_graph");

      I& ima = exact(ima_);
      const V& vcolor_f = exact(vcolor_f_);
      const E& ecolor_f = exact(ecolor_f_);

      mln_precondition(ima.is_valid());

      // Draw edges.
      const G& g = pv.graph();
      typedef p_vertices<G, F> pv_t;
      mln_edge_iter(G) ei(g);
      for_all(ei)
	if (ei.v1() != 0 && ei.v2() != 0)
	  draw::line(ima, pv(ei.v1()), pv(ei.v2()), ecolor_f(ei.id()));

      // Draw vertices.
      unsigned vhalf_width = vwidth / 2;
      mln_piter(pv_t) p(pv);
      for_all(p)
	if (ima.has(p) && p.id() != 0u)
	{
	  box2d box(p + dpoint2d(-vhalf_width, -vhalf_width),
		    p + dpoint2d(+vhalf_width, +vhalf_width));
	  box.crop_wrt(ima.domain());
	  draw::box_plain(ima, box, vcolor_f(p.id()));
	}

    }


    template <typename I, typename G, typename F, typename V, typename E>
    inline
    void
    draw_graph(Image<I>& ima,
	       const p_vertices<G, F>& pv,
	       const Function<V>& vcolor_f, const Function<E>& ecolor_f)
    {
      draw_graph(ima, pv, vcolor_f, ecolor_f, 5);
    }


    // FIXME: Refactor + be more restrictive on the function type.
    template <typename I, typename G, typename F, typename V, typename E>
    inline
    void
    draw_graph(Image<I>& ima_,
	       const p_vertices<util::line_graph<G>, F>& pv,
	       const Function<V>& vcolor_f_, const Function<E>& ecolor_f_)
    {
      mln_trace("debug::draw_graph");

      I& ima = exact(ima_);
      const V& vcolor_f = exact(vcolor_f_);
      const E& ecolor_f = exact(ecolor_f_);

      mln_precondition(ima.is_valid());

      typedef util::line_graph<G> LG;

      const LG& lg = pv.graph();
      const G& g = lg.graph();
      typedef p_vertices<LG, F> pv_t;
      mln_vertex_iter(LG) vi(lg);
      for_all(vi)
      {
	p_line2d l = pv(vi.id());
	// Draw edges (Line graph vertices).
	draw::line(ima, l.begin(), l.end(), ecolor_f(vi.id()));

	// Draw vertices (graph vertices).
	if (ima.has(l.begin()))
	  ima(l.begin()) = vcolor_f(g.edge(vi).v1());
	if (ima.has(l.end()))
	  ima(l.end()) = vcolor_f(g.edge(vi).v2());
      }

    }

# endif // ! MLN_INCLUDE_ONLY

   } // end of namespace mln::debug

} // end of namespace mln

#endif // ! MLN_DEBUG_DRAW_GRAPH_HH
