// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/util/graph.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/site_set/p_vertices.hh>
#include <mln/fun/i2v/array.hh>


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
  g.add_edge(0, 2);
  g.add_edge(3, 4);
  g.add_edge(4, 5);
  g.add_edge(5, 4);
  g.add_edge(1, 0);
  g.add_edge(5, 3);
  g.add_edge(2, 1);

  // Map vertices to sites.
  typedef fun::i2v::array<point2d> F;
  F f(6);
  for (unsigned i = 0; i < 6; ++i)
    f(i) = point2d(i, 0);

  typedef p_vertices<util::graph, F> p_vertices;
  p_vertices pv(g, f);

  // Forward iterator.
  {
    mln_fwd_piter_(p_vertices) p(pv);
    unsigned i = 0;
    for_all(p)
      mln_assertion(p.p_hook_().v().id() == i++);
    mln_assertion(i == g.v_nmax());
  }

  // Backward iterator.
  {
    mln_bkd_piter_(p_vertices) p(pv);
    unsigned i = g.v_nmax() - 1;
    for_all(p)
      mln_assertion(p.p_hook_().v().id() == i--);
    mln_assertion(i == mln_max(unsigned));
  }

}
