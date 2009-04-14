// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file tests/core/site_set/p_edges.cc
///
/// Tests on mln::p_edges.

#include <mln/util/graph.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/site_set/p_edges.hh>


// Function mapping an edge to a specific site.

template <typename G>
struct my_fun : mln::Function< my_fun<G> >
{
  typedef mln::point2d result;

  const result& operator()(unsigned v) const
  {
    static mln::point2d res(0, 0);
    res.row() = v;
    return res;
  }
};


int main()
{
  using namespace mln;

  // Create a graph.
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

  typedef p_edges<util::graph, my_fun<util::graph> > p_edges;
  p_edges pv(g, my_fun<util::graph>());

  // Forward iterator.
  {
    mln_fwd_piter_(p_edges) p(pv);
    unsigned i = 0;
    for_all(p)
      mln_assertion(p.p_hook_().e().id() == i++);
    mln_assertion(i == g.e_nmax());
  }

  // Backward iterator.
  {
    mln_bkd_piter_(p_edges) p(pv);
    unsigned i = g.e_nmax() - 1;
    for_all(p)
      mln_assertion(p.p_hook_().e().id() == i--);
    mln_assertion(i == mln_max(unsigned));
  }

}
