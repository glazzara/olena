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
#include <mln/arith/plus.hh>
#include <mln/labeling/foreground.hh>

#include <scribo/primitive/extract/lines_h_pattern.hh>
#include <scribo/primitive/extract/lines_v_pattern.hh>
#include <scribo/primitive/extract/separators_nonvisible.hh>
#include <scribo/debug/usage.hh>
#include <scribo/debug/bboxes_image.hh>
#include <scribo/core/document.hh>
#include <scribo/core/def/lbl_type.hh>

#include <scribo/io/xml/save.hh>

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

  if (argc != 8 && argc != 6)
    return scribo::debug::usage(argv,
				"Extract horizontal, vertical lines and stoppers",
				"input.pbm out_seps.pbm out_in_wo_seps.pbm "
				"out_seps_bbox.ppm out_visible_seps.pbm length delta",
				args_desc);

  mln_trace("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  // Set default option value
  unsigned
    length = 101,
    delta = 4;
  if (argc > 6)
  {
    length = atoi(argv[6]);
    delta = atoi(argv[7]);
  }

  util::timer t;

  t.start();
  image2d<bool>
    h_lines = scribo::primitive::extract::lines_h_pattern(input, length,
							  delta);

  image2d<bool>
    v_lines = scribo::primitive::extract::lines_v_pattern(input, length,
							  delta);

  v_lines += h_lines;

  t.stop();

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
  io::ppm::save(scribo::debug::bboxes_image(input, bbox, literal::red),
		argv[4]);

  // Save visible separators
  mln::io::pbm::save(v_lines, argv[5]);

  t.resume();

  // Non visible separators
  image2d<bool>
    nonvisible = scribo::primitive::extract::separators_nonvisible(input);

  t.stop();
  std::cout << t << "s" << std::endl;

  // // Saving stoppers data to XML
  // typedef image2d<scribo::def::lbl_type> L;
  // scribo::document<L> doc(argv[1]);
  // doc.open();
  // doc.set_whitespace_separators(nonvisible);
  // doc.set_line_separators(v_lines);
  // scribo::io::xml::save(doc, argv[5], scribo::io::xml::Full);

  // Save binary image.
  v_lines += nonvisible;
  mln::io::pbm::save(v_lines, argv[2]);

}
