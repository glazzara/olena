// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

/*! \file tests/core/other/line_piter.cc
 *
 * \brief Tests on mln::line_piter.
 */

//FIXME: replace by the necessary include
#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/line_piter.hh>

int main()
{
  using namespace mln;

  const unsigned border = 2;

  /// Test with image 1d
  {
    box1d b1(point1d(5), point1d(42));
    image1d<int> f1(b1, border);
    image1d<int>::line_piter p1(f1.domain());
    for_all(p1)
    {
      mln_assertion(p1[0] == 5);
      std::cout << p1 <<std::endl;
    }
  }


  /// Test with image 2d
  {
    box2d b2(point2d(1,2), point2d(5,8));
    image2d<int> f2(b2, border);

    image2d<int>::line_piter p2(f2.domain());
    int i = 1;
    for_all(p2)
    {
      mln_assertion(p2[1] == 2 && p2[0] == i++);
      std::cout << p2 <<std::endl;
    }
  }

  /// Test with image 3d
  {
    box3d b3(point3d(1,2,3), point3d(5,8,7));
    image3d<int> f3(b3, border);

    image3d<int>::line_piter p3(f3.domain());
    int i = 1;
    int j = 2;
    for_all(p3)
    {
      mln_assertion( p3[0] == i && p3[1] == j && p3[2] == 3);
      std::cout << p3 << std::endl;
      if (i++ == 5)
	i = 1;
      if (j++ == 8)
	j = 2;
    }
  }

}
