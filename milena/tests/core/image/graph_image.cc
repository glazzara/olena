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

/// \file tests/core/image/graph_image.cc
/// \brief Tests on mln::graph_image.

#include <vector>

#include <mln/accu/bbox.hh>
#include <mln/core/alias/box2d.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/site_set/p_vertices.hh>

#include <mln/core/image/image_if.hh>
#include <mln/core/image/sub_image.hh>
#include <mln/core/var.hh>
#include <mln/core/routine/ops.hh>
#include <mln/literal/ops.hh>
#include <mln/fun/ops.hh>
#include <mln/value/ops.hh>
#include <mln/core/image/graph_elt_window.hh>
#include <mln/core/image/graph_elt_neighborhood.hh>

#include <mln/fun/i2v/array.hh>
#include <mln/fun/p2v/iota.hh>

#include <mln/util/graph.hh>

#include <mln/debug/graph.hh>
//#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>
#include <mln/core/concept/function.hh>

template <typename S>
struct viota_t : public mln::Function_p2v< viota_t<S> >
{
  typedef unsigned result;

  viota_t(unsigned size)
  {
    v_.resize(size);
    for (unsigned i = 0; i < size; ++i)
      v_[i] = 10 + i;
  }

  unsigned
  operator()(const mln_psite(S)& p) const
  {
    return v_[p.v().id()];
  }

  protected:
    std::vector<result> v_;
};



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
       4 |        3-4

  */

  // Points associated to vertices.
  typedef fun::i2v::array<point2d> fpoint_t;
  fpoint_t points(5);
  points(0) = point2d(0,0); // Point associated to vertex 0.
  points(1) = point2d(2,2); // Point associated to vertex 1.
  points(2) = point2d(0,4); // Point associated to vertex 2.
  points(3) = point2d(4,3); // Point associated to vertex 3.
  points(4) = point2d(4,4); // Point associated to vertex 4.

  // Edges.
  util::graph g;
  // Populate the graph with vertices.
  for (unsigned i = 0; i < points.size(); ++i)
    g.add_vertex();
  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 2);

  g.print_debug(std::cout);

  /*----------------------.
  | Graph image support.  |
  `----------------------*/

  typedef p_vertices<util::graph, fpoint_t> S;
  S pv(g, points);

  /*-------------.
  | Graph image.  |
  `-------------*/

  // Graph values.
  viota_t<S> iota(5);

  // Create graph image.
  mln_const_VAR(ima, (iota | pv));

  {
    // Compute the bounding box of 'ima'.
    accu::bbox<point2d> a;
    mln_piter_(ima_t) p(ima.domain());
    for_all(p)
      a.take(p);
    box2d bbox = a.to_result();
    mln_assertion(bbox == make::box2d(5, 5));

  // Print the image.
  /* FIXME: Unfortunately, displaying graph images is not easy right
     now (2008-02-05).  We could use

       debug::println(ima);

     but there's not specialization working for graph_image; the one
     selected by the compiler is based on a 2-D bbox, and expects the
     interface of graph_image to work with points (not psites).
     Moreover, this implementation only shows *values*, not the graph
     itslef.

     An alternative is to use debug::graph,
     but it doesn't show the values, only the vertices and edges of the
     graph.

     The current solution is a mix between debug::graph and hand-made
     iterations.  */
    image2d<int> ima_rep(bbox);

  // We use the value 9 in debug::graph to represent edges to distinguish it
  // from vertices holding a value of 1.
    debug::graph(ima_rep, pv, 1, 9);
    debug::println(ima_rep);
  }

  /*------------.
  | Iterators.  |
  `------------*/

  // iteration over the domain of IMA.
  mln_piter_(ima_t) p(ima.domain());
  for_all (p)
    std::cout << "ima (" << p << ") = " << ima(p) << std::endl;

  {
    // Window - Forward iteration
    typedef graph_elt_window<util::graph, fpoint_t> win_t;
    win_t win;
    mln_qiter_(win_t) q(win, p);
    for_all (p)
    {
      std::cout << "neighbors of " << p << " (" << ima(p) << "), "
		<< "including the site itself:" << std::endl;
      for_all (q)
	std::cout << "  " << q << " (level = " << ima(q) << ")" << std::endl;
    }
  }

  {
    // Window - Backward iteration
    typedef graph_elt_window<util::graph, fpoint_t> win_t;
    win_t win;
    mln_bkd_qiter_(win_t) q(win, p);
    for_all (p)
    {
      std::cout << "neighbors of " << p << " (" << ima(p) << "), "
		<< "including the site itself:" << std::endl;
      for_all (q)
	std::cout << "  " << q << " (level = " << ima(q) << ")" << std::endl;
    }
  }

  {
    // Neighborhood - Forward iteration
    typedef graph_elt_neighborhood<util::graph, fpoint_t> neigh_t;
    neigh_t neigh;
    mln_niter_(neigh_t) n(neigh, p);
    for_all (p)
    {
      std::cout << "neighbors of " << p << " (" << ima(p) << "), "
		<< "including the site itself:" << std::endl;
      for_all (n)
	std::cout << "  " << n << " (level = " << ima(n) << ")" << std::endl;
    }
  }

  {
    // Neighborhood - Backward iteration
    typedef graph_elt_neighborhood<util::graph, fpoint_t> neigh_t;
    neigh_t neigh;
    mln_bkd_niter_(neigh_t) n(neigh, p);
    for_all (p)
    {
      std::cout << "neighbors of " << p << " (" << ima(p) << "), "
		<< "including the site itself:" << std::endl;
      for_all (n)
	std::cout << "  " << n << " (level = " << ima(n) << ")" << std::endl;
    }
  }
  std::cout << std::endl;
}
