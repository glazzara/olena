// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

/// \file   tests/util/graph.cc
/// \brief  test of mln::util::graph
///         This test is a copy of the BGL quick tours example.

#include <mln/util/internal/boost_graph.hh>
#include <iostream>

using namespace mln::util::internal;
using namespace boost;

struct empty {};

template <class Graph>
struct exercise_vertex
{
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  exercise_vertex(Graph& g_) : g(g_) {}

  void operator()(const Vertex& v) const
  {
    typedef graph_traits<Graph> GraphTraits;
    typename property_map<Graph, vertex_index_t>::type
      index = get(vertex_index, g);

    std::cout << "out-edges: ";
    typename GraphTraits::out_edge_iterator out_i, out_end;
    typename GraphTraits::edge_descriptor e;
    for (tie(out_i, out_end) = out_edges(v, g); out_i != out_end; ++out_i)
    {
      e = *out_i;
      Vertex src = source(e, g), targ = target(e, g);
      std::cout << "(" << index[src] << "," << index[targ] << ") ";
    }
    std::cout << std::endl;

    std::cout << "adjacent vertices: ";
    typename graph_traits<Graph>::adjacency_iterator ai;
    typename graph_traits<Graph>::adjacency_iterator ai_end;
    for (tie(ai, ai_end) = adjacent_vertices(v, g); ai != ai_end; ++ai)
      std::cout << index[*ai] <<  " ";
    std::cout << std::endl;
  }

  Graph& g;
};

int main ()
{

  typedef boost_graph<empty, empty>  Graph;

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
  typedef property_map<Graph, vertex_index_t>::type IndexMap;
  IndexMap index = get(vertex_index, g);

  // add the edges to the graph object
  for (int i = 0; i < num_edges; ++i)
    add_edge(edge_array[i].first, edge_array[i].second, g);

  std::cout << "vertices(g) = ";
  typedef graph_traits<Graph>::vertex_iterator vertex_iter;
  std::pair<vertex_iter, vertex_iter> vp;
  for (vp = vertices(g); vp.first != vp.second; ++vp.first)
    std::cout << index[*vp.first] <<  " ";
  std::cout << std::endl;

  std::cout << "edges(g) = ";
  graph_traits<Graph>::edge_iterator ei, ei_end;
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
    std::cout << "(" << index[source(*ei, g)]
	      << "," << index[target(*ei, g)] << ") ";
  std::cout << std::endl;

  std::for_each(vertices(g).first, vertices(g).second,
		exercise_vertex<Graph>(g));
  return 0;
}
