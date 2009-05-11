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

/// \file tests/make/region_adjacency_graph.cc
///
/// Tests on mln::make::region_adjacency_graph.

#include <mln/core/alias/neighb2d.hh>

#include <mln/util/graph.hh>

#include <mln/value/label_8.hh>

#include <mln/make/image2d.hh>
#include <mln/make/region_adjacency_graph.hh>

int main()
{
  using namespace mln;

  value::label_8 vals[16] = { 1, 0, 3, 3,
			      1, 0, 0, 0,
			      0, 0, 4, 4,
			      2, 2, 0, 4 };

  image2d<value::label_8> ima = make::image2d(vals);

  util::graph g = make::region_adjacency_graph(ima, c4(), 4u);

  mln_assertion(g.e_nmax() == 4u);
  mln_assertion(g.v_nmax() == 5u);
  mln_assertion(g.v_nmax_nbh_edges(0) == 0);
  for (unsigned i = 1; i < 4; ++i)
    mln_assertion(g.v_nmax_nbh_edges(i) == 2);
}
