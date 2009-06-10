// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/util/lemmings.hh>
#include <mln/core/image/image2d.hh>


int main ()
{
  using namespace mln;

  typedef image2d<int> I;

  int vals[4][4] = {{2, 2, 6, 6},
		    {2, 2, 6, 6},
		    {3, 3, 4, 4},
		    {3, 3, 4, 4}};

  I ima = make::image<int>(vals);
  
  point2d
    pt1(1, 0),
    pt2(0, 2),
    pt3(2, 3),
    pt4(3, 1),
    pt5(1, 1);

  int
    vl1 = ima(pt1),
    vl2 = ima(pt2),
    vl3 = ima(pt3),
    vl4 = ima(pt4),
    vl5 = ima(pt5);

  point2d
    ptl1 = util::lemmings(ima, pt1, right, vl1),
    ptl2 = util::lemmings(ima, pt2, down,  vl2),
    ptl3 = util::lemmings(ima, pt3, left,  vl3),
    ptl4 = util::lemmings(ima, pt4, up,    vl4),
    ptl5 = util::lemmings(ima, pt5, up,    vl5);

  mln_assertion(ptl1 == point2d(1, 2));
  mln_assertion(ptl2 == point2d(2, 2));
  mln_assertion(ptl3 == point2d(2, 1));
  mln_assertion(ptl4 == point2d(1, 1));
  mln_assertion(ptl5 == point2d(-1, 1));
}
