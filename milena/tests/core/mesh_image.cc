// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/// \file tests/core/mesh_image.cc
/// \brief Tests on mln::mesh_image.

#include <vector>

#include <mln/core/point2d.hh>
#include <mln/core/mesh_image.hh>
#include <mln/core/mesh_elt_window.hh>
#include <mln/core/mesh_window_piter.hh>

#include <mln/morpho/dilation.hh>

#include <mln/draw/mesh.hh>
#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>


int main()
{
  using namespace mln;

  /*--------.
  | Graph.  |
  `--------*/

  // Points associated to nodes.
  std::vector<point2d> points;
  points.push_back(make::point2d(0,0)); // Point associated to node 0.
  points.push_back(make::point2d(2,2)); // Point associated to node 1.
  points.push_back(make::point2d(0,4)); // Point associated to node 2.
  points.push_back(make::point2d(4,3)); // Point associated to node 3.
  points.push_back(make::point2d(4,4)); // Point associated to node 4.

  // Edges.
  util::graph<void> g;
  // Populate the graph with nodes.
  for (unsigned i = 0; i < points.size(); ++i)
    g.add_node ();
  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 2);

  /*-------.
  | Mesh.  |
  `-------*/

  mesh_p<point2d> mesh(g, points);

  /*-------------.
  | Mesh image.  |
  `-------------*/

  // Values ("empty" vector).
  std::vector<int> values(5);
  // Mesh image.
  typedef mesh_image<point2d, int> ima_t;
  ima_t ima(mesh, values);
  // Initialize values.
  debug::iota(ima);
  // Print the image.
  /* FIXME: Unfortunately, displaying mesh images is not easy right
     now (2007-12-19).  We could use 

       debug::println(ima);

     but there's not specialization working for mesh_image; the one
     selected by the compiler is based on a 2-D bbox, and expects the
     interface of mesh_image to work with points (not psites).
     Moreover, this iplementation only shows *values*, not the graph
     itslef.
 
     An alternative is to use draw::mesh (which, again, is misnamed),
     but it doesn't show the values, only the nodes and edges of the
     graph.

     The current solution is a mix between draw::mesh and hand-made
     iterations.  */
  image2d<int> ima_rep(ima.bbox());
  // We use the value 9 in draw::mesh instead of the default (which is
  // 1) to represent edges to distinguish it from nodes holding a
  // value of 1.
  draw::mesh (ima_rep, ima, 9);
  debug::println (ima_rep);


  /*------------.
  | Iterators.  |
  `------------*/

  // Manual iteration over the domain of IMA.
  mln_piter_(ima_t) p(ima.domain());
  for_all (p)
    std::cout << "ima (" << p << ") = " << ima(p) << std::endl;

  // Manual iterations over the neighborhoods of each point site of IMA.
  /* FIXME: In fact, this class should be named
     `mesh_elt_neighborhood' (there's no such thing as an elementary
     window on a mesh/graph!).  */
  typedef mesh_elt_window<point2d> win_t;
  win_t win;
  mln_qiter_(win_t) q(win, p);
  for_all (p)
  {
    std::cout << "neighbors of " << p << " (" << ima(p) << "), "
	      << "including the site itself:" << std::endl;
    for_all (q)
      std::cout << "  " << q << " (level = " << ima(q) << ")" << std::endl;
  }

  /*-------------------------.
  | Processing mesh images.  |
  `-------------------------*/

  mesh_image<point2d, int> ima_dil = morpho::dilation(ima, win);
  draw::mesh(ima_rep, ima_dil, 9);
  debug::println(ima_rep);

  mesh_image<point2d, int> ima_ero = morpho::erosion(ima, win);
  draw::mesh(ima_rep, ima_ero, 9);
  debug::println(ima_rep);
}
