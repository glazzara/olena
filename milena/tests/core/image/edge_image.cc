// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory (LRDE)
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

/// \file
/// \brief Exercise mln::edge_image.
///
/// This test is similar to tests/core/image/line_graph_image.cc
/// (apart from tge visualisation part).  (We might want to factor
/// them.)

#include <mln/core/image/image2d.hh>

#include <mln/util/graph.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/util/site_pair.hh>
#include <mln/core/image/edge_image.hh>

#include <mln/accu/shape/bbox.hh>
#include <mln/debug/draw_graph.hh>


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

     Numbers in the graph represent the vertices and edges identifers
     (resp.).  */

  // Graph.
  util::graph g;

  // Populate the graph with 5 vertices.
  g.add_vertices(5);

  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 2);

  // Sites (points) associated to edges.
  /* FIXME: It would be nicer to deduce these pairs from points
     attached to a vertex graph (image) rather than building them
     manually.  */
  typedef util::site_pair<point2d> site_t;
  typedef fun::i2v::array<site_t> fsite_t;
  fsite_t sites(5);
  sites(0) = site_t(point2d(0,0), point2d(2,2)); // Site associated to edge 0.
  sites(1) = site_t(point2d(2,2), point2d(0,4)); // Site associated to edge 1.
  sites(2) = site_t(point2d(2,2), point2d(4,3)); // Site associated to edge 2.
  sites(3) = site_t(point2d(4,3), point2d(4,4)); // Site associated to edge 3.
  sites(4) = site_t(point2d(4,4), point2d(0,4)); // Site associated to edge 4.

  /*-------------.
  | Edge image.  |
  `-------------*/

  // Graph values.
  typedef fun::i2v::array<unsigned> viota_t;
  viota_t iota(g.v_nmax());
  for (unsigned i = 0; i < iota.size(); ++i)
    iota(i) = 10 + i;

  typedef edge_image<site_t,unsigned> ima_t;
  ima_t ima(g, sites, iota);

  // FIXME: As of 2011-10-06, this visualization does not work.
  {
    // FIXME: Move this part to a special test case.

    // Compute the bounding box of 'ima'.
    accu::shape::bbox<point2d> a;
    mln_piter_(ima_t) p(ima.domain());
    for_all(p)
    {
      a.take(p.first());
      a.take(p.second());
    }
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
     itself.

     An alternative is to use debug::graph,
     but it doesn't show the values, only the vertices and edges of the
     graph.

     The current solution is a mix between debug::graph and hand-made
     iterations.  */
    image2d<int> ima_rep(bbox);

    // We use the value 9 in debug::graph to represent edges to distinguish it
    // from vertices holding a value of 1.
    debug::draw_graph(ima_rep, ima.domain(), 1, 9);
  }

  /*------------.
  | Iterators.  |
  `------------*/

  // Invalid identifier.
  const unsigned X = mln_max(unsigned);

  // Expected neighbors for forward and backward iterations on an
  // elementary window.
  const unsigned expected_fwd_nbh[5][3] = { { 1, 2, X },
					    { 0, 2, 4 },
					    { 0, 1, 3 },
					    { 2, 4, X },
					    { 1, 3, X } };

  const unsigned expected_bkd_nbh[5][3] = { { 2, 1, X },
					    { 4, 2, 0 },
					    { 3, 1, 0 },
					    { 4, 2, X },
					    { 3, 1, X } };

  // Iteration over the domain of IMA.
  mln_piter_(ima_t) p(ima.domain());
  unsigned i = 10;
  for_all(p)
    mln_assertion(ima(p) == i++);

  // Elementary window.
  typedef ima_t::win_t win_t;
  win_t win;

  {
    // Window -- Forward iteration.
    mln_fwd_qiter_(win_t) q(win, p);
    for_all(p)
    {
      i = 0;
      for_all(q)
      {
	mln_assertion(expected_fwd_nbh[p.id()][i] == q.id());
	++i;
      }
    }
  }

  {
    // Window -- Backward iteration.
    mln_bkd_qiter_(win_t) q(win, p);
    for_all(p)
    {
      i = 0;
      for_all(q)
      {
	mln_assertion(expected_bkd_nbh[p.id()][i] == q.id());
	++i;
      }
    }
  }


  // Elementary neighborhood.
  typedef ima_t::nbh_t nbh_t;
  nbh_t nbh;

  {
    // Neighborhood -- Forward iteration.
    mln_fwd_niter_(nbh_t) n(nbh, p);
    for_all(p)
    {
      i = 0;
      for_all(n)
      {
	mln_assertion(expected_fwd_nbh[p.id()][i] == n.id());
	++i;
      }
    }
  }

  {
    // Neighborhood -- Backward iteration.
    mln_bkd_niter_(nbh_t) n(nbh, p);
    for_all(p)
    {
      i = 0;
      for_all(n)
      {
	mln_assertion(expected_bkd_nbh[p.id()][i] == n.id());
	++i;
      }
    }
  }
}
