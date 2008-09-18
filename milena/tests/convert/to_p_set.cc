// Copyright (C) 2007. 2008 EPITA Research and Development Laboratory
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

/*! \file tests/convert/to_p_set.cc
 *
 * \brief Tests on mln::convert::to_p_set.
 */

#include <mln/core/alias/point2d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/sub_image.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/box2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/inplace.hh>

#include <mln/level/fill.hh>

#include <mln/convert/to_p_set.hh>
#include <mln/util/ord.hh>

using namespace mln;

  void test(p_set<point2d> ref, p_set<point2d> cmp)
  {
    mln_assertion(ref.nsites() == cmp.nsites());
    for (unsigned i = 0; i < ref.nsites(); ++i)
      mln_assertion(ref[i] == cmp[i]);
  }

int main()
{
  point2d
    a(1, 0),
    b(0, 1),
    c(-1,0),
    d(0,-1);

  p_set<point2d> ref;
  ref.insert(a);
  ref.insert(b);
  ref.insert(c);
  ref.insert(d);
  // Reference constructed.

  // Nbh :
  neighb2d nbh = c4();
  p_set<point2d> test_nbh = convert::to_p_set(nbh);
  test(ref, test_nbh);

  // Image :
  image2d<bool> ima(make::box2d(-6, -6, 6, 6));
  level::fill(ima, false);
  level::fill(inplace(ima | ref), true);
  p_set<point2d> test_ima = convert::to_p_set(ima);
  test(ref, test_ima);

  // Window :
  window2d win;
  win
    .insert(a - point2d::origin)
    .insert(b - point2d::origin)
    .insert(c - point2d::origin)
    .insert(d - point2d::origin);
  p_set<point2d> test_win = convert::to_p_set(win);
  test(ref, test_win);

  // std::set :
  std::set<point2d, util::ord<point2d> > set;

  set.insert(a);
  set.insert(b);
  set.insert(c);
  set.insert(d);

  p_set<point2d> test_set = convert::to_p_set(set);
  test(ref, test_set);
}
