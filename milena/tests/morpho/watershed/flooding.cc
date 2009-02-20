// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/morpho/watershed/flooding.cc
///
/// Test on mln::morpho::watershed::flooding.

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>

#include <mln/morpho/watershed/flooding.hh>
#include <mln/level/transform.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/util/timer.hh>

#include "tests/data.hh"

struct f_16_to_8 : mln::Function_v2v< f_16_to_8 >
{
  typedef mln::value::int_u8 result;
  result operator()(const mln::value::int_u16& v) const
  {
    if (v == 0)
      return 0;
    return 1 + (v - 1) % 255;
  }
};


int main()
{
  using namespace mln;
  using value::int_u8;
  using value::int_u16;

  image2d<int_u8> input;
  io::pgm::load(input, MLN_IMG_DIR "/squares.pgm");

  typedef int_u16 L;
  L n_basins;
  {
    util::timer t;
    t.start();
    image2d<L> output = morpho::watershed::impl::generic::flooding(input, c4(), n_basins);
    std::cout << "gen:  " << t << std::endl;
    io::pgm::save(level::transform(output, f_16_to_8()),
		  "tmp_ref.pgm");
  }
  {
    util::timer t;
    t.start();
    image2d<L> output = morpho::watershed::impl::flooding_fastest(input, c4(), n_basins);
    std::cout << "fast: " << t << std::endl;
    io::pgm::save(level::transform(output, f_16_to_8()),
		  "tmp_out.pgm");
  }
}
