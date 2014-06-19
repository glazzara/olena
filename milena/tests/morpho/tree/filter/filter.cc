// Copyright (C) 2009, 2014 EPITA Research and Development Laboratory (LRDE).
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
#include <mln/pw/all.hh>

#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/morpho/tree/component_tree.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/attribute/card.hh>

#include <mln/morpho/tree/filter/all.hh>
#include <mln/morpho/tree/propagate_representative.hh>

#include <mln/morpho/closing/area.hh>

#include "tests/data.hh"

#include <iostream>

int main()
{
  using namespace mln;
  using value::int_u8;


  typedef image2d<int_u8> I;
  I input;
  io::pgm::load(input, MLN_IMG_DIR "/tiny.pgm");

  typedef p_array< mln_site_(I) > S;
  typedef morpho::tree::data<I,S> tree_t;

  tree_t tree = morpho::tree::min_tree(input, c4());

  // Test with increasing attribute -> area closing.
  {
    typedef morpho::attribute::card<I> attribute_t;
    typedef mln_ch_value_(I, unsigned) A;

    A a = morpho::tree::compute_attribute_image(attribute_t (), tree);

    unsigned lambda = 5;
    typedef fun::geq_v2b_expr_< pw::value_<image2d<unsigned> >,
                                pw::cst_<unsigned> > predicate_t;
    predicate_t predicate = pw::value(a) >= pw::cst(lambda);
    I ref = morpho::closing::area(input, c4(), lambda);

    {
      I out = duplicate(input);
      morpho::tree::filter::min(tree, out, predicate);
      morpho::tree::propagate_representative(tree, out);
      mln_assertion(out == ref);
    }

    {
      I out = duplicate(input);
      morpho::tree::filter::max(tree, out, predicate);
      morpho::tree::propagate_representative(tree, out);
      mln_assertion(out == ref);
    }

    {
      I out = duplicate(input);
      morpho::tree::filter::direct(tree, out, predicate);
      morpho::tree::propagate_representative(tree, out);
      mln_assertion(out == ref);
    }

    // Not for subtractive strategy (removal procedure differs).
  }
}
