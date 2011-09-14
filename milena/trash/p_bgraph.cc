// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/core/site_set/p_bgraph.cc
/// \brief Tests on mln::p_bgraph (the psite based on boost-graph).

#include <mln/core/site_set/p_bgraph.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/util/internal/boost_graph.hh>
#include <boost/typeof/std/utility.hpp>

struct empty {};

int main()
{
  /*--------------.
  | boost_graph.  |
  `--------------*/

  using namespace mln;
  typedef util::internal::boost_graph<point2d, util::empty>  Graph;

  // Make convenient labels for the vertices
  const int num_vertices = 5;

  // writing out the edges in the graph
  typedef std::pair<int, int> Edge;
  Edge edge_array[] = {
    Edge(0, 1), Edge(0, 3), Edge(2, 0), Edge(3, 2),
    Edge(2, 4), Edge(1, 3), Edge(3, 4)
  };

  const int num_edges = sizeof(edge_array)/sizeof(edge_array[0]);
  // declare a graph object
  Graph g(num_vertices);

  // add the edges to the graph object
  for (int i = 0; i < num_edges; ++i)
    boost::add_edge(edge_array[i].first, edge_array[i].second, g);

  g[0] = point2d(0, 0);
  g[1] = point2d(0, 1);
  g[2] = point2d(1, 0);
  g[3] = point2d(1, 1);
  g[4] = point2d(0, 2);

  /*-----------.
  | p_bgraph.  |
  `-----------*/

  /// Creation
  p_bgraph<point2d> pset(&g);

  /// Iterator
  p_bgraph_piter_<point2d> p(pset);

  for (p.start(); p.is_valid(); p.next())
    std::cout << p << std::endl;



  return 0;
}
