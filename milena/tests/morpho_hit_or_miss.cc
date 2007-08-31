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

/*! \file tests/morpho_hit_or_miss.cc
 *
 * \brief Test on mln::morpho::hit_or_miss.
 */

#include <mln/core/image2d_b.hh>
#include <mln/value/int_u8.hh>

#include <mln/core/win/rectangle2d.hh>
#include <mln/core/window2d.hh>
#include <mln/geom/shift.hh>
#include <mln/set/diff.hh>

#include <mln/io/load_pgm.hh>
#include <mln/io/save_pgm.hh>
#include <mln/level/fill.hh>
#include <mln/level/stretch.hh>

#include <mln/morpho/hit_or_miss.hh>


int main()
{
  using namespace mln;
  using value::int_u8;

  window2d win_hit = geom::shift(win::rectangle2d(3, 3),
				 make::dpoint2d(+1, +1));
  window2d win_miss = set::diff(win::rectangle2d(5, 5), win_hit);

  {
    bool hit[] = { 0, 0, 0, 0, 0,
		   0, 0, 0, 0, 0,
		   0, 0, 1, 1, 1,
		   0, 0, 1, 1, 1,
		   0, 0, 1, 1, 1 };
    window2d win_hit_ = make::window2d(hit);
    mln_precondition(win_hit_ == win_hit);

    bool miss[] = { 1, 1, 1, 1, 1,
		    1, 1, 1, 1, 1,
		    1, 1, 0, 0, 0,
		    1, 1, 0, 0, 0,
		    1, 1, 0, 0, 0 };
    window2d win_miss_ = make::window2d(miss);
    mln_precondition(win_miss_ == win_miss);
  }

  border::thickness = 2;

  image2d_b<int_u8>
    pic = io::load_pgm("../img/picasso.pgm"),
    out(pic.domain());

  morpho::hit_or_miss(pic, win_hit, win_miss, out);

  io::save_pgm(out, "out.pgm");
}
