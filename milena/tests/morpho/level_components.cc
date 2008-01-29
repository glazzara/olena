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

/// \file tests/morpho/level_components.cc
// /\brief Test on mln::morpho::level_components

#include <iostream>

#include <mln/core/image2d.hh>
#include <mln/core/window2d.hh>
#include <mln/core/neighb2d.hh>

#include <mln/convert/to_window.hh>

#include <mln/value/int_u8.hh>

#include <mln/morpho/level_components.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> input;
  io::pgm::load(input, MLN_IMG_DIR "/squares.pgm");
  unsigned ncomps;
  // FIXME: Do we really need to use a neighborood to express a 4-c window?
  image2d<int_u8> output =
    morpho::level_components<int_u8>(input, convert::to_window(c4()), ncomps);
  std::cout << "ncomps = " << ncomps << std::endl;
  io::pgm::save(output, "out.pgm");
}
