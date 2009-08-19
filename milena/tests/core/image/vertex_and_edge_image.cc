// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <vector>

#include <mln/core/image/vertex_image.hh>
#include <mln/core/image/edge_image.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/util/graph.hh>



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

static const unsigned X = mln_max(unsigned); // Invalid id.


static unsigned expected_fwd_nb[5][3] = { { 0, X, X },
					  { 0, 1, 2 },
					  { 1, 4, X },
					  { 2, 3, X },
					  { 3, 4, X } };


static unsigned expected_bkd_nb[5][3] = { { 0, X, X },
					  { 2, 1, 0 },
					  { 4, 1, X },
					  { 3, 2, X },
					  { 4, 3, X } };


int main()
{
  using namespace mln;

  /*--------.
  | Graph.  |
  `--------*/

  // Points associated to vertices.
  typedef fun::i2v::array<point2d> fsite_t;
  fsite_t sites(5);
  sites(0) = point2d(0,0); // Point associated to vertex 0.
  sites(1) = point2d(2,2); // Point associated to vertex 1.
  sites(2) = point2d(0,4); // Point associated to vertex 2.
  sites(3) = point2d(4,3); // Point associated to vertex 3.
  sites(4) = point2d(4,4); // Point associated to vertex 4.

  // Edges.
  util::graph g;

  // Populate the graph with vertices.
  g.add_vertices(sites.size());

  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 2);

  //g.print_debug(std::cout);

  /*-------------.
  | Graph image.  |
  `-------------*/

  // Vertex values.
  typedef fun::i2v::array<unsigned> viota_t;
  viota_t iota(g.v_nmax());
  for (unsigned i = 0; i < iota.size(); ++i)
    iota(i) = 10 + i;


  // Edge values.
  typedef fun::i2v::array<unsigned> eiota_t;
  eiota_t eiota(g.e_nmax());
  for (unsigned i = 0; i < eiota.size(); ++i)
    eiota(i) = 20 + i;


  typedef vertex_image<void,unsigned> v_ima_t;
  v_ima_t v_ima(g, iota);

  typedef edge_image<void,unsigned> e_ima_t;
  e_ima_t e_ima(g, eiota);


  // Iterator over the vertex image.
  mln_piter_(v_ima_t) v(v_ima.domain());


  typedef graph_elt_window<util::graph,
    v_ima_t::domain_t,
    e_ima_t::domain_t> edge_win_t;
  edge_win_t win;

  // Forward Iteration
  {
    for_all(v)
    {
      int i = 0;
      // Iterator over the neighbor edges in the edge image.
      mln_qiter_(edge_win_t) e(win, e_ima.domain(), v);
      for_all(e)
      {
	mln_assertion(expected_fwd_nb[v.id()][i++] == e.id());
	mln_assertion((e.id() + 20) == e_ima(e));
      }
    }
  }

  // Backward Iteration
  {
    for_all(v)
    {
      int i = 0;
      // Iterator over the neighbor edges in the edge image.
      mln_bkd_qiter_(edge_win_t) e(win, e_ima.domain(), v);
      for_all(e)
      {
	mln_assertion(expected_bkd_nb[v.id()][i++] == e.id());
	mln_assertion((e.id() + 20) == e_ima(e));
      }
    }
  }



// FIXME: add tests for graph_window_if and graph_neighborhood_if when
// they support this feature.

}
