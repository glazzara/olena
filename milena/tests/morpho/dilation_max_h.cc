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

/*! \file tests/morpho/dilation_max_h.cc
 *
 * \brief Test on mln::morpho::dilatation_fast.
 */

#include <mln/core/image/image2d.hh>
#include <mln/win/rectangle2d.hh>
#include <mln/core/alias/window2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/morpho/dilation_fast.hh>

#include <mln/convert/to_p_array.hh>
#include <mln/convert/to_window.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  win::rectangle2d rec(21, 21);
  border::thickness = 66;

  image2d<int_u8> lena;
  io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");

  {
    p_array<point2d> vec = convert::to_p_array(rec, point2d::origin);
    window2d win = convert::to_window(vec);

    image2d<int_u8> out(lena.domain());
    morpho::dilation_fast(lena, win, out);
    io::pgm::save(out, "out.pgm");
  }
}
