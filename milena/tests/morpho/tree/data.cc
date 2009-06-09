// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_array.hh>

#include <mln/data/sort_psites.hh>
#include <mln/debug/println.hh>

#include <mln/morpho/tree/data.hh>


int main()
{
  using namespace mln;

  typedef image2d<unsigned char> I;
//   unsigned char vals[] = { 3, 2, 1,
// 			   3, 2, 3,
// 			   3, 4, 1 };

  unsigned char vals[] = { 3, 3, 3,
			   3, 4, 4,
			   3, 4, 4 };

  I ima = make::image2d(vals);

  debug::println("ima = ", ima);

  typedef p_array<point2d> S;
  S s = data::sort_psites_decreasing(ima);

  typedef morpho::tree::data<I,S> tree_t;
  tree_t t(ima, s, c4());

  debug::println( "parent  = ", t.parent_image() | t.domain() );
  debug::println( "on node = ", t.parent_image() | t.nodes()  );
  debug::println( "on leaves = ", t.parent_image() | t.leaves()  );

  {
    /* Check site and node up order */
    tree_t::up_node_piter n(t);
    tree_t::up_site_piter s(t);
    tree_t::up_leaf_piter l(t);
    n.start();
    l.start();
    for_all(s)
      if (t.is_a_node(s))
	{
	  mln_assertion(s == n);
	  if (t.is_a_leaf(n))
	    {
	      mln_assertion(l == n);
	      l.next();
	    }
	  n.next();
	}
    mln_assertion(!n.is_valid() && !s.is_valid() && !l.is_valid());
  }

  {
    /* Check site and node up order */
    tree_t::dn_node_piter n(t);
    tree_t::dn_site_piter s(t);
    tree_t::dn_leaf_piter l(t);
    n.start();
    l.start();
    for_all(s)
      if (t.is_a_node(s))
	{
	  mln_assertion(s == n);
	  if (t.is_a_leaf(n))
	    {
	      mln_assertion(l == n);
	      l.next();
	    }
	  n.next();
	}
    mln_assertion(!n.is_valid() && !s.is_valid() && !l.is_valid());
  }


  {
    std::cout << "nodes = ";
    tree_t::up_node_piter n(t);
    for_all(n)
      std::cout << n << ' ';
    std::cout << std::endl
	      << std::endl;
  }


  {
    image2d<unsigned> area(ima.domain());
    data::fill(area, 1);
    tree_t::up_site_piter p(t);
    for_all(p)
      if (! t.is_root(p))
	area(t.parent(p)) += area(p);
    debug::println("area = ", area | t.nodes());
  }

}
