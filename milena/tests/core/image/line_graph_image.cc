// Copyright (C) 2009
 EPITA Research and Development Laboratory (LRDE)
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

/// Required for line graph images.
#include <mln/core/site_set/p_edges.hh>
#include <mln/core/image/graph_elt_window.hh>
#include <mln/core/image/graph_elt_neighborhood.hh>
#include <mln/core/image/vertex_image.hh>
#include <mln/core/neighb.hh>
#include <mln/core/var.hh>
#include <mln/pw/all.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/util/graph.hh>
#include <mln/make/vertex_image.hh>


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


static const unsigned X = mln_max(unsigned); // Invalid id.


// Expected neighbors for forward and backward iteration.
static unsigned expected_fwd_nbh[5][3] = { { 1, X, X },
					   { 0, 2, 3 },
					   { 1, 4, X },
					   { 1, 4, X },
					   { 3, 2, X } };

static unsigned expected_bkd_nbh[5][3] = { { 1, 0, X },
					   { 3, 2, 0 },
					   { 4, 1, X },
					   { 4, 1, X },
					   { 2, 3, X } };


int main()
{
  using namespace mln;

  /*--------.
  | Graph.  |
  `--------*/

  // Sites associated to edges.
  typedef fun::i2v::array<point2d> fsite_t;
  fsite_t sites(5);
  sites(0) = point2d(0,0); // Point associated to edge 0.
  sites(1) = point2d(2,2); // Point associated to edge 1.
  sites(2) = point2d(0,4); // Point associated to edge 2.
  sites(3) = point2d(4,3); // Point associated to edge 3.
  sites(4) = point2d(4,4); // Point associated to edge 4.

  util::graph g;

  // Populate the graph with vertices.
  g.add_vertices(sites.size());

  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 2);

  /*-------------------.
  | Line graph image.  |
  `-------------------*/

  // Graph values.
  typedef fun::i2v::array<unsigned> viota_t;
  viota_t iota(g.v_nmax());
  for (unsigned i = 0; i < iota.size(); ++i)
    iota(i) = 10 + i;

  // Create line graph image.
  typedef vertex_image<point2d,unsigned> ima_t;
  ima_t ima = make::vertex_image(g, sites, iota);

  /*------------.
  | Iterators.  |
  `------------*/

  mln_edge_iter_(util::graph) ei(g);
  mln_edge_nbh_edge_iter_(util::graph) en(ei);

  // Manual iteration over the domain of IMA.
  mln_piter_(ima_t) p(ima.domain());
  unsigned i = 10;
  for_all(p)
    mln_assertion(ima(p) == i++);

  typedef ima_t::win_t win_t;
  win_t win;

  {
    // Window - Forward iteration
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
    // Window - Backward iteration
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


  typedef ima_t::nbh_t neighb_t;
  neighb_t neigh;

  {
    // Neighborhood - Forward iteration
    mln_fwd_niter_(neighb_t) n(neigh, p);
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
    // Neighborhood - Backward iteration
    mln_bkd_niter_(neighb_t) n(neigh, p);
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
