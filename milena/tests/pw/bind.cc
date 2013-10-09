// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE).
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

/// \file
/// \brief Exercise mln::pw::bind with unary and binary functions
/// bound to point-wise expressions.

#include <mln/core/image/image2d.hh>

#include <mln/pw/all.hh>
#include <mln/fun/v2v/abs.hh>
#include <mln/fun/vv2v/max.hh>

#include <mln/arith/minus.hh>
#include <mln/debug/iota.hh>
#include <mln/test/predicate.hh>


int main()
{
  using namespace mln;

  image2d<int> pos_ints(3, 3);
  debug::iota(pos_ints);
  image2d<int> neg_ints = -pos_ints;

  // Exercise unary point-wise function binding.
  mln_assertion(test::predicate(pos_ints.domain(),
                                pw::bind(fun::v2v::abs<int>(),
                                         pw::value(neg_ints))
                                == pw::value(pos_ints)));

  // Exercise binary point-wise function binding.
  mln_assertion(test::predicate(pos_ints.domain(),
                                pw::bind(fun::vv2v::max<int>(),
                                         pw::value(pos_ints),
                                         pw::value(neg_ints))
                                == pw::value(pos_ints)));
}
