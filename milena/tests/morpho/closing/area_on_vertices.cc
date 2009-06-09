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

#include <mln/core/image/edge_image.hh>
#include <mln/morpho/closing/area_on_vertices.hh>
#include <mln/util/graph.hh>
#include <mln/fun/i2v/array.hh>

#include "tests/data.hh"


const unsigned result[] = { 101, 101, 102 };

int main()
{
  using namespace mln;

  util::graph gr;
  gr.add_vertices(5);
  gr.add_edge(1, 3);
  gr.add_edge(1, 2);
  gr.add_edge(2 ,4);

  fun::i2v::array<unsigned> f(5);
  for (unsigned i = 0; i < 5; ++i)
    f(i) = 100 + i;
  typedef edge_image<void,unsigned> e_ima_t;
  e_ima_t e_ima(gr, f);
  typedef e_ima_t::nbh_t nbh_t;
  nbh_t nbh;

  e_ima_t clo = morpho::closing::area_on_vertices(e_ima, nbh, 3);

  unsigned i = 0;
  mln_piter_(e_ima_t) p(clo.domain());
  for_all(p)
    mln_assertion(result[i++] == clo(p));
}
