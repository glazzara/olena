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
#include <mln/io/pbm/all.hh>
#include <mln/data/convert.hh>
#include <mln/util/timer.hh>

#include <scribo/text/extract_lines_wo_merge.hh>
#include <scribo/primitive/extract/alignments.hh>
#include <scribo/primitive/remove/separators.hh>
#include <scribo/debug/usage.hh>

const char *args_desc[][2] =
{
  { "input.pbm", "A binary image without vertical or horizontal separators." },
  { "output.pbm", "Output image." },
  { "enable_debug", "0 or 1 (default 0)" },
  {0, 0}
};


int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;

  if (argc != 3 && argc != 4)
    return scribo::debug::usage(argv,
				"Extract delimitors based on alignments.",
				"input.pbm output.pbm [enable_debug]",
				args_desc);

  trace::entering("main");

  if (argc > 3 && atoi(argv[3]))
    scribo::debug::logger().set_level(scribo::debug::All);

  float dmax_ratio = 3;
  unsigned delta_pixel = 3;

  typedef image2d<scribo::def::lbl_type> L;
  document<L> doc(argv[1]);
  doc.open();

  // Vertical and horizontal separators
  image2d<bool> input = data::convert(bool(), doc.image());
  image2d<bool> separators = primitive::extract::separators(input, 81);
  input = primitive::remove::separators(input, separators);
  input = preprocessing::denoise_fg(input, c8(), 3);
  doc.set_binary_image(input);

  // Extract lines
  line_set<L>
    lines = scribo::text::extract_lines_wo_merge(doc, c8(), separators);
  doc.set_paragraphs(scribo::make::paragraph(lines));

  util::timer t;
  t.start();
  mln::util::couple<component_set<L>, mln_ch_value_(L,bool)>
    res = primitive::extract::alignments(doc, dmax_ratio, delta_pixel);
  t.stop();
  std::cout << t << std::endl;

  io::pbm::save(res.second(), argv[2]);

  trace::exiting("main");
}
