// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file tests/win/rectangle2d.cc
 *
 * \brief Tests on mln::win::rectangle2d.
 */

#include <cmath>
#include <mln/win/rectangle2d.hh>
#include <mln/geom/sym.hh>

#include <mln/convert/to_image.hh>

#include <mln/debug/println.hh>

int main()
{
  using namespace mln;

  const unsigned h = 3, w = 5;
  win::rectangle2d rec(h, w);

  mln_assertion(rec.is_centered());
  mln_assertion(rec.is_symmetric());
  mln_assertion(rec == geom::sym(rec));
  mln_assertion(rec.size() == h * w);

  mln_assertion(rec.delta() == 2);

  for (int x = -5; x <= 5; ++x)
    for (int y = -5; y <= 5; ++y)
    {
      mln_assertion((abs(x) <= 1 && abs(y) <= 2) == (rec.has(dpoint2d(x, y))));
    }
  
  debug::println(convert::to_image(rec));
}

