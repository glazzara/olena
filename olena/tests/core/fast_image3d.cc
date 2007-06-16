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

#include <cassert>
#include <oln/core/3d/image3d.hh>

using namespace oln;

void test(const image3d<int>& ima)
{
  point3d q;
  image3d<int>::piter p(ima.points());
  image3d<int>::index offset;

  assert(ima.offset_from_point(point3d(0, 0, 0)) == 0);

  for_all(p)
  {
    q = p;
    offset = ima.offset_from_point(q);
    q = ima.point_at_offset(offset);
    assert(q == p);
  }
}


int
main()
{
  image3d<int> ima(50, 50, 50), ima2(30, 50, 50), ima3(50, 30, 30);
  test(ima);
  test(ima2);
  test(ima3);
}
