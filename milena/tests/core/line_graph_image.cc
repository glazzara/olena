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

/// \file tests/core/graph_image.cc
/// \brief Tests on mln::graph_image.

#include <vector>

#include <mln/core/point2d.hh>
#include <mln/core/line_graph_image.hh>
#include <mln/core/line_graph_elt_window.hh>
#include <mln/core/line_graph_window_piter.hh>

#include <mln/morpho/dilation.hh>

#include <mln/draw/graph.hh>
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
  util::graph<point2d> g;
  // Populate the graph with nodes.
  for (unsigned i = 0; i < points.size(); ++i)
    g.add_node (points[i]);
  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 2);

  /*-------.
  | Graph.  |
  `-------*/

  p_line_graph<point2d> plg(g);

  /*-------------------.
  | Line graph image.  |
  `-------------------*/

  // Values ("empty" vectors).
  std::vector<int> node_values(5);
  std::vector<int> edge_values(5);
  // FIXME: hand-made iota's.
  for (unsigned i = 0; i < node_values.size(); ++i)
    node_values[i] = i;
  for (unsigned i = 0; i < edge_values.size(); ++i)
    edge_values[i] = i;

  // Line graph image.
  /* FIXME: We probably don't want to build line_graph_images by hand;
     provide helpers and/or conversion functions.  */
  typedef line_graph_image<point2d, int> ima_t;
  ima_t ima(plg, node_values, edge_values);

  /*------------.
  | Iterators.  |
  `------------*/

  // Manual iteration over the domain of IMA.
  mln_piter_(ima_t) p(ima.domain());
  for_all (p)
    std::cout << "ima (" << p << ") = " << ima(p) << std::endl;

  // Manual iterations over the neighborhoods of each point site of IMA.
  typedef line_graph_elt_window<point2d> win_t;
  win_t win;
  mln_qiter_(win_t) q(win, p);
  for_all (p)
  {
    std::cout << "neighbors of " << p << " (" << ima(p) << "), "
	      << "including the site itself:" << std::endl;
    for_all (q)
      std::cout << "  " << q << " (level = " << ima(q) << ")"
		<< std::endl;
  }
  std::cout << std::endl;


//   /* FIXME: When implementing convert::to_line_graph_image, don't
//      forget to give a second argument defaulting to something like
//      fun::vv2v::max().  This second argument is a functor used to
//      compute the values of the edges of the line graph image.  */
//   image2d ima; // = ...
//   lg_ima_t lg_ima = convert::to_line_graph_image (ima);

//   // Image2d representation.
//   image2d<value_t> out = convert::to_image2d (lg_ima);
//   io::pgm::save(out, "out.pgm");

//   /* FIXME: Then, add some real processing on the line graph image
//      before converting to an image2d:
//      - erosion/dilation,
//      - Beucher's gradient,
//      - Meyer's WST (on the gradient of LG_IMA?),
//      - attribute filters (see my notes on Laurent's remarks about
//        attributes),
//      - etc.
//      Creating seperate tests for all these would be wise.  */
}
