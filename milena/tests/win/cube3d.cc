// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <cmath>

#include <mln/win/cube3d.hh>
#include <mln/convert/to_image.hh>
#include <mln/debug/println.hh>


int main()
{
  using namespace mln;

  const unsigned l = 5;
  win::cube3d cube(l);

  mln_assertion(cube.delta() == 2);

  for (def::coord s = -5; s <= 5; ++s)
    for (def::coord x = -5; x <= 5; ++x)
      for (def::coord y = -5; y <= 5; ++y)
      {
	mln_assertion((std::abs(x) <= 2 && std::abs(y) <= 2 && std::abs(s) <= 2) ||
		      ! cube.has(dpoint3d(s, y, x)));
	mln_assertion((std::abs(x) <= 2 && std::abs(y) <= 2 && std::abs(s) <= 2) ==
		      (cube.has(dpoint3d(s, y, x))));
      }

  debug::println(convert::to_image(cube));
}
