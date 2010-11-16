// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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
///
/// \brief Tests on mln::accu::deviation.

#include <mln/core/image/image2d.hh>
#include <mln/debug/iota.hh>

#include <mln/accu/stat/deviation.hh>
#include <mln/data/compute.hh>

#include <mln/io/pgm/save.hh>
#include <mln/data/convert.hh>
#include <mln/value/int_u8.hh>

int main()
{
  using namespace mln;
  image2d<short> ima(3, 3);
  debug::iota(ima);

  accu::stat::deviation<int> dev(5);

  volatile float
    ref = 2.5819f,
    trunc_val = (unsigned(data::compute(dev, ima) * 10000.f) / 10000.f);

  mln_assertion(trunc_val == ref);
}
