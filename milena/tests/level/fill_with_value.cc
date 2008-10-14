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

/*! \file tests/level/fill_with_value.cc
 *
 * \brief Tests on mln::level::fill_with_value
 */

#include <mln/level/fill_with_value.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/flat_image.hh>
#include <mln/core/image/image_if.hh>

#include <mln/fun/p2b/chess.hh>

int main()
{
  using namespace mln;
  const unsigned size = 100;

  {
    image2d<unsigned char> ima(size, size);
    level::fill_with_value(ima, 51);
    box2d::piter p(ima.domain());
    for_all(p)
      mln_assertion(ima(p) == 51);
  }


  {
    flat_image<short, box2d> ima(5, make::box2d(2, 3));
    level::fill_with_value(ima, 51);
    box2d::piter p(ima.domain());
    for_all(p)
      mln_assertion(ima(p) == 51);
  }


  {
    typedef image2d<unsigned char> I;
    typedef image_if<I, fun::p2b::chess_t> II;

    I ima(size, size);
    level::fill_with_value(ima, 51);

    II ima_if = ima | fun::p2b::chess;
    level::fill_with_value(ima_if, 42);

    II::piter p(ima_if.domain());
    for_all(p)
      mln_assertion(ima_if(p) == 42);
  }
}
