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

#include <iostream>

#include <mln/util/graph.hh>
#include <mln/util/line_graph.hh>

int main()
{
  using namespace mln;

  util::graph g;

  g.add_vertex(); // 0
  g.add_vertex(); // 1
  g.add_vertex(); // 2
  g.add_vertex(); // 3
  g.add_vertex(); // 4
  g.add_vertex(); // 5
  g.add_edge(0, 1);
  g.add_edge(1, 0); // Not inserted twice
  g.add_edge(0, 2);
  g.add_edge(3, 4);
  g.add_edge(4, 5);
  g.add_edge(5, 4); // Not inserted twice
  g.add_edge(5, 3);
  g.add_edge(2, 1);

  typedef util::line_graph<util::graph> LG;
  LG lg(g);

  // Vertex and edge forward iterators.
  {
    unsigned i = 0;
    mln_vertex_fwd_iter_(LG) v(lg);
    for_all(v)
      mln_assertion(i++ == v.id());
    mln_assertion(i != 0);

    i = 0;
    mln_edge_fwd_iter_(LG) e(lg);
    for_all(e)
      mln_assertion(i++ == e.id());
    mln_assertion(i != 0);
  }

  // Vertex and edge backward iterators.
  {
    unsigned i = lg.v_nmax() - 1;
    mln_vertex_bkd_iter_(LG) v(lg);
    for_all(v)
      mln_assertion(i-- == v.id());
    mln_assertion(i != lg.v_nmax() - 1);

    i = lg.e_nmax() - 1;
    mln_edge_bkd_iter_(LG) e(lg);
    for_all(e)
      mln_assertion(i-- == e.id());
    mln_assertion(i != lg.e_nmax() - 1);
  }

  // Vertex and edge nbh forward iterators
  {
    mln_vertex_fwd_iter_(LG) v(lg);
    mln_vertex_nbh_edge_fwd_iter_(LG) n(v);
    for_all(v)
    {
      unsigned i = 0;
      for_all(n)
	mln_assertion(i++ == n.index());
      mln_assertion(i != 0);
    }
  }

  // Vertex and edge nbh backward iterators.
  {
    mln_vertex_bkd_iter_(LG) v(lg);
    mln_vertex_nbh_edge_bkd_iter_(LG) e(v);
    for_all(v)
    {
      unsigned i = v.nmax_nbh_edges();
      for_all(e)
	mln_assertion(--i == e.index());
      mln_assertion((v.nmax_nbh_edges() == 0 && i == 0)
		    || i != v.nmax_nbh_edges());
    }
  }
  {
    mln_edge_fwd_iter_(LG) e(lg);
    mln_edge_nbh_edge_fwd_iter_(LG) n(e);
    for_all(e)
    {
      unsigned i = 0;
      for_all(n)
	++i;
      // we check i == e.nmax_nbh_edges() - 2 since e is it's own
      // neighboor and the iterator skip it.
      mln_assertion((i == 0 && e.nmax_nbh_edges() < 2)
		    || i == e.nmax_nbh_edges() - 2);
    }
  }
  {
    mln_edge_bkd_iter_(LG) e(lg);
    mln_edge_nbh_edge_bkd_iter_(LG) n(e);
    for_all(e)
    {
      //std::cout << "== e.id() = " << e.id() << std::endl;
      unsigned i = e.nmax_nbh_edges();
      for_all(n)
	--i;
      // we check i == e.nmax_nbh_edges() - 2 since e is it's own
      // neighboor and the iterator skip it.
      mln_assertion((i == e.nmax_nbh_edges() && e.nmax_nbh_edges() < 2)
		    || i == 2);

    }
  }
  {
    mln_vertex_fwd_iter_(LG) v(lg);
    mln_vertex_nbh_vertex_fwd_iter_(LG) n(v);
    for_all(v)
    {
      unsigned i = 0;
      for_all(n)
	++i;
      mln_assertion(i == v.nmax_nbh_vertices());
    }
  }
  {
    mln_vertex_bkd_iter_(LG) v(lg);
    mln_vertex_nbh_vertex_bkd_iter_(LG) n(v);
    for_all(v)
    {
      unsigned i = v.nmax_nbh_vertices();
      for_all(n)
	--i;
      mln_assertion(i == 0);
    }
  }
}
