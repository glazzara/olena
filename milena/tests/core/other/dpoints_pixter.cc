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

/// \file tests/core/other/dpoints_pixter.cc
/// \brief Test on mln::dpoints_fwd_pixter and mln::dpoints_bkd_pixter.

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/win/segment1d.hh>
#include <mln/win/rectangle2d.hh>
#include <mln/win/cuboid3d.hh>

#include <mln/make/pixel.hh>


template <typename I, typename W>
void test_fill(I& ima, const W& win, unsigned nsites)
{
  mln_piter(I) p(ima.domain());
  mln_fwd_qixter(I, W) fq(ima, win, p);
  for_all(p)
  {
    unsigned i = 0;
    for_all(fq)
      ++i, fq.val() = 51;
    mln_assertion(i == nsites);
  }
  mln_bkd_qixter(I, W) bq(ima, win, p);
  for_all(p)
  {
    unsigned i = 0;
    for_all(bq)
      ++i, bq.val() = 42;
    mln_assertion(i == nsites);
  }
}

// FIXME: Test promotion and other constructions.

template <typename P, typename W>
void test_pixel(const P& pxl, const W& win)
{
  mln_fwd_qixter(mln_image(P), W) fq(pxl, win);
  for_all(fq)
    fq.val() = 2 * fq.val();
  mln_bkd_qixter(mln_image(P), W) bq(pxl, win);
  for_all(bq)
    bq.val() = bq.val() + 1;
}

int main()
{
  using namespace mln;

  border::thickness = 1;

  // 1-D case.
  {
    typedef image1d<int> I;
    I ima(6);

    win::segment1d seg(3);
    point1d p = point1d(3);

    test_fill(ima, seg, 3);
    test_pixel(make::pixel(ima, p), seg);
  }

  // 2-D case.
  {
    typedef image2d<int> I;
    I ima(5, 5);

    win::rectangle2d rect(3, 3);
    point2d p = point2d(1, 1);

    test_fill(ima, rect, 3 * 3);
    test_pixel(make::pixel(ima, p), rect);
  }

  // 3-D case.
  {
    typedef image3d<int> I;
    I ima(3, 4, 5);

    win::cuboid3d cuboid(3, 3, 3);
    point3d p = point3d(2, 1, 3);

    test_fill(ima, cuboid, 3 * 3 * 3);
    test_pixel(make::pixel(ima, p), cuboid);
  }

}
