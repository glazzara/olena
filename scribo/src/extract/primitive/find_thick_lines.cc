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

#include <mln/core/image/image2d.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/data/convert.hh>
#include <mln/debug/superpose.hh>

#include <scribo/debug/usage.hh>

#include <scribo/core/object_image.hh>
#include <scribo/primitive/extract/lines_h_thick.hh>
#include <scribo/primitive/extract/lines_v_thick.hh>

#include <scribo/primitive/extract/lines_h_single.hh>
#include <scribo/primitive/extract/lines_v_single.hh>

#include <mln/util/timer.hh>

const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. Objects are set to True." },
  { "vlength  ", "Minimum vertical line length." },
  { "vratio   "  "Minimum vertical ratio height/width." },
  { "hlength  ", "Minimum horizontal line length." },
  { "hratio   "  "Minimum horizontal ratio width/height." },
  {0, 0}
};


int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 7)
    return scribo::debug::usage(argv,
				"Extract thick horizontal and vertical lines.\
\n Common argument values: 150 10 150 10.",
				"<input.pbm> <vlength> <vratio> <hlength>\
 <hration> <output.ppm>",
				args_desc,
				"A color image. Horizontal lines are in red\
 and vertical lines in green.");

  trace::entering("main");

  typedef image2d<bool> I;
  I input;
  io::pbm::load(input, argv[1]);

  typedef image2d<value::label_16> L;

  value::label_16
    nhlines,
    nvlines;

  util::timer t;

  t.start();

  object_image(L)
    hlines = scribo::primitive::extract::lines_h_thick(input, c8(),
						       nhlines, atoi(argv[2])),
    vlines = scribo::primitive::extract::lines_v_thick(input, c8(),
						       nvlines, atoi(argv[2]));
  std::cout << "lines thick done" << std::endl;
  std::cout << t << std::endl;
  t.restart();


  hlines = scribo::primitive::extract::lines_h_single(hlines,
						      atoi(argv[2]),
						      10);

  vlines = scribo::primitive::extract::lines_v_single(vlines,
						      atoi(argv[2]),
						      10);

  std::cout << "lines single done" << std::endl;
  std::cout << t << std::endl;

  image2d<value::rgb8> out = debug::superpose(input, hlines, literal::red);
  out = debug::superpose(out, vlines, literal::green);

  io::ppm::save(out, argv[3]);

  trace::exiting("main");
}
