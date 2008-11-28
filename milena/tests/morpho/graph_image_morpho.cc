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

/// \file tests/morpho/graph_image_morpho.cc
/// \brief Tests on mln::graph_image with morphological filters.

#include <vector>

#include <mln/accu/bbox.hh>
#include <mln/core/alias/box2d.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/image/graph_elt_window.hh>
#include <mln/core/image/graph_window_piter.hh>
#include <mln/core/var.hh>

#include <mln/morpho/dilation.hh>
#include <mln/morpho/erosion.hh>

#include <mln/util/graph.hh>

#include <mln/pw/all.hh>

#include <mln/debug/draw_graph.hh>
#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>


template <typename S>
struct vertex_site_value_t : public mln::Function_p2v< vertex_site_value_t<S> >
{
  typedef unsigned result;

  unsigned
  operator()(const mln_psite(S)& p) const
  {
    return v_[p.v().id()];
  }

  protected:
    std::vector<result> v_;
};


int main()
{
  using namespace mln;

  /*--------.
  | Graph.  |
  `--------*/

  /* The graph is as follows:

            0 1 2 3 4
         .-----------
         |
       0 |  0       2
       1 |    \   / |
       2 |      1   |
       3 |       \  |
       4 |        3-4

  */

  // Points associated to vertices.
  typedef fun::i2v::array<point2d> F;
  F points(5);
  points(0) = point2d(0,0); // Point associated to vertex 0.
  points(1) = point2d(2,2); // Point associated to vertex 1.
  points(2) = point2d(0,4); // Point associated to vertex 2.
  points(3) = point2d(4,3); // Point associated to vertex 3.
  points(4) = point2d(4,4); // Point associated to vertex 4.

  // Edges.
  typedef mln::util::graph G;
  G g;
  // Populate the graph with vertices.
  for (unsigned i = 0; i < points.size(); ++i)
    g.add_vertex ();
  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 2);

  /*----------------------.
  | Graph image support.  |
  `----------------------*/

  p_vertices<G, F> pg(g, points);

  /*--------------.
  | Graph image.  |
  `--------------*/

  vertex_site_value_t< p_vertices<G, F> > values;
  // Graph image.
  mln_VAR(ima, (values | pg));
  // Initialize values.
  debug::iota(ima);

  /*-------------------------------------.
  | Image representation/visualization.  |
  `-------------------------------------*/

  // Compute the bounding box of IMA.
  /* FIXME: mln::graph_image should automatically feature a bbox when
     its parameter P is akin to a point.  */
  accu::bbox<point2d> a;
  for (unsigned i = 0; i < points.size(); ++i)
    a.take(points(i));
  box2d bbox = a.to_result();
  // Print the image.
  /* FIXME: Unfortunately, displaying graph images is not easy right
     now (2008-02-05).  We could use 

       debug::println(ima);

     but there's not specialization working for graph_image; the one
     selected by the compiler is based on a 2-D bbox, and expects the
     interface of graph_image to work with points (not psites).
     Moreover, this implementation only shows *values*, not the graph
     itslef.
 
     An alternative is to use draw::graph (which, again, is misnamed),
     but it doesn't show the values, only the vertices and edges of the
     graph.

     The current solution is a mix between draw::graph and hand-made
     iterations.  */
  image2d<int> ima_rep(bbox);

  /*--------------------------.
  | Processing graph images.  |
  `--------------------------*/

  typedef graph_elt_window<G, F> win_t;
  win_t win;

  image2d<unsigned> ima_dil = morpho::dilation(ima, win);
//  debug::draw_graph(ima_rep, ima_dil, 9);
//  debug::println(ima_rep);

  image2d<unsigned> ima_ero = morpho::erosion(ima, win);
//  draw::graph(ima_rep, ima_ero, 9);
//  debug::println(ima_rep);
}
