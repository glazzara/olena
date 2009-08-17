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


// Results published in:
//
// `ICDAR2009 Handwriting Segmentation Contest`.
// B. Gatos, N. Stamatopoulos and G. Louloudis
// 2009 10th International Conference on Document Analysis and Recognition
//
// http://www.cvc.uab.es/icdar2009/papers/3725b393.pdf


#include <sandbox/icdar/2009/hsc/input_to_lines.hh>
#include <sandbox/icdar/2009/hsc/io/icdar/save.hh>

#include <mln/value/rgb8.hh>
#include <mln/labeling/colorize.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm output.dat [output.ppm]" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << "  input.pbm:  input 2D binary image (text is black; background is white)" << std::endl
	    << "  output.dat: line buffer (int; 0 is bg)." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 3 && argc != 4)
    usage(argv);

  trace::entering("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  typedef value::label<12> L;
  L n_lines;
  image2d<L> output = input_to_lines(input, n_lines, 0.7); // with 70%

  io::icdar::save(output, argv[2]);

  if (argc == 4)
    {
      io::ppm::save(labeling::colorize(value::rgb8(),
				    output,
				    n_lines),
		    argv[3]);
    }

  trace::exiting("main");
}
