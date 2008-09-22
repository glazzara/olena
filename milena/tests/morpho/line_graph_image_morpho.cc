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

/// \file tests/morpho/line_graph_image_morpho.cc
/// \brief Tests on mln::line_graph_image with morphological filters.

#include <vector>

#include <mln/core/alias/point2d.hh>
#include <mln/core/image/line_graph_image.hh>
#include <mln/core/image/line_graph_elt_window.hh>
#include <mln/core/image/line_graph_window_piter.hh>

#include <mln/morpho/erosion.hh>
#include <mln/morpho/dilation.hh>


int main()
{
  using namespace mln;

  /*--------.
  | Graph.  |
  `--------*/

  /* The graph and its corresponding line graph are as follows:

            0 1 2 3 4               0 1 2 3 4
         .-----------	         .-----------
         |		         |	     
       0 |  0       2	       0 |  *       *
       1 |    \   / |	       1 |    0   1 |
       2 |      1   |	       2 |      *   4
       3 |       \  |	       3 |       2  |
       4 |        3-4	       4 |        *3*

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

  /*---------------------------.
  | Line graph image support.  |
  `---------------------------*/

  p_line_graph<point2d> plg(g);

  /*-------------------.
  | Line graph image.  |
  `-------------------*/

  // Values ("empty" vectors).
  std::vector<int> vertex_values(5);
  std::vector<int> edge_values(5);
  // FIXME: hand-made iota's.
  for (unsigned i = 0; i < vertex_values.size(); ++i)
    vertex_values[i] = i;
  for (unsigned i = 0; i < edge_values.size(); ++i)
    edge_values[i] = i;

  // Line graph image.
  /* FIXME: We probably don't want to build line_graph_images by hand;
     provide helpers and/or conversion functions.  */
  typedef line_graph_image<point2d, int> ima_t;
  ima_t ima(plg, vertex_values, edge_values);

  /*--------------------------.
  | Processing graph images.  |
  `--------------------------*/

  line_graph_image<point2d, int> ima_dil = morpho::dilation(ima, win);
  // Manual iteration over the domain of IMA_DIL.
  mln_piter_(ima_t) p_dil(ima_dil.domain());
  for_all (p_dil)
    std::cout << "ima_dil (" << p_dil << ") = " << ima_dil(p_dil) << std::endl;
  std::cout << std::endl;

  line_graph_image<point2d, int> ima_ero = morpho::erosion(ima, win);
  // Manual iteration over the domain of IMA_ERO.
  mln_piter_(ima_t) p_ero(ima_ero.domain());
  for_all (p_ero)
    std::cout << "ima_ero (" << p_ero << ") = " << ima_ero(p_ero) << std::endl;
}
