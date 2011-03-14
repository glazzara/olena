// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/box2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/arith/plus.hh>
#include <mln/labeling/foreground.hh>
#include <mln/labeling/compute.hh>

#include <scribo/primitive/extract/lines_h_pattern.hh>
#include <scribo/primitive/extract/lines_v_pattern.hh>
#include <scribo/debug/usage.hh>
#include <scribo/debug/bboxes_image.hh>


const char *args_desc[][2] =
{
  { "length", "   Minimum line length. (default: 101)" },
  { "delta", "    Distance between the object pixel and the background pixel"
    "(default: 4)" },
  {0, 0}
};


int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 7 && argc != 5)
    return scribo::debug::usage(argv,
				"Extract horizontal and vertical lines patterns",
				"input.pbm output.pbm input_wo_seps.pbm output.ppm length delta",
				args_desc);

  trace::entering("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  // Set default option value
  unsigned
    length = 101,
    delta = 4;
  if (argc > 5)
  {
    length = atoi(argv[5]);
    delta = atoi(argv[6]);
  }

  util::timer t;
  t.start();

  image2d<bool>
    h_lines = scribo::primitive::extract::lines_h_pattern(input, length, delta);

  image2d<bool>
    v_lines = scribo::primitive::extract::lines_v_pattern(input, length, delta);

  v_lines += h_lines;

  t.stop();
  std::cout << t << "s" << std::endl;

  // Save binary image.
  io::pbm::save(v_lines, argv[2]);

  // Save input without separators
  {
    image2d<bool> in_wo_seps = duplicate(input);
    data::fill((in_wo_seps | pw::value(v_lines)).rw(), false);
    io::pbm::save(in_wo_seps, argv[3]);
  }

  // Save bbox image
  value::int_u8 nlabels;
  image2d<value::int_u8> lbl = labeling::foreground(v_lines, c4(), nlabels);
  mln::util::array<box2d>
    bbox = labeling::compute(accu::shape::bbox<point2d>(), lbl, nlabels);
  io::ppm::save(scribo::debug::bboxes_image(input, bbox, literal::red), argv[4]);

  trace::exiting("main");
}
