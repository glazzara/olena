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

/*! \file tests/core/rle_image.cc
 *
 * \brief Test on mln::core::value_enc_image.hh.
 */

#include <mln/core/image2d.hh>
#include <mln/core/value_enc_image.hh>
#include <iostream>


int main()
{
  using namespace mln;

  /// Basic test
  {
    typedef value_enc_image<point2d, int> ima_type;
    typedef p_runs_<point2d> runs;

    p_runs_<point2d> pruns0;
    p_runs_<point2d> pruns1;
    p_runs_<point2d> pruns2;

    pruns0.insert(p_run<point2d>(make::point2d(0, 0), 2));
    pruns1.insert(p_run<point2d>(make::point2d(2, 4), 7));
    pruns1.insert(p_run<point2d>(make::point2d(18, 42), 5));
    pruns1.insert(p_run<point2d>(make::point2d(50, 76), 2));
    pruns1.insert(p_run<point2d>(make::point2d(17,40), 6));
    pruns2.insert(p_run<point2d>(make::point2d(10,10), 5));

    ima_type ima;
    ima.insert(pruns0, 0);
    ima.insert(pruns2, 2);
    ima.insert(pruns1, 1);

    mln_piter_(ima_type) piter (ima.domain());
    int i = 0;
    int nb = 0;
    for_all(piter)
    {
      assert(ima(piter) == i);

      ++nb;
      if (nb == 2)
	i = 2;
      if (nb == 7)
	i = 1;
    }
  }
}
