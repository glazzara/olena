// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

/// \file   tests/util/graph.cc
/// \brief  test of mln::util::graph

#include <mln/util/graph.hh>
#include <iostream>

int main ()
{
  using namespace mln;

  util::graph g;

  g.add_vertices(6);
  g.add_edge (0, 1);
  g.add_edge (0, 2);
  g.add_edge (3, 4);
  g.add_edge (4, 5);
  g.add_edge (5, 4);
  g.add_edge (1, 0);
  g.add_edge (5, 3);
  g.add_edge (2, 1);


  // Vertex iter and edge iter
  {
    unsigned i = 0;
    mln_vertex_fwd_iter_(util::graph) v(g);
    for_all(v)
      mln_assertion(i++ == v.index());
    mln_assertion(i != 0);

    i = 0;
    mln_edge_fwd_iter_(util::graph) e(g);
    for_all(e)
      mln_assertion(i++ == e.index());
    mln_assertion(i != 0);
  }
  {
    unsigned i = g.v_nmax() - 1;
    mln_vertex_bkd_iter_(util::graph) v(g);
    for_all(v)
      mln_assertion(i-- == v.index());
    mln_assertion(i != g.v_nmax() - 1);

    i = g.e_nmax() - 1;
    mln_edge_bkd_iter_(util::graph) e(g);
    for_all(e)
      mln_assertion(i-- == e.index());
    mln_assertion(i != g.e_nmax() - 1);
  }

  // vertex iter + Edge nbh iter
  {
    mln_vertex_fwd_iter_(util::graph) v(g);
    mln_vertex_nbh_edge_fwd_iter_(util::graph) n(v);
    for_all(v)
    {
      unsigned i = 0;
      for_all(n)
	mln_assertion(i++ == n.index());
      mln_assertion(i != 0);
    }
  }
  {
    mln_vertex_bkd_iter_(util::graph) v(g);
    mln_vertex_nbh_edge_bkd_iter_(util::graph) e(v);
    for_all(v)
    {
      unsigned i = v.nmax_nbh_edges();
      for_all(e)
	mln_assertion(--i == e.index());
      mln_assertion((v.nmax_nbh_edges() == 0 && i == 0) || i != v.nmax_nbh_edges());
    }
  }

  {
    mln_edge_fwd_iter_(util::graph) e(g);
    mln_edge_nbh_edge_fwd_iter_(util::graph) n(e);
    for_all(e)
    {
      unsigned i = 0;
      for_all(n)
	++i;
      // we check i == e.nmax_nbh_edges() - 2 since e is it's own neighboor and the
      // iterator skip it.
      mln_assertion((i == 0 && e.nmax_nbh_edges() < 2) || i == e.nmax_nbh_edges() - 2);
    }
  }
  {
    mln_edge_bkd_iter_(util::graph) e(g);
    mln_edge_nbh_edge_bkd_iter_(util::graph) n(e);
    for_all(e)
    {
      //std::cout << "== e.id() = " << e.id() << std::endl;
      unsigned i = e.nmax_nbh_edges();
      for_all(n)
	--i;
      // we check i == e.nmax_nbh_edges() - 2 since e is it's own neighboor and the
      // iterator skip it.
      mln_assertion((i == e.nmax_nbh_edges() && e.nmax_nbh_edges() < 2) || i == 2);

    }
  }

  {
    mln_vertex_fwd_iter_(util::graph) v(g);
    mln_vertex_nbh_vertex_fwd_iter_(util::graph) n(v);
    for_all(v)
    {
      unsigned i = 0;
      for_all(n)
	++i;
      mln_assertion(i == v.nmax_nbh_vertices());
    }
  }
  {
    mln_vertex_bkd_iter_(util::graph) v(g);
    mln_vertex_nbh_vertex_bkd_iter_(util::graph) n(v);
    for_all(v)
    {
      unsigned i = v.nmax_nbh_vertices();
      for_all(n)
	--i;
      mln_assertion(i == 0);
    }
  }
}
