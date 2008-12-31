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

/// \file tests/morpho/line_graph_image_wst.cc
///
/// Tests on the Watershed Transform on a mln::line_graph_image.

#include <mln/core/alias/point2d.hh>

/// Required for line graph images.
#include <mln/core/site_set/p_edges.hh>
#include <mln/core/image/line_graph_elt_window.hh>
#include <mln/core/neighb.hh>
#include <mln/core/var.hh>
#include <mln/pw/all.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/util/graph.hh>

#include <mln/morpho/meyer_wst.hh>


int main()
{
  using namespace mln;

  /*-------------.
  | Line graph.  |
  `-------------*/

  /* Actually this graph is from Jean Cousty's PhD thesis, page 76.

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

    In G, vertices and egdes are numbered following in the classical
    foward order.  */
  util::graph g;

  // Sites associated to vertices.
  typedef fun::i2v::array<point2d> fsite_t;
  fsite_t sites(5);
  sites(0) = point2d(0,0); // Point associated to vertex 0.
  sites(1) = point2d(0,1); // Point associated to vertex 1.
  sites(2) = point2d(0,2); // Point associated to vertex 2.
  sites(3) = point2d(0,3); // Point associated to vertex 3.
  sites(4) = point2d(1,0); // Point associated to vertex 4.
  sites(5) = point2d(1,1); // Point associated to vertex 5.
  sites(6) = point2d(1,2); // Point associated to vertex 6.
  sites(7) = point2d(1,3); // Point associated to vertex 7.

  // Populate the graph with vertices.
  for(unsigned i = 0; i < sites.size(); ++i)
    g.add_vertex();

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

  typedef p_edges<util::graph, fsite_t> pe_t;
  pe_t pe(g, sites);

  // Edge values.
  typedef fun::i2v::array<int> edge_values_t;
  edge_values_t edge_values(pe.nsites());

  static const int values[] = { 0, 10, 5, 2, 4, 6, 0, 3, 5, 2 };
  for (unsigned i = 0; i < edge_values.size(); ++i)
    edge_values(i) = values[i];

  // Create line graph image.
  mln_const_VAR(ima,(edge_values | pe));


  /*------------.
  | Iterators.  |
  `------------*/

  // Manual iteration over the domain of IMA.
  mln_piter_(ima_t) p(ima.domain());
  for_all (p)
    std::cout << "ima (" << p << ") = " << ima(p) << std::endl;
  std::cout << std::endl;

  typedef line_graph_elt_window<util::graph, fsite_t> win_t;
  win_t win;
  neighb<win_t> nbh(win);

  unsigned nbasins;
  mln_const_VAR(wshed, morpho::meyer_wst(ima, nbh, nbasins));
  std::cout << "nbasins = " << nbasins << std::endl;

  // Manual iteration over the domain of WSHED.
  mln_piter_(wshed_t) pw(wshed.domain());
  for_all (pw)
    std::cout << "wshed (" << pw << ") = " << wshed(pw) << std::endl;
}
