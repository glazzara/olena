//Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)

// This file is part of Olena.

// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.

// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.

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

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/tree/propagate.hh>
#include <mln/morpho/tree/propagate_representative.hh>
#include <mln/morpho/tree/debug.hh>

# include <mln/io/pgm/save.hh>
# include <mln/io/pgm/load.hh>

#include <mln/accu/stat/mean.hh>

#include <mln/data/sort_psites.hh>

#include <map>
#include <iostream>


using namespace mln;
using value::int_u8;

int main(int argc, char** argv)
{

  if (argc < 1)
    abort();

  const char* finput = argv[1];

  // Image loadin'.
  typedef image2d<value::int_u8> I;
  I input;
  io::pgm::load(input, finput);


  // Tree construction.
  typedef p_array<mln_psite_(I)> S;
  typedef morpho::tree::data<I, S> T;

  S s = data::sort_psites_decreasing(input);
  T tree(input, s, c4());

  typedef std::map< int_u8, unsigned > M;

  M m = morpho::tree::debug::tree_height(tree);

  std::cout << "Height:\tLeaf Count:" << std::endl;
  for (M::iterator it = m.begin(); it != m.end(); ++it)
    std::cout << it->first << "\t" << it->second << std::endl;


  mln_ch_value_(I, int_u8) bro = morpho::tree::debug::tree_brother(tree);
  morpho::tree::propagate_representative(tree, bro);
  io::pgm::save(bro, "bro.pgm");
}
