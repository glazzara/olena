// Copyright (C) 2008, 2009, 2010 EPITA Research and Development
// Laboratory (LRDE)
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
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <iostream>

#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>

#include <mln/morpho/gradient.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/meyer_wst.hh>

#include <mln/convert/to_window.hh>

#include <mln/pw/cst.hh>
#include <mln/pw/value.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/labeling/colorize.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> input;
  io::pgm::load(input, MLN_IMG_DIR "/lena.pgm");

  image2d<int_u8>
    grad = morpho::gradient(input, c4().win()),
    clo  = morpho::closing::area(grad, c4(), 200);

  // Perform a Watershed Transform.
  typedef value::label_16 L;
  L nbasins;
  image2d<L> ws = morpho::meyer_wst(clo, c4(), nbasins);

  // Save the image in color.
  io::ppm::save(labeling::colorize(value::rgb8(), ws, nbasins),
		"meyer_wst_long-out.ppm");
}
