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

# include "reconstructions_on_set.hh"
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/io/pbm/load.hh>
# include <mln/io/pbm/save.hh>

# include <tests/timer.hh>

void usage(char** argv)
{
  std::cerr << "Usage: " << argv[0] << " (-dilation|-erosion) marker.pbm mask.pbm" << std::endl;
  exit(1);
}

int main(int argc, char** argv)
{
  using namespace mln;
  typedef image2d<bool> I;

  image2d<bool> marker;
  image2d<bool> mask;
  image2d<bool> output;
  image2d<bool> tmp;


  if (argc < 3)
    usage(argv);

  io::pbm::load(marker, argv[2]);
  io::pbm::load(mask,   argv[3]);

  trace::quiet = false;

  if (std::string(argv[1]) == "-dilation")
  {
    timer t;
    t.start();
    tmp = reconstruction_on_set_by_dilation    (marker, mask, c4());
    std::cout << "====================> reconstruction " << t << std::endl;
    io::pbm::save(tmp, "r_dilation.pbm");

    t.start();
    tmp = reconstruction_on_set_by_dilation_alt(marker, mask, c4());
    std::cout << "====================> reconstruction alt " << t << std::endl;
    io::pbm::save(tmp, "r_dilation_alt.pbm");
  }
  else if (std::string(argv[1]) == "-erosion")
  {
    timer t;
    t.start();
    tmp = reconstruction_on_set_by_erosion     (marker, mask, c4());
    std::cout << "====================> reconstruction " << t << std::endl;
    io::pbm::save(tmp, "r_erosion.pbm");

    t.start();
    tmp = reconstruction_on_set_by_erosion_alt (marker, mask, c4());
    std::cout << "====================> reconstruction alt " << t << std::endl;
    io::pbm::save(tmp, "r_erosion_alt.pbm");
  }
  else
    usage(argv);
}
