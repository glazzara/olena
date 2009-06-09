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

#include <mln/core/alias/neighb2d.hh>

#include <mln/util/graph.hh>

#include <mln/value/label_8.hh>

#include <mln/make/image2d.hh>
#include <mln/make/rag_and_labeled_wsl.hh>

# include <mln/debug/println.hh>

int main()
{
  using namespace mln;

  value::label_8 vals[16] = { 1, 0, 3, 3,
			      1, 0, 0, 0,
			      0, 0, 4, 4,
			      2, 2, 0, 4 };

  typedef image2d<value::label_8> wst_t;
  wst_t ima = make::image2d(vals);

  util::couple<util::graph, wst_t>
    res = make::rag_and_labeled_wsl(ima, c4(), 4u);

  wst_t& lwsl = res.second();
  util::graph& g = res.first();

  // check that adjacency labels map edge labels.
  mln_piter_(wst_t) p(lwsl.domain());
  mln_niter_(neighb2d) n(c4(),p);
  for_all(p)
    if (lwsl(p) != mln_max(value::label_8).prev())
    {
      value::label_8 l1 = 0, l2 = 0;
      for_all(n)
	if (ima.has(n) && ima(n) != 0u)
	{
	  if (l1 == 0u) // First label to be stored.
	    l1 = ima(n);
	  else
	    if (ima(n) != l1) // Useless: && l2 == 0)
	    { // Second label to be stored.
	      mln_invariant(l2 == 0u);
	      l2 = ima(n);
	      break;
	    }
	}
      if (l2 < l1)
	std::swap(l1, l2);
      mln_assertion(g.v1(lwsl(p)) == l1);
      mln_assertion(g.v2(lwsl(p)) == l2);
    }

  mln_assertion(g.e_nmax() == 4u);
  mln_assertion(g.v_nmax() == 5u);
  mln_assertion(g.v_nmax_nbh_edges(0) == 0);
  for (unsigned i = 1; i < 4; ++i)
    mln_assertion(g.v_nmax_nbh_edges(i) == 2);
}
