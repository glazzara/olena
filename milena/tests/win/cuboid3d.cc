// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/win/cuboid3d.cc
///
/// Tests on mln::win::cuboid3d.

#include <cmath>

#include <mln/win/cuboid3d.hh>
#include <mln/win/sym.hh>
#include <mln/convert/to_image.hh>
#include <mln/debug/println.hh>


int main()
{
  using namespace mln;

  const unsigned d = 7, h = 3, w = 5;
  win::cuboid3d cuboid(d, h, w);

  mln_assertion(cuboid.is_centered());
  mln_assertion(cuboid.is_symmetric());
  mln_assertion(cuboid == win::sym(cuboid));
  mln_assertion(cuboid.size() == d * h * w);

  mln_assertion(cuboid.delta() == 3);

  for (def::coord s = -7; s <= 7; ++s)
    for (def::coord x = -7; x <= 7; ++x)
      for (def::coord y = -7; y <= 7; ++y)
    {
      mln_assertion((std::abs(s) <= 3 && std::abs(x) <= 1 && std::abs(y) <= 2) ||
		    !cuboid.has(dpoint3d(s, x, y)));
      mln_assertion((std::abs(s) <= 3 && std::abs(x) <= 1 && std::abs(y) <= 2) ==
		    (cuboid.has(dpoint3d(s, x, y))));
    }
  
  debug::println(convert::to_image(cuboid));
}
