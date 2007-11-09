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

/*! \file tests/color_pretty.cc
 *
 * \brief Tests on mln::display::color::pretty.
 */

# include <mln/core/image2d.hh>
# include <mln/io/pbm/load.hh>
# include <mln/make/win_chamfer.hh>
# include <mln/geom/chamfer.hh>
# include <mln/value/rgb8.hh>
# include <mln/core/sub_image.hh>
# include <mln/core/image_if_value.hh>
# include <mln/core/inplace.hh>
# include <mln/core/w_window2d_int.hh>
# include <mln/display/color_pretty.hh>
# include <mln/io/ppm/save.hh>
# include <mln/core/p_set.hh>


int main()
{
  using namespace mln;

  unsigned max = 51;


  image2d<bool> input = io::pbm::load("../img/toto.pbm");

  // Create a weighted windows :
  // 0 2 0
  // 2 p 2
  // 0 2 0
  const w_window2d_int& w_win = make::mk_chamfer_3x3_int<2, 0> ();

  // Call chamfer for a distance image.
  image2d<unsigned> tmp = geom::chamfer(input, w_win, max);

  p_set<point2d > s1;
  p_set<point2d > s2;
  p_set<point2d > s3;


//   typedef image_if_value<image2d<unsigned> > I;
//   {
//     I t = inplace (tmp | 4);
//     mln_piter(image_if_value<image2d<unsigned> >) p (t.domain ());
//     for_all (p)
//       s1.insert (p);
//   }

//  {
//     I tmp = inplace (tmp | 6);
//     mln_point(I) p (tmp.domain ());
//     for_all (p)
//       s2.insert (p);
//   }

//   {
//     I tmp = inplace (tmp | 8);
//     mln_point(I) p (tmp.domain ());
//     for_all (p)
//       s3.insert (p);
//   }

//   for (int i = 0; i < 100; ++i)
//     for (int j = 0; j < 100; ++j)
//       s1.insert(point2d(i, j));

  for (int i = 200; i < 300; ++i)
    for (int j = 0; j < 100; ++j)
      s2.insert(point2d(i, j));

  for (int i = 0; i < 100; ++i)
    for (int j = 200; j < 300; ++j)
      s3.insert(point2d(i, j));



  // Call color_pretty for sub_image.
  image2d<value::rgb8> out = display::color_pretty_rgb(tmp, s1, s2, s3);

  // Save output image from color in out.ppm.
  io::ppm::save(out, "out.ppm");

  std::cout << "out.ppm generate" << std::endl;
}
