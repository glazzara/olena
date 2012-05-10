// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#include <mln/core/alias/point2d.hh>

#include <mln/fun/i2v/array.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/core/site_set/p_set.hh>
#include <mln/core/site_set/p_vertices.hh>
#include <mln/util/graph.hh>


int main()
{
  using namespace mln;

  /*--------------------------------------------------------.
  | Compatibility of mln::graph_psite with mln::p_array and |
  | mln::p_array_piter.                                     |
  `--------------------------------------------------------*/

  // Graph

  //Vertices
  typedef fun::i2v::array<point2d> fpoint_t;
  fpoint_t points(5);
  points(0) = point2d(0,0); // Point associated to vertex 0.
  points(1) = point2d(2,2); // Point associated to vertex 1.
  points(2) = point2d(0,4); // Point associated to vertex 2.
  points(3) = point2d(4,3); // Point associated to vertex 3.
  points(4) = point2d(4,4); // Point associated to vertex 4.

  // Edges.
  util::graph g;
  // Populate the graph with vertices.
  for (unsigned i = 0; i < points.size(); ++i)
    g.add_vertex();
  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 2);



  // Graph point set.
  typedef p_vertices<util::graph, fpoint_t> pv_t;
  typedef mln_psite_(pv_t) gpsite_t;
  pv_t pv(g, points);

  {
    // Array of graph point sites.
    typedef p_array<gpsite_t> pa_t;
    pa_t pa;

    // Tests: copying all psites from PG to PA.
    mln_piter_(pv_t) p(pv);
    for_all (p)
      pa.append(p);

    // Test: create and use an iterator over PA.
    mln_piter_(pa_t) p2(pa);
    for_all (p2)
      std::cout << p2 << ' ';
    std::cout << std::endl;
  }

  {
    // Set of graph point sites.
    typedef p_set<gpsite_t> ps_t;
    ps_t ps;

    // Tests: copying all psites from PG to PS.
    mln_piter_(pv_t) p(pv);
    for_all (p)
      ps.insert(p);

    // Test: create and use an iterator over PS.
    mln_piter_(ps_t) p2(ps);
    for_all (p2)
      std::cout << p2 << ' ';
    std::cout << std::endl;
  }
}
