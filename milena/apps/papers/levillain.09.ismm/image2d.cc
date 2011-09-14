// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <cstdlib>

#include <string>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>

#include <mln/morpho/gradient.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/labeling/colorize.hh>

#include "chain.hh"

int main(int argc, char* argv[])
{
  if (argc != 4)
    {
      std::cerr << "usage: " << argv[0] << " input.pgm lambda output.ppm"
		<< std::endl;
      std::exit(1);
    }
  std::string input_filename = argv[1];
  unsigned lambda = atoi(argv[2]);
  std::string output_filename = argv[3];

  using namespace mln;

  typedef value::int_u8 val;
  typedef value::label_8 label;
  // Input and output types.
  typedef image2d<val> input;
  typedef mln_ch_value_(input, label) output;
  neighb2d nbh = c4();
  window2d win = win_c4p();
  label nbasins;

  std::cout << c4() << std::endl;
  std::cout << win_c4p() << std::endl;

  // Load, process, save.
  input ima = io::pgm::load<val>(input_filename);
  // Gradient.
  /* FIXME: Unfortunately, we cannot write

       morpho::gradient(ima, win)

     here, since MM only uses windows (yet?), not neighborhoods.  And
     we cannot either write

       morpho::gradient(ima, nbh.win()),

     since that window would not contain the center site...
     Therefore, this function asks for a window WIN matching the
     neighborhood NBH, plus the center site.

     A neighborhood `n' should provide a method `win_p()' returning a
     window corresponding to `n' plus the center site.  */
  input g = morpho::gradient(ima, win);

#if 0
  // FIXME: get the name as argument.
  io::pgm::save(g, "apps/lena-g.pgm");
#endif

  // Chain.
  output s = chain(g, nbh, lambda, nbasins);
  io::ppm::save(labeling::colorize(value::rgb8(), s, nbasins),
		output_filename);
}
