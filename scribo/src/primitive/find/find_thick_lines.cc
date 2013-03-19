// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#include <scribo/primitive/extract/lines_h_thick.hh>
#include <scribo/primitive/extract/lines_v_thick.hh>

#include <scribo/primitive/extract/lines_h_single.hh>
#include <scribo/primitive/extract/lines_v_single.hh>


const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. Objects are set to True." },
  { "vlength  ", "Minimum vertical line length." },
  { "hlength  ", "Minimum horizontal line length." },
  {0, 0}
};


int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;

  if (argc != 5)
    return scribo::debug::usage(argv,
				"Extract thick horizontal and vertical "
				"lines.\n Common argument values: 150 150.",
				"<input.pbm> <vlength> <hlength> <output.ppm>",
				args_desc);

  mln_trace("main");

  typedef image2d<bool> I;
  I input;
  io::pbm::load(input, argv[1]);

  typedef image2d<value::label_16> L;

  value::label_16
    nhlines,
    nvlines;

  component_set<L>
    hlines = scribo::primitive::extract::lines_h_thick(input, c8(),
						       nhlines, atoi(argv[2])),
    vlines = scribo::primitive::extract::lines_v_thick(input, c8(),
						       nvlines, atoi(argv[3]));

  image2d<value::rgb8>
    out = mln::debug::superpose(input, hlines.labeled_image(), literal::red);
  out = mln::debug::superpose(out, vlines.labeled_image(), literal::green);

  io::ppm::save(out, argv[4]);

}
