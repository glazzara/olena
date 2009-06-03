// Copyright (C) 2009 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file tests/core/other/graph_elt_window_if.cc
///
/// Tests on mln::graph_elt_window_if.


#include <mln/core/image/edge_image.hh>
#include <mln/core/image/vertex_image.hh>

#include <mln/core/image/graph_elt_window_if.hh>

#include <mln/make/dummy_p_vertices.hh>
#include <mln/make/p_vertices_with_mass_centers.hh>
#include <mln/make/dummy_p_edges.hh>
#include <mln/make/p_edges_with_mass_centers.hh>

#include <mln/data/fill.hh>

#include <mln/util/graph.hh>
#include <mln/util/site_pair.hh>

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

  /// Mask of vertices while iterating on vertices.
  {
    static const unsigned result[] = { 3, 3, 5, 1, 5, 1, 5, 3 };

    // Create a vertex image.
    typedef p_vertices<G> pv_t;
    pv_t pv = make::dummy_p_vertices(gr);
    typedef vertex_image<void,unsigned> v_ima_t;
    v_ima_t v_ima(pv);
    data::fill(v_ima, 4);

    // Create a mask on vertices.
    typedef mln_ch_value_(v_ima_t, bool) v_mask_t;
    v_mask_t v_mask(pv);
    mln_piter_(v_mask_t) vm(v_mask.domain());
    for_all(vm)
      v_mask(vm) = vm.id()%2;

    mln_piter_(v_ima_t) v(v_ima.domain());
    typedef graph_elt_window_if<util::graph, pv_t, v_mask_t> win_t;
    win_t win(v_mask);
    mln_qiter_(win_t) q(win, v);
    unsigned i = 0;
    for_all(v)
      for_all(q)
      {
	mln_assertion(result[i++] == q.id());
	mln_assertion(q.id()%2);
      }
  }

  /// Mask of edges while iterating on edges.
  {
    // Create an edge image.
    typedef p_edges<G> pe_t;
    pe_t pe = make::dummy_p_edges(gr);
    typedef edge_image<void,unsigned> e_ima_t;
    e_ima_t e_ima(pe);
    data::fill(e_ima, 3);

    // Create a mask on edges.
    typedef mln_ch_value_(e_ima_t, bool) e_mask_t;
    e_mask_t e_mask(pe);
    mln_piter_(e_mask_t) em(e_mask.domain());
    for_all(em)
      e_mask(em) = em.id()%2;

    // Iterate on edges neighborhood according to the given mask.
    mln_piter_(e_ima_t) e(e_ima.domain());
    typedef graph_elt_window_if<util::graph, pe_t, e_mask_t> win_t;
    win_t win(e_mask);
    mln_qiter_(win_t) q(win, e);
    for_all(e)
      for_all(q)
	mln_assertion(q.id()%2);
  }


  /// Mask of vertices while iterating on edges.
  {
    // Create an edge image.
    typedef p_edges<G> pe_t;
    pe_t pe = make::dummy_p_edges(gr);
    typedef edge_image<void,unsigned> e_ima_t;
    e_ima_t e_ima(pe);
    data::fill(e_ima, 3);


    // Create a mask on vertices.
    typedef p_vertices<G> pv_t;
    pv_t pv = make::dummy_p_vertices(gr);
    typedef vertex_image<void,unsigned> v_ima_t;
    v_ima_t v_ima(pv);
    typedef mln_ch_value_(v_ima_t, bool) v_mask_t;
    v_mask_t v_mask(pv);
    mln_piter_(v_mask_t) vm(v_mask.domain());
    for_all(vm)
      v_mask(vm) = vm.id()%2;

    mln_piter_(e_ima_t) e(e_ima.domain());
    typedef graph_elt_window_if<util::graph, pe_t, v_mask_t> win_t;
    win_t win(v_mask);
    mln_qiter_(win_t) q(win, e);
    for_all(e)
      for_all(q)
	mln_assertion(v_mask(q.v1()) || v_mask(q.v2()));
  }


  /// Mask of edges while iterating on vertices.
  {
    static const unsigned result[] = { 4, 4, 5, 1, 2, 3 };

    // Create a vertex image.
    typedef p_vertices<G> pv_t;
    pv_t pv = make::dummy_p_vertices(gr);
    typedef vertex_image<void,unsigned> v_ima_t;
    v_ima_t v_ima(pv);
    data::fill(v_ima, 4);

    // Create a mask on edges.
    typedef p_edges<G> pe_t;
    pe_t pe = make::dummy_p_edges(gr);
    typedef edge_image<void,bool> e_mask_t;
    e_mask_t e_mask(pe);
    mln_piter_(e_mask_t) em(e_mask.domain());
    for_all(em)
      e_mask(em) = em.id()%2;

    mln_piter_(v_ima_t) v(v_ima.domain());
    typedef graph_elt_window_if<util::graph, pv_t, e_mask_t> win_t;
    win_t win(e_mask);
    mln_qiter_(win_t) q(win, v);
    unsigned i = 0;
    for_all(v)
      for_all(q)
      {
	mln_assertion(result[i++] == q.id());
	mln_assertion(e_mask(v.edge_with(q).id()));
      }
  }
}
