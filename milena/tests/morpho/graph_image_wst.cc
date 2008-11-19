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

/// \file tests/morpho/graph_image_wst.cc
/// \brief Tests on the Watershed Transform on a mln::graph_image.

#include <vector>

#include <mln/core/alias/point2d.hh>
#include <mln/core/image/graph_image.hh>
#include <mln/core/image/graph_elt_neighborhood.hh>
#include <mln/core/image/graph_neighborhood_piter.hh>

#include <mln/morpho/meyer_wst.hh>


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
       4 |        3-0

  */

  // Points associated to vertices.
  std::vector<point2d> points;
  points.push_back(point2d(0,0)); // Point associated to vertex 0.
  points.push_back(point2d(2,2)); // Point associated to vertex 1.
  points.push_back(point2d(0,4)); // Point associated to vertex 2.
  points.push_back(point2d(4,3)); // Point associated to vertex 3.
  points.push_back(point2d(4,4)); // Point associated to vertex 4.

  // Edges.
  util::graph<point2d> g;
  // Populate the graph with vertices.
  for (unsigned i = 0; i < points.size(); ++i)
    g.add_vertex (points[i]);
  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 2);

  /*-------.
  | Graph.  |
  `-------*/

  p_graph<point2d> pg(g);

  /*-------------.
  | Graph image.  |
  `-------------*/

  // Values.
  std::vector<int> values(5);
  values[0] = 0; // Value associated to vertex 0.
  values[1] = 1; // Value associated to vertex 1.
  values[2] = 2; // Value associated to vertex 2.
  values[3] = 3; // Value associated to vertex 3.
  values[4] = 0; // Value associated to vertex 4.
  // Graph image.
  typedef graph_image<point2d, int> ima_t;
  ima_t ima(pg, values);

  /*------.
  | WST.  |
  `------*/

  typedef graph_elt_neighborhood<point2d> nbh_t;
  nbh_t nbh;

  typedef unsigned wst_val_t;
  wst_val_t nbasins;
  typedef graph_image<point2d, wst_val_t> wst_ima_t;
  wst_ima_t wshed = morpho::meyer_wst(ima, nbh, nbasins);
  std::cout << "nbasins = " << nbasins << std::endl;

  // Manual iteration over the domain of WSHED.
  mln_piter_(wst_ima_t) pw(wshed.domain());
  for_all (pw)
    std::cout << "wshed (" << pw << ") = " << wshed(pw) << std::endl;
}
