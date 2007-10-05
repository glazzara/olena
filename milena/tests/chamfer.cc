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

/*! \file tests/chamfer.cc
 *
 * \brief Tests on all files in mln/geom/chamfer.hh .
 */

#include <mln/core/image2d_b.hh>
#include <mln/core/sub_image.hh>
#include <mln/core/image_if_value.hh>
#include <mln/core/inplace.hh>

#include <mln/level/fill.hh>
# include <mln/debug/println.hh>
# include <mln/core/w_window2d_int.hh>
# include <mln/core/w_window2d_float.hh>
# include <mln/core/image_if_interval.hh>

# include <mln/make/win_chamfer.hh>
# include <mln/geom/chamfer.hh>

int main()
{
  using namespace mln;
  unsigned max = 51;

  image2d_b<bool> ima(9, 9);

  {
    level::fill(ima, false);
    ima.at(4,4) = true;
    const w_window2d_int& w_win = win_chamfer::mk_chamfer_3x3_int<2, 0> ();
    image2d_b<unsigned> out = geom::chamfer(ima, w_win, max);
    debug::println(out | value::interval(0, 8));
  }

  {
    level::fill(ima, false);
    ima.at(4,4) = true;
    const w_window2d_int& w_win = win_chamfer::mk_chamfer_3x3_int<2, 3> ();
    image2d_b<unsigned> out = geom::chamfer(ima, w_win, max);
    debug::println(out | value::interval(0, 8));
  }

  {
    level::fill(ima, false);
    ima.at(4,4) = true;
    const w_window2d_int& w_win = win_chamfer::mk_chamfer_5x5_int<4, 6, 9> ();
    image2d_b<unsigned> out = geom::chamfer(ima, w_win, max);
    image2d_b<unsigned>::fwd_piter p(out.domain());
    for_all(p)
      out(p) = out(p) / 2;
    debug::println(out | value::interval(0, 8));
  }

}
