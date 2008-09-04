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

/*! \file tests/display/color_pretty.cc
 *
 * \brief Tests on mln::display::color::pretty.
 */

# include <mln/core/image/image2d.hh>
# include <mln/value/int_u8.hh>
# include <mln/level/fill.hh>
# include <mln/core/site_set/p_set.hh>
# include <mln/core/image/sub_image.hh>
# include <mln/core/image/inplace.hh>
# include <mln/value/rgb8.hh>
# include <mln/display/color_pretty.hh>
# include <mln/level/compare.hh>


int main()
{
  using namespace mln;
  using value::int_u8;

  /// Test on color_pretty.
  {
    image2d<int_u8> ima (2, 2);
    level::fill (ima, 51);

    p_set<point2d > s;
    s.insert(point2d(0, 0));
    s.insert(point2d(1, 1));

    sub_image<image2d<int_u8>, p_set<point2d > > input = inplace(ima | s);
    image2d<value::rgb8> out = display::color_pretty(input);

    value::rgb8 vs[2][2] = {
      {value::rgb8(51, 51, 51), value::rgb8(255, 0, 0) },
      {value::rgb8(255, 0, 0), value::rgb8(51, 51, 51)}
    };

    image2d<value::rgb8> ref (make::image2d(vs));
    mln_assertion (ref == out);
  }


  /// Test on color_pretty_rgb.
  {
    image2d<value::rgb8> ima (2, 2);

    p_set<point2d > s1;
    p_set<point2d > s2;
    p_set<point2d > s3;

    s1.insert(point2d(0, 0));
    s2.insert(point2d(0, 1));
    s3.insert(point2d(1, 0));

    image2d<value::rgb8> out = display::color_pretty_rgb(ima, s1, s2, s3);

    value::rgb8 vs[2][2] = {
      {value::rgb8(255, 0, 0), value::rgb8(0, 255, 0) },
      {value::rgb8(0, 0, 255), value::rgb8(0, 0, 0)}
    };

    image2d<value::rgb8> ref (make::image2d(vs));
    mln_assertion (ref == out);
  }
}
