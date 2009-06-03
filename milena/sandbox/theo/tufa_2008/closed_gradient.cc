// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file sandbox/geraud/tufa/closed_gradient.cc

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/morpho/elementary/gradient.hh>
#include <mln/morpho/closing_volume.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda output.pgm" << std::endl;
  std::abort();
}


int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 4)
    usage(argv);

  image2d<int_u8> f;
  io::pgm::load(f, argv[1]);
  unsigned lambda = std::atoi(argv[2]);
  io::pgm::save(morpho::closing_volume(morpho::elementary::gradient(f, c4()),
				       c4(),
				       lambda),
		argv[3]);
}
