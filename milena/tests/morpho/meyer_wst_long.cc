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

/// \file tests/morpho/meyer_wst_long.cc
// /\brief Test on mln::morpho::extrema_components

#include <iostream>

#include <mln/core/image/image_if.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>

#include <mln/morpho/gradient.hh>
#include <mln/morpho/closing_area.hh>
#include <mln/morpho/meyer_wst.hh>

#include <mln/convert/to_window.hh>

#include <mln/pw/cst.hh>
#include <mln/pw/value.hh>

#include <mln/display/color_pretty.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/save.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;
  using value::int_u16;

  image2d<int_u8> input;
  io::pgm::load(input, MLN_IMG_DIR "/lena.pgm");

  image2d<int_u8> gradient =
    morpho::gradient (input, convert::to_window(c4()));

  // Simplify the input image.
  image2d<int_u8> work(input.domain());
  morpho::closing_area(gradient, c4(), 200, work);

  // Perform a Watershed Transform.
  typedef int_u16 wst_val;
  wst_val nbasins;
  image2d<wst_val> ws = morpho::meyer_wst(work, c4(), nbasins);
  std::cout << "nbasins = " << nbasins << std::endl;

  // Save the image in color.
  image2d<value::rgb8> input_plus_ws =
    display::color_pretty(input | (pw::value(ws) != pw::cst(0)));
  io::ppm::save(input_plus_ws, "out.ppm");
}
