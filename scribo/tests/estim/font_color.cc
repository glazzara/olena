// Copyright (C) 2011, 2013 EPITA Research and Development Laboratory (LRDE).
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.

/// \file

#include <cstdio>
#include <cstdlib>

#include <mln/core/image/image2d.hh>
#include <mln/data/compare.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pbm/load.hh>

#include <mln/value/rgb8.hh>

#include <scribo/estim/font_color.hh>

#include "tests/data.hh"

using namespace mln;

int main()
{
  image2d<value::rgb8> input_rgb;
  io::ppm::load(input_rgb, SCRIBO_IMG_DIR "/phillip.ppm");
  image2d<bool> input_bin;
  io::pbm::load(input_bin, SCRIBO_IMG_DIR "/phillip.pbm");

  value::rgb8 val = scribo::estim::font_color(input_rgb, input_bin);

  mln_assertion(val == value::rgb8(77, 146, 140));
}
