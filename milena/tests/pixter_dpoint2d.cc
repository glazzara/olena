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

/*! \file tests/pixter_dpoint2d.cc
 *
 * \brief Test on mln::dpoints_fwd_pixter.
 */

#include <cassert>
#include <iostream>

#include <mln/core/image2d_b.hh>
#include <mln/core/window.hh>
#include <mln/core/dpoints_pixter.hh>

#include <mln/level/fill.hh>


int main()
{
  using namespace mln;

  typedef image2d_b<int> I;
  typedef I::dpoint      D;
  typedef window<D>      W;

  typedef dpoints_fwd_pixter<I> qixter;

  const unsigned size = 20;
  I ima(size, size);

  const int value = 51;
  level::fill(ima, value);

  W win;
  win
    .insert(make::dpoint2d(0, -1))
    .insert(make::dpoint2d(0, -1))
    .insert(make::dpoint2d(1, 0))
    .insert(make::dpoint2d(1, 0));

  mln_piter_(I) p(ima.domain());
  qixter        qix(ima, win, p);

  for_all(p)
    if (p[0] > 0 && p[1] > 0 && p[0] < int(size - 1) && p[1] < int(size - 1))
      for_all(qix)
      	mln_assertion(*qix == value);
}
