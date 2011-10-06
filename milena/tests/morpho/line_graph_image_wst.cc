// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

/// \file tests/morpho/line_graph_image_wst.cc
///
/// Tests on the Watershed Transform on a line-graph-based image.

#include <mln/core/alias/point2d.hh>

#include <mln/util/graph.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/util/site_pair.hh>
#include <mln/core/image/edge_image.hh>

#include <mln/morpho/watershed/flooding.hh>


int main()
{
  using namespace mln;

  /*-------------.
  | Line graph.  |
  `-------------*/

  /* This graph is from Jean Cousty's PhD thesis, page 76.

               0     1     2     3  (rows)
         ,------------------------
         |        0     10    5
       0 |     o-----o-----o-----o
         |     |     |     |     |
         |    2|    4|    6|    0|
         |     |     |     |     |
       1 |     o-----o-----o-----o
                  3     5     2
    (cols)

    In G, vertices and edges are numbered following in the classical
    foward order.  */
  util::graph g;

  // Populate the graph with 8 vertices.
  g.add_vertices(8);

  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(2, 3);

  g.add_edge(0, 4);
  g.add_edge(1, 5);
  g.add_edge(2, 6);
  g.add_edge(3, 7);

  g.add_edge(4, 5);
  g.add_edge(5, 6);
  g.add_edge(6, 7);

  // Sites associated to edges.
  typedef util::site_pair<point2d> P;
  typedef fun::i2v::array<P> fsite_t;
  fsite_t sites(10);
  sites(0) = P(point2d(0,0), point2d(0,1)); // Site associated to edge 0.
  sites(1) = P(point2d(0,1), point2d(0,2)); // Site associated to edge 1.
  sites(2) = P(point2d(0,2), point2d(0,3)); // Site associated to edge 2.
  sites(3) = P(point2d(0,0), point2d(1,0)); // Site associated to edge 7.
  sites(4) = P(point2d(0,1), point2d(1,1)); // Site associated to edge 8.
  sites(5) = P(point2d(0,2), point2d(1,2)); // Site associated to edge 9.
  sites(6) = P(point2d(0,3), point2d(1,3)); // Site associated to edge 3.
  sites(7) = P(point2d(1,0), point2d(1,1)); // Site associated to edge 4.
  sites(8) = P(point2d(1,1), point2d(1,2)); // Site associated to edge 5.
  sites(9) = P(point2d(1,2), point2d(1,3)); // Site associated to edge 6.

  // Edge values.
  typedef fun::i2v::array<unsigned> edge_values_t;
  edge_values_t edge_values(10);

  static const unsigned values[] = { 0, 10, 5, 2, 4, 6, 0, 3, 5, 2 };
  for (unsigned i = 0; i < edge_values.size(); ++i)
    edge_values(i) = values[i];

  typedef edge_image< P, unsigned, util::graph > ima_t;
  ima_t ima(g, sites, edge_values);


  /*------------.
  | Iterators.  |
  `------------*/

  // Reference values.
  const unsigned ima_ref[] = { 0, 10, 5, 2, 4, 6, 0, 3, 5, 2 };
  const unsigned ima_wst[] = { 2, 0, 1, 2, 2, 1, 1, 2, 0, 1 };

  unsigned i = 0;

  // Manual iteration over the domain of IMA.
  mln_piter_(ima_t) p(ima.domain());
  for_all (p)
    mln_assertion(ima_ref[i++] == ima(p));

  // Elementary neighborhood of an edge.
  typedef ima_t::nbh_t nbh_t;
  nbh_t nbh;

  unsigned nbasins;
  typedef mln_ch_value_(ima_t, unsigned) wshed_t;
  wshed_t wshed = morpho::watershed::flooding(ima, nbh, nbasins);
  mln_assertion(nbasins == 2);

  i = 0;
  // Manual iteration over the domain of WSHED.
  mln_piter_(wshed_t) pw(wshed.domain());
  for_all (pw)
    mln_assertion(ima_wst[i++] == wshed(pw));
}
