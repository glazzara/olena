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

#include <cassert>
#include <cmath>
#include <oln/core/3d/image3d.hh>
#include <oln/level/fill.hh>
#include <oln/debug/print.hh>
#include <ostream>
#include <oln/core/gen/rle_image.hh>
#include <oln/core/gen/rle_encode.hh>

float my_sinus(const oln::point3d& p)
{
  return std::sin(float(p.sli() + p.row() + p.col()));
}


int
main()
{
  using namespace oln;

  // Fill a 3D image with a cos.
  image3d<float> ima3d(100, 100, 100);

  level::fill(inplace(ima3d), my_sinus);


  // Transform it into a 3 states image
  // value < 0         => -1
  // value > 0         => 1
  // value next to 0   => 0
  image3d<short> ima3s(100, 100, 100);
  image3d<float>::piter p(ima3d.points());
  for_all(p)
    {
      if (ima3d(p) < 0.05 && ima3d(p) > -0.05)
	ima3s(p) = 0;
      else
      {
	if (ima3d(p) < 0)
	  ima3s(p) = -1;
	else
	  ima3s(p) = 1;
      }
    }

//   oln::debug::print(ima3s);
//   std::cout << std::endl;

  // Encode ima3s into a rle_image
  rle_image<point3d, short> ima3rle;


  ima3rle = rle_encode(ima3s);
  rle_image<point3d, short>::piter p1(ima3rle.points());
//   std::cout << "start test" << std::endl;
//   for_all(p1)
//     {
// //       std::cout << "point: " << p1.to_point() << std::endl;
// //       std::cout << "3s : " << ima3s(p1) << std::endl;
// //       std::cout << "rle: " << ima3rle(p1) << std::endl;
//       assert(ima3s(p1) == ima3rle(p1));
//     }

}
