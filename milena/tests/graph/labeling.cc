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

#include <mln/core/image/edge_image.hh>
#include <mln/core/image/vertex_image.hh>

#include <mln/core/image/graph_elt_neighborhood_if.hh>

#include <mln/data/fill.hh>

#include <mln/util/graph.hh>

#include <mln/value/label_8.hh>

#include <mln/fun/i2v/array.hh>

#include <mln/graph/labeling.hh>
#include <mln/graph/to_neighb.hh>



static const unsigned result[] = { 1, 2, 2, 3, 2, 3 };



int main()
{
  using namespace mln;

  typedef util::graph G;

  /*--------.
  | Graph.  |
  `--------*/

  /* The graph is as follows:

          _____
         /     \
     0  1  2 - 3
        \ / \ /
         4 - 5

  */

  util::graph gr;
  gr.add_vertices(6);
  gr.add_edge(1,3);
  gr.add_edge(1,4);
  gr.add_edge(2,3);
  gr.add_edge(2,4);
  gr.add_edge(2,5);
  gr.add_edge(3,5);
  gr.add_edge(4,5);


  typedef fun::i2v::array<point2d> fsite_t;
  fsite_t fsite(6);
  fsite(0) = point2d(0,0);
  fsite(1) = point2d(2,2);
  fsite(2) = point2d(4,2);
  fsite(3) = point2d(6,2);
  fsite(4) = point2d(2,4);
  fsite(5) = point2d(2,6);

  // Create a vertex image.
  typedef p_vertices<G,fsite_t> pv_t;
  pv_t pv(gr, fsite);
  typedef vertex_image<point2d,unsigned> v_ima_t;
  v_ima_t v_ima(pv);
  data::fill(v_ima, 4);

  // Create a mask on edges.
  typedef p_edges<G> pe_t;
  pe_t pe(gr);
  typedef edge_image<void,bool> e_mask_t;
  e_mask_t e_mask(pe);
  mln_piter_(e_mask_t) em(e_mask.domain());
  for_all(em)
    e_mask(em) = em.id()%2;

  typedef graph_elt_neighborhood_if<util::graph, pv_t, e_mask_t> nbh_t;
  nbh_t nbh = graph::to_neighb(v_ima, e_mask);

  value::label_8 nlabels;
  typedef mln_ch_value_(v_ima_t,value::label_8) lbl_t;
  lbl_t lbl = graph::labeling(v_ima, nbh, nlabels);

  unsigned i = 0;
  mln_piter_(lbl_t) p(lbl.domain());
  for_all(p)
    mln_assertion(result[i++] == lbl(p));
}
