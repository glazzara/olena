// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
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

#include "granulometry.hh"

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>

#include <mln/io/pgm/load.hh>

#include <mln/data/sort_psites.hh>
#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/attribute/card.hh>

#include <mln/data/fill.hh>
#include "../../tree/propagate_node.hh"

#include <iostream>
#include <map>

void usage(char** argv)
{
  std::cout << "Usage: " << argv[0] << " input.pgm" << std::endl;
  abort();
}

int main(int argc, char** argv)
{
  using namespace mln;
  using value::int_u8;

  if (argc != 2)
    usage(argv);

  typedef image2d<int_u8> I;
  I input;
  io::pgm::load(input, argv[1]);

  typedef p_array<point2d> S;
  typedef morpho::tree::data<I,S> tree_t;

  S s = data::sort_psites_decreasing(input);
  tree_t tree(input, s, c4());

  typedef mln_ch_value_(I, unsigned) A;
  typedef morpho::attribute::card<I> card_t;
  A a = morpho::tree::compute_attribute_image(card_t (), tree);



  {
    typedef std::map<mln_value_(A), unsigned> F;
    F f = morpho::tree::compute_attribute_curve_beta(tree, a);

    unsigned count;
    mln_ch_value_(A, bool) activity;
    initialize(activity, a);
    F::iterator it;
    for (it = f.begin(); it != f.end(); ++it)
      {
	mln_up_node_piter_(tree_t) n(tree);
	data::fill(activity, true);
	count = 0;
	for_all(n)
	  if (a(n) >= it->first && activity(n))
	    {
	      morpho::tree::propagate_node_to_ancestors(n, tree, activity, false);
	      count++;
	    }

	std::cout << it->first << " -> " << it->second << " = " << count
		  << std::endl;
	mln_assertion(count == it->second);
    }
  }

  
}


