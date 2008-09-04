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

/// \file tests/core/line_graph_image.cc
/// \brief Tests on the Watershed Transform on a mln::line_graph_image.

#include <vector>

#include <mln/core/alias/point2d.hh>
#include <mln/core/image/line_graph_image.hh>
#include <mln/core/image/line_graph_elt_neighborhood.hh>
#include <mln/core/image/line_graph_neighborhood_piter.hh>

#include <mln/morpho/meyer_wst.hh>

#include <mln/draw/graph.hh>
#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>


int main()
{
  using namespace mln;

  /*-------------.
  | Line graph.  |
  `-------------*/

  /* Actually this graph is from Jean Cousty's PhD thesis, page 76.

               0     1     2     3  (rows)
         ,------------------------
         |        0     10    5
       0 |     o-----o-----o-----o
         |     |     |     |     |
         |    2|    4|    6|    0|
         |     |     |     |     |
       1 |     o-----o-----o-----o
                  3     5     2
    (cols)

    In G, vertices and egdes are numbered following in the classical
    foward order.  */
  util::graph<point2d> g;

  // Vertices.
  std::vector<int> vertex_values;
  // We don't really care about values on vertices, because the algorithm
  // won't see them.  So all are set to 0.
  g.add_vertex (make::point2d(0, 0)); vertex_values.push_back (0); // Vertex 0.
  g.add_vertex (make::point2d(0, 1)); vertex_values.push_back (0); // Vertex 1.
  g.add_vertex (make::point2d(0, 2)); vertex_values.push_back (0); // Vertex 2.
  g.add_vertex (make::point2d(0, 3)); vertex_values.push_back (0); // Vertex 3.

  g.add_vertex (make::point2d(1, 0)); vertex_values.push_back (0); // Vertex 4.
  g.add_vertex (make::point2d(1, 1)); vertex_values.push_back (0); // Vertex 5.
  g.add_vertex (make::point2d(1, 2)); vertex_values.push_back (0); // Vertex 6.
  g.add_vertex (make::point2d(1, 3)); vertex_values.push_back (0); // Vertex 7.

  // Edges.
  std::vector<int> edge_values;
  g.add_edge (0, 1); edge_values.push_back (0);
  g.add_edge (1, 2); edge_values.push_back (10);
  g.add_edge (2, 3); edge_values.push_back (5);

  g.add_edge (0, 4); edge_values.push_back (2);
  g.add_edge (1, 5); edge_values.push_back (4);
  g.add_edge (2, 6); edge_values.push_back (6);
  g.add_edge (3, 7); edge_values.push_back (0);

  g.add_edge (4, 5); edge_values.push_back (3);
  g.add_edge (5, 6); edge_values.push_back (5);
  g.add_edge (6, 7); edge_values.push_back (2);

  // Line graph point set.
  p_line_graph<point2d> plg(g);

  // Line graph image.
  /* FIXME: We probably don't want to build line_graph_images by hand;
     provide helpers and/or conversion functions.  */
  typedef line_graph_image<point2d, int> ima_t;
  ima_t ima(plg, vertex_values, edge_values);

  /*------------.
  | Iterators.  |
  `------------*/

  // Manual iteration over the domain of IMA.
  mln_piter_(ima_t) p(ima.domain());
  for_all (p)
    std::cout << "ima (" << p << ") = " << ima(p) << std::endl;
  std::cout << std::endl;

  typedef line_graph_elt_neighborhood<point2d> nbh_t;
  nbh_t nbh;

  typedef unsigned wst_val_t;
  wst_val_t nbasins;
  typedef line_graph_image<point2d, wst_val_t> wst_ima_t;
  wst_ima_t wshed = morpho::meyer_wst(ima, nbh, nbasins);
  std::cout << "nbasins = " << nbasins << std::endl;

  // Manual iteration over the domain of WSHED.
  mln_piter_(wst_ima_t) pw(wshed.domain());
  for_all (pw)
    std::cout << "wshed (" << pw << ") = " << wshed(pw) << std::endl;
}
