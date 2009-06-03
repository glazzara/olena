// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file tests/win/hline2d.cc
///
/// Tests on mln::win::hline2d.

#include <cmath>

#include <mln/win/hline2d.hh>
#include <mln/convert/to_image.hh>
#include <mln/debug/println.hh>


int main()
{
  using namespace mln;

  const unsigned l = 5;
  win::hline2d hline(l);

  mln_assertion(hline.delta() == 2);

  for (def::coord x = -5; x <= 5; ++x)
    for (def::coord y = -5; y <= 5; ++y)
    {
      mln_assertion((std::abs(x) <= 2 && std::abs(y) <= 2) ||
		    ! hline.has(dpoint2d(x, y)));
      mln_assertion((x == 0) == (hline.has(dpoint2d(x, y))) ||
		    std::abs(y) > 2);
    }

  debug::println(convert::to_image(hline));
}
