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
#include <mln/level/fill_with_image.hh>
#include <mln/level/compare.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/pw/image.hh>
#include <mln/core/image/flat_image.hh>
#include <mln/core/image/cast_image.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/image/sub_image.hh>
#include <mln/core/image/extension_val.hh>

#include <mln/fun/p2b/chess.hh>
#include <mln/fun/p2v/iota.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>
#include <mln/trace/all.hh>


int main()
{
  using namespace mln;
  const unsigned size = 100;

  // 2d tests
  {
    image2d<unsigned int> ima(size, size);
    image2d<unsigned int> ima2(size, size);
    debug::iota(ima2);

    level::fill_with_image(ima, ima2);
    mln_assertion(ima == ima2);
  }

  {
    box2d b(point2d(1,2), point2d(2,4));
    image2d<int> ima(b, 2);

    box2d b2(point2d(-1,-2), point2d(3,6));
    image2d<int> ima2(b2, 0);
    debug::iota(ima2);

    level::fill_with_image(ima, ima2);
    mln_assertion(ima == (ima2 | b));
  }

  {
    typedef image2d<unsigned char> I;
    typedef image_if<I, fun::p2b::chess_t> II;

    I ima(size, size);
    I ima2(size, size);
    level::fill_with_value(ima, 51);
    level::fill_with_value(ima2, 42);

    II ima_if = ima | fun::p2b::chess;
    level::fill_with_image(ima_if, ima2);

    II::piter p(ima_if.domain());
    for_all(p)
      mln_assertion(ima_if(p) == 42);

  }

  // tests in three dimension
  {
    box3d b(point3d(1,2,1), point3d(2,4,3));
    image3d<int> ima(b, 2);
    debug::iota(ima);

    box3d b2(point3d(-1,-2,-1), point3d(3,6,3));
    image3d<int> ima2(b2, 2);
    debug::iota(ima2);

    image3d<int> ima3(b, 2);

    level::fill_with_image(ima, ima2); // Not so fast version...
    mln_assertion(ima == (ima2 | b));

    level::fill_with_image(ima3, ima); // Fast version...
    mln_assertion(ima == ima3);
  }

  // image 1d test
  {
    image1d<unsigned short> ima(size);
    image1d<unsigned short> out(size);

    debug::iota(ima);
    level::fill_with_image(out, ima);

    mln_assertion(ima == out);
  }


  // pw image test
  {
    const pw::image<fun::p2v::iota_t, box2d> ima(fun::p2v::iota,
                                                 make::box2d(-2,-2, 15,15));
    image2d<short unsigned int> out(8, 8);

    level::fill(out, 0);
    level::fill_with_image(out, ima);
  }

  // flat image test
  {
    flat_image<short, box2d> ima(5, make::box2d(size, size));
    image2d<unsigned short> out(size, size);

    level::fill_with_value(ima, 51);
    level::fill_with_image(out, ima);

    mln_assertion(ima == out);
  }

  // image if test
  {
    typedef image2d<unsigned short> I;
    typedef image_if<I, fun::p2b::chess_t> II;

    I ima(size, size);
    I out(size, size);
    II ima_if = ima | fun::p2b::chess;

    level::fill_with_value(ima, 42);
    level::fill_with_value(out, 0);
    level::fill_with_image(ima_if, ima);

    mln_piter_(II) p(ima_if.domain());
    for_all(p)
      mln_assertion(ima_if(p) == ima(p));
  }

  // cast image test
  {
    typedef image2d<unsigned short> I;
    typedef cast_image_<int, I> II;

    I in(size, size);
    II cast(in);
    I out(size, size);

    level::fill(in, 51);
    level::fill(out, 42);

    level::fill_with_image(out, cast);

    mln_assertion(cast == out);
  }

  // sub_image test
   {
     typedef image2d<int> I;
     typedef sub_image< image2d<int>, box2d > II;

     I ima(size, size);
     I out(size, size);
     II sub_ima(ima, make::box2d(4,4, 10,10));


     level::fill(ima, 51);
     level::fill(out, 0);
     level::fill_with_image(sub_ima, ima);

     II::piter p(sub_ima.domain());
     for_all(p)
       mln_assertion(sub_ima(p) == ima(p));
   }

   // extended image test
  {
    typedef image2d<int> I;
    typedef extension_val< image2d<int> > II;
    typedef image2d<unsigned short> III;

    I ima(size, size);
    II extend_ima(ima, 5);
    III out(size, size);

    level::fill(ima, 51);
    level::fill_with_image(out, extend_ima);

    mln_assertion(out == extend_ima);
  }

}
