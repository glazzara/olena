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

/// \file
/// \brief Tests on the Watershed Transform on a graph-based image.

#include <vector>

#include <mln/util/graph.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/image/vertex_image.hh>
#include <mln/make/vertex_image.hh>

#include <mln/morpho/watershed/flooding.hh>


int main()
{
  using namespace mln;

  /*--------.
  | Graph.  |
  `--------*/

  /* The graph is as follows:

            0 1 2 3 4
         .-----------
         |
       0 |  0       2
       1 |    \   / |
       2 |      1   |
       3 |       \  |
       4 |        3-0

  */

  // Points associated to vertices.
  typedef fun::i2v::array<point2d> fsite_t;
  fsite_t sites(5);
  sites(0) = point2d(0,0); // Point associated to vertex 0.
  sites(1) = point2d(2,2); // Point associated to vertex 1.
  sites(2) = point2d(0,4); // Point associated to vertex 2.
  sites(3) = point2d(4,3); // Point associated to vertex 3.
  sites(4) = point2d(4,4); // Point associated to vertex 4.

  // Graph.
  util::graph g;

  // Populate the graph with vertices.
  g.add_vertices(sites.size());

  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 2);

  /*-------------.
  | Graph image.  |
  `-------------*/

  // Graph values.
  typedef fun::i2v::array<unsigned> viota_t;
  viota_t iota(g.v_nmax());
  for (unsigned i = 0; i < iota.size(); ++i)
    iota(i) = 10 + i;

  typedef vertex_image<point2d, unsigned, util::graph> ima_t;
  ima_t ima = make::vertex_image(g, sites, iota);

  /*------.
  | WST.  |
  `------*/

  // Elementary neighborhood of a vertex.
  typedef ima_t::nbh_t nbh_t;
  nbh_t nbh;

  unsigned nbasins;
  ima_t wshed = morpho::watershed::flooding(ima, nbh, nbasins);
  std::cout << "nbasins = " << nbasins << std::endl;

  // Manual iteration over the domain of WSHED.
  mln_piter_(ima_t) pw(wshed.domain());
  for_all (pw)
    std::cout << "wshed (" << pw << ") = " << wshed(pw) << std::endl;
}
