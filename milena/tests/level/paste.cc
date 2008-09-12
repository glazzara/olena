// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

/*! \file tests/level/paste.cc
 *
 * \brief Tests on mln::level::paste.
 */

#include <mln/core/image/image2d.hh>
// #include <mln/core/image/image3d.hh>
#include <mln/core/image/sub_image.hh>

#include <mln/level/fill.hh>
#include <mln/level/paste.hh>
#include <mln/level/compare.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>


int main()
{
  using namespace mln;

  // tests in two dimension
  {
    box2d b(make::point2d(1,2), make::point2d(2,4));
    image2d<int> ima(b, 2);
    debug::iota(ima);

    box2d b2(make::point2d(-1,-2), make::point2d(3,6));
    image2d<int> ima2(b2, 0);
    debug::iota(ima2);

    level::paste(ima, ima2); // Fast version.
    assert(ima == (ima2 | b));

    level::impl::generic::paste(ima, ima2); // Not so fast version...
    assert(ima == (ima2 | b));
  }

//   // tests in three dimension
//   {
//     box3d b(make::point3d(1,2, 1), make::point3d(2,4, 3));
//     image3d<int> ima(b, 2);
//     debug::iota(ima);

//     box3d b2(make::point3d(-1,-2, -1), make::point3d(3,6, 3));
//     image3d<int> ima2(b2, 2);
//     debug::iota(ima2);

//     level::paste(ima, ima2); // Fast version.
//     assert(ima == (ima2 | b));

//     level::impl::generic::paste(ima, ima2); // Not so fast version...
//     assert(ima == (ima2 | b));
//   }
}
