// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

/*! \file
 *
 * \brief Tests on mln::box_runstart_piter.
 */
#include <mln/core/alias/box1d.hh>
#include <mln/core/alias/box2d.hh>
#include <mln/core/alias/box3d.hh>
#include <mln/core/box_runstart_piter.hh>

int main()
{
  using namespace mln;

  /// Test with box1d
  {
    box1d b1(point1d(40), point1d(42));
    box_runstart_piter<point1d> p1(b1);
    for_all(p1)
    {
      mln_assertion(p1[0] == 40);
      std::cout << p1 <<std::endl;
    }
    std::cout << "run_len : " << p1.run_length()<<std::endl;
    mln_assertion(p1.run_length() == 3);
  }


  /// Test with box2d
  {
    box2d b2(point2d(1,2), point2d(5,8));
    box_runstart_piter<point2d> p2(b2);
    int i = 1;
    for_all(p2)
    {
      mln_assertion(p2[1] == 2 && p2[0] == i++);
      std::cout << p2 <<std::endl;
    }
    std::cout << "run_len : " << p2.run_length()<<std::endl;
    mln_assertion(p2.run_length() == 7);
  }

//   Test with image 3d
  {
    box3d b3(point3d(1,2,3), point3d(5,8,7));
    box_runstart_piter<point3d> p3(b3);
    int i = 1;
    int j = 2;
    for_all(p3)
    {
      std::cout << p3 << std::endl;
       if (i++ == 5)
 	i = 1;
       if (j++ == 8)
 	j = 2;
    }
    std::cout << "run_len : " << p3.run_length() << std::endl;
    mln_assertion(p3.run_length() == 5);
  }

}
