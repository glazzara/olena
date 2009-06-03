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

/*! \file tests/value/stack.cc
 *
 * \brief Tests on mln::value::stack.
 */

#include <mln/core/image/image2d.hh>
#include <mln/value/stack.hh>


int main()
{
  using namespace mln;

  typedef image2d<int> I;

  box2d b = make::box2d(2, 2);
  image2d<int> ima5(b), ima1(b);

  point2d p = point2d(0, 0);
  algebra::vec<2, int> v = make::vec(5, 1);

  value::stack(ima5, ima1)(p) = v;
  mln_assertion(value::stack(ima5, ima1)(p) == v);
  mln_assertion(ima5(p) == 5 && ima1(p) == 1);

  value::stack_image<2, image2d<int> > s1(value::stack(ima5, ima1));

  value::stack_image<2, image2d<int> > s2;

  p = point2d(1, 1);
  s2 = s1;
  s2(p) = v;
  mln_assertion(s1(p) == v);
  mln_assertion(ima5(p) == 5 && ima1(p) == 1);
}
