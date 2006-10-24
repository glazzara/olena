// Copyright (C) 2006 EPITA Research and Development Laboratory
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

/// Test the look-up table morpher.

#include <oln/basics2d.hh>
#include <oln/morpher/with_lut.hh>
#include <oln/value/color/rgb.hh>
#include <oln/level/fill.hh>
#include <oln/debug/print.hh>


// FIXME: Remove.
using namespace oln;

template <typename I>
void image_test(const oln::abstract::image<I>&)
{
  // Do nothing.
}

oln::value::color::rgb8 white(255, 255, 255);
oln::value::color::rgb8 blue (  0,   0, 255);

int main()
{
  using oln::value::color::rgb8;

  unsigned data[] = { 0, 1, 2, 0, 1, 2, 0, 1, 2};

  typedef image2d<unsigned> image_t;
  image_t ima(3, 3);
  level::fill(ima, data);
  debug::print(ima);

  typedef lookup_table<unsigned, rgb8> lut_t;
  lut_t lut;
  rgb8 c(16, 6, 4);
  lut.
    add(0, blue).
    add(1, white).
    add(2, c);
  std::cout << lut << std::endl;

  typedef morpher::with_lut<image_t, lut_t> lutimage_t;
  lutimage_t ima2 = ima + lut;

  // ima2 is an image, and can be as argument to to image routines.
  image_test(ima2);

  // FIXME: To be enabled later.
#if 0
  // it is value-wise accessible:
  ima2.value(c) = red;
#endif

  // let's look at it
  debug::print(ima2);
  std::cout << std::endl;

  // it is a 2D image so we have:
  point2d p(1, 1);
  std::cout << "ima2(p) =" << ima2(p) << std::endl;
  // or (likewise):
  std::cout << "ima2.at(1, 1) =" << ima2.at(1, 1) << std::endl;

  // FIXME: To be enabled later.
#if 0
  // FIXME...
  level::apply(ima2, fun); // 3 ops only !!!
#endif
}
