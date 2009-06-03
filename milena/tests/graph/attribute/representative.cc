// Copyright (C) 2009 EPITA Research and Development Laboratory
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

/// \file tests/graph/attribute/representative.cc
///
/// Tests on mln::graph::attribute::representative.

#include <mln/util/graph.hh>
#include <mln/graph/compute.hh>
#include <mln/graph/attribute/representative.hh>

int main()
{
  using namespace mln;

  util::graph g;

  /// Construct the following graph:
  ///
  /// 0 -- 1 -- 2
  ///
  /// 3 -- 4
  ///
  /// 5
  ///
  /// There are 3 components.
  //
  g.add_vertices(6);
  g.add_edge(0,1);
  g.add_edge(1,2);
  g.add_edge(3,4);

  util::array<unsigned> result = graph::compute(g, graph::attribute::representative);

  mln_assertion(result.size() == 3);
  mln_assertion(result[0] == 0);
  mln_assertion(result[1] == 3);
  mln_assertion(result[2] == 5);
}
