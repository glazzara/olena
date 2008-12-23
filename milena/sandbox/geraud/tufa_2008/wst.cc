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

/// \file wst.cc

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/var.hh>

#include <mln/value/int_u8.hh>

#include <mln/data/fill.hh>
#include <mln/level/replace.hh>
#include <mln/morpho/meyer_wst.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm output.pgm" << std::endl;
  std::abort();
}


int main(int argc, char* argv [])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 3)
    usage(argv);


  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);

  int_u8 nbasins;
  image2d<int_u8> output = morpho::meyer_wst(input, c4(), nbasins);
  std::cout << "n basins = " << nbasins << std::endl;
  io::pgm::save(output, argv[2]);
}
