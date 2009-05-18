// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/canvas/browsing/breadth_first_search.cc
///
/// Tests on mln::canvas::browsing::breadth_first_search.hh.

#include <iostream>
#include <mln/util/graph.hh>
#include <mln/util/array.hh>

#include <mln/canvas/browsing/breadth_first_search.hh>

struct Functor
{
  template <typename G> void init(const mln::Graph<G>& g)
  {
    deja_vu_.resize(exact(g).v_nmax());
    deja_vu_.fill(false);
    order.resize(0);
  }

  bool to_be_treated(unsigned id)
  {
    return !deja_vu_[id];
  }

  void new_component_from_vertex(unsigned id)
  {
    deja_vu_[id] = true;
  }

  void process_vertex(unsigned id)
  {
    order.append(id);
  }

  bool to_be_queued(unsigned id)
  {
    return !deja_vu_[id];
  }

  void added_to_queue(unsigned id)
  {
    deja_vu_[id] = true;
  }

  void next_component()
  {
  }

  void final()
  {
  }

  mln::util::array<unsigned> order;
  mln::util::array<bool> deja_vu_;
};

int main()
{
  using namespace mln;

  typedef util::graph G;
  G g;
  g.add_vertices(5);
  g.add_edge(0,4);
  g.add_edge(1,2);
  g.add_edge(1,3);
  g.add_edge(4,3);
  g.add_edge(4,2);

  unsigned result[] = {0, 4, 3, 2, 1};
  Functor f;

  canvas::browsing::breadth_first_search(g, f);

  for (unsigned i = 0; i < f.order.size(); ++i)
    mln_assertion(f.order[i] == result[i]);
}
