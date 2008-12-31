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

/// \file tests/morpho/line_graph_image_morpho.cc
///
/// Tests on mln::line_graph_image with morphological filters.

#include <mln/core/alias/point2d.hh>

/// Required for line graph images.
#include <mln/core/site_set/p_edges.hh>
#include <mln/core/image/line_graph_elt_window.hh>
#include <mln/core/var.hh>
#include <mln/pw/all.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/util/graph.hh>

#include <mln/morpho/erosion.hh>
#include <mln/morpho/dilation.hh>


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

  */

  // Graph.
  util::graph g;

  // Points associated to vertices.
  typedef fun::i2v::array<point2d> fsite_t;
  fsite_t sites(5);
  sites(0) = point2d(0,0); // Point associated to vertex 0.
  sites(1) = point2d(2,2); // Point associated to vertex 1.
  sites(2) = point2d(0,4); // Point associated to vertex 2.
  sites(3) = point2d(4,3); // Point associated to vertex 3.
  sites(4) = point2d(4,4); // Point associated to vertex 4.

  // Populate the graph with vertices.
  for (unsigned i = 0; i < sites.size(); ++i)
    g.add_vertex();

  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 2);


  /*---------------------------.
  | Line graph image support.  |
  `---------------------------*/

  typedef p_edges<util::graph, fsite_t> pe_t;
  pe_t pe(g, sites);

  /*-------------------.
  | Line graph image.  |
  `-------------------*/

  // Line graph values.
  typedef fun::i2v::array<unsigned> viota_t;
  viota_t iota(pe.nsites());
  for (unsigned i = 0; i < iota.size(); ++i)
    iota(i) = 10 + i;

  // Create line graph image.
  mln_const_VAR(ima, (iota | pe));


  /*-------------------------------.
  | Processing line graph images.  |
  `-------------------------------*/

  line_graph_elt_window<util::graph, fsite_t> win;

  mln_const_VAR(ima_dil, morpho::dilation(ima, win));
  // Manual iteration over the domain of IMA_DIL.
  mln_piter_(ima_dil_t) p_dil(ima_dil.domain());
  for_all (p_dil)
    std::cout << "ima_dil (" << p_dil << ") = " << ima_dil(p_dil) << std::endl;
  std::cout << std::endl;

  mln_const_VAR(ima_ero, morpho::erosion(ima, win));
  // Manual iteration over the domain of IMA_ERO.
  mln_piter_(ima_ero_t) p_ero(ima_ero.domain());
  for_all (p_ero)
    std::cout << "ima_ero (" << p_ero << ") = " << ima_ero(p_ero) << std::endl;
}
