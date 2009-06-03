// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

# include "reconstructions_on_function.hh"
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/value/int_u8.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pgm/save.hh>

void usage(char** argv)
{
  std::cerr << "Usage: " << argv[0] << " (-dilation|-erosion) marker.pgm mask.pgm" << std::endl;
  exit(1);
}

int main(int argc, char** argv)
{
  using namespace mln;

  if (argc < 3)
    usage(argv);

  typedef image2d<value::int_u8> I;
  I marker;
  I mask;
  I output;

  io::pgm::load(marker, argv[2]);
  io::pgm::load(mask,   argv[3]);

  if (std::string(argv[1]) == "-dilation")
  {
    io::pgm::save(reconstruction_on_function_by_dilation(marker, mask, c8()), "r_dilation.pgm");
//     io::pgm::save(reconstruction_on_function_by_dilation_slow(marker, mask, c8()), "r_dilation_ref.pgm");
  }
  else if (std::string(argv[1]) == "-erosion")
    io::pgm::save(reconstruction_on_function_by_erosion (marker, mask, c4()), "r_erosion.pgm");
  else
    usage(argv);
}
