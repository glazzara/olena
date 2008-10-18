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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/core/image/cast_image.hh>

#include <mln/value/int_u8.hh>

#include "resize.hh"
#include <mln/linear/convolve.hh>
#include <mln/linear/gaussian.hh>

#include <mln/trace/all.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/core/alias/w_window2d_float.hh>


int main(int argc, char** argv)
{
  mln::trace::quiet = false;

  using namespace mln;
  using value::int_u8;

  image2d<bool> input;
  image2d<int_u8> output;
  image2d<int_u8> output2;

  if (argc != 3)
    return 1;



  io::pbm::load(input, argv[1]);


  // Step 1: Enlarge input.
  output = geom::resize(cast_image<int_u8>(input), 4);

  // Step 2: Blur.
  initialize(output2, output);
  linear::gaussian(output, 15, output2);

  io::pgm::save(output2, argv[2]);
}
