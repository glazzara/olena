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

/*! \file tests/level_fill.cc
 *
 * \brief Tests on mln::level::fill
 */

#include <mln/core/image2d_b.hh>
#include <mln/level/fill.hh>
#include <mln/level/paste.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>


int main()
{
  using namespace mln;

  box2d b(make::point2d(1,2), make::point2d(2,4));
  image2d_b<int> ima(b, 2);
  debug::iota(ima);
  debug::println(ima);


  box2d b2(make::point2d(-1,-2), make::point2d(3,6));
  image2d_b<int> ima2(b2, 0);
  debug::iota(ima2);
  debug::println(ima2);

  trace::quiet = false;

  level::paste(ima, ima2); // Fast version.
  debug::println(ima2);

  level::impl::generic::paste_(ima, ima2); // Not so fast version...
}
