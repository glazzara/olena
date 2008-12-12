// Copyright(C) 2007, 2008 EPITA Research and Development Laboratory(LRDE)
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

/// \file tests/core/image/graph_image.cc
///
/// Tests on mln::graph_image.

#include <vector>

#include <mln/core/site_set/p_vertices.hh>
#include <mln/core/image/graph_elt_window.hh>
#include <mln/core/concept/function.hh>
#include <mln/core/neighb.hh>
#include <mln/core/var.hh>

#include <mln/accu/bbox.hh>

#include <mln/fun/i2v/array.hh>

#include <mln/util/graph.hh>

#include <mln/debug/draw_graph.hh>
#include <mln/debug/println.hh>

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


// Expected neighbors for forward and backward iteration.
// -1 is an invalid id.
static unsigned expected_fwd_nb[5][3] = { { 1, -1, -1 },
					  { 0,  2,  3 },
					  { 1,  4, -1 },
					  { 1,  4, -1 },
					  { 3,  2, -1 } };

static unsigned expected_bkd_nb[5][3] = { { 1, -1, -1 },
					  { 3,  2,  0 },
					  { 4,  1, -1 },
					  { 4,  1, -1 },
					  { 2,  3, -1 } };


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
  for(unsigned i = 0; i < sites.size(); ++i)
    g.add_vertex();
  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 2);

  //g.print_debug(std::cout);

  /*----------------------.
  | Graph image support.  |
  `----------------------*/

  typedef p_vertices<util::graph, fsite_t> S;
  S pv(g, sites);

  /*-------------.
  | Graph image.  |
  `-------------*/

  // Graph values.
  typedef fun::i2v::array<unsigned> viota_t;
  viota_t iota(pv.nsites());
  for(unsigned i = 0; i < iota.size(); ++i)
    iota(i) = 10 + i;

  // Create graph image.
  mln_const_VAR(ima,(iota | pv));

  {
    // FIXME: Move this part to a special test case.

    // Compute the bounding box of 'ima'.
    accu::bbox<point2d> a;
    mln_piter_(ima_t) p(ima.domain());
    for_all(p)
      a.take(p);
    box2d bbox = a.to_result();
    mln_assertion(bbox == make::box2d(5, 5));

  // Print the image.
  /* FIXME: Unfortunately, displaying graph images is not easy right
     now(2008-02-05).  We could use

       debug::println(ima);

     but there's not specialization working for graph_image; the one
     selected by the compiler is based on a 2-D bbox, and expects the
     interface of graph_image to work with points(not psites).
     Moreover, this implementation only shows *values*, not the graph
     itslef.

     An alternative is to use debug::graph,
     but it doesn't show the values, only the vertices and edges of the
     graph.

     The current solution is a mix between debug::graph and hand-made
     iterations.  */
    image2d<int> ima_rep(bbox);

  // We use the value 9 in debug::graph to represent edges to distinguish it
  // from vertices holding a value of 1.
    debug::draw_graph(ima_rep, pv, 1, 9);
  }

  /*------------.
  | Iterators.  |
  `------------*/

  // iteration over the domain of IMA.
  mln_piter_(ima_t) p(ima.domain());
  unsigned i = 10;
  for_all(p)
    mln_assertion(ima(p) == i++);

  typedef graph_elt_window<util::graph, fsite_t> win_t;
  win_t win;

  {
    // Window - Forward iteration
    mln_qiter_(win_t) q(win, p);
    for_all(p)
    {
      i = 0;
      for_all(q)
      {
	mln_assertion(expected_fwd_nb[p.id()][i] == q.id());
	++i;
      }
    }
  }

  {
    // Window - Backward iteration
    mln_bkd_qiter_(win_t) q(win, p);
    for_all(p)
    {
      i = 0;
      for_all(q)
      {
	mln_assertion(expected_bkd_nb[p.id()][i] == q.id());
	++i;
      }
    }
  }

  typedef neighb<win_t> neighb_t;
  neighb_t neigh(win);
  {
    // Neighborhood - Forward iteration
    mln_niter_(neighb_t) n(neigh, p);

    for_all(p)
    {
      i = 0;
      for_all(n)
      {
	mln_assertion(expected_fwd_nb[p.id()][i] == n.id());
	++i;
      }
    }
  }

  {
    // Neighborhood - Backward iteration
    mln_bkd_niter_(neighb_t) n(neigh, p);
    for_all(p)
    {
      i = 0;
      for_all(n)
      {
	mln_assertion(expected_bkd_nb[p.id()][i] == n.id());
	++i;
      }
    }
  }
}
