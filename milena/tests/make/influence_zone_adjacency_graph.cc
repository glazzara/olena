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

#include <mln/make/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/util/graph.hh>
#include <mln/make/influence_zone_adjacency_graph.hh>
#include <mln/value/label_8.hh>


int main()
{
  using namespace mln;

  value::label_8 vals[9] = { 1, 1, 3,
			     1, 2, 3,
			     1, 0, 4 };

  image2d<value::label_8> ima = make::image2d(vals);

  util::graph g = make::influence_zone_adjacency_graph(ima, c4(), 4u);

  mln_assertion(g.e_nmax() == 7u);
  mln_assertion(g.v_nmax() == 5u);
  mln_assertion(g.v_nmax_nbh_edges(0) == 3);
  mln_assertion(g.v_nmax_nbh_edges(1) == 3);
  mln_assertion(g.v_nmax_nbh_edges(2) == 3);
  mln_assertion(g.v_nmax_nbh_edges(3) == 3);
  mln_assertion(g.v_nmax_nbh_edges(4) == 2);
}
