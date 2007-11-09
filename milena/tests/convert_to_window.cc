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

/*! \file tests/convert_to_window.cc
 *
 * \brief Tests on mln::convert::to_window.
 */

#include <mln/core/dpoint2d.hh>
#include <mln/core/image2d.hh>
#include <mln/core/sub_image.hh>
#include <mln/core/window2d.hh>
#include <mln/core/box2d.hh>
#include <mln/core/neighb2d.hh>
#include <mln/core/inplace.hh>

#include <mln/level/fill.hh>

#include <mln/convert/to_window.hh>
#include <mln/convert/to_p_set.hh>

using namespace mln;

  void test(window2d ref, window2d cmp)
  {
    mln_assertion(ref.ndpoints() == cmp.ndpoints());
    for (unsigned i = 0; i < ref.ndpoints(); ++i)
      mln_assertion(ref.dp(i) == cmp.dp(i));
  }

int main()
{
  dpoint2d
    a(1, 0),
    b(0, 1),
    c(-1,0),
    d(0,-1);

  window2d ref;
  ref.insert(a).insert(b).insert(c).insert(d);
  // Reference constructed.

  // Nbh :
  neighb2d nbh = c4();
  window2d test_nbh = convert::to_window(nbh);
  test(ref, test_nbh);

  // Image :
  image2d<bool> ima(make::box2d(-6, -6, 6, 6));
  level::fill(ima, false);
  level::fill(inplace(ima | convert::to_p_set(ref)), true);
  window2d test_ima = convert::to_window(ima);
  test(ref, test_ima);

  // Window :
  p_set<point2d> setp;
  setp
    .insert(point2d::origin + a)
    .insert(point2d::origin + b)
    .insert(point2d::origin + c)
    .insert(point2d::origin + d);
  window2d test_setp = convert::to_window(setp);
  test(ref, test_setp);

  // std::set :
  std::set<dpoint2d> set;
  set.insert(a);
  set.insert(b);
  set.insert(c);
  set.insert(d);
  window2d test_set = convert::to_window(set);
  test(ref, test_set);

  // FIXME: To upper window.
}
