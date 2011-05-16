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

#include <scribo/primitive/extract/lines_h_thick_and_thin.hh>
#include <scribo/preprocessing/rotate_90.hh>

#include <scribo/debug/usage.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/labeling/compute.hh>
#include <mln/labeling/foreground.hh>


const char *args_desc[][2] =
{
  { "input.pbm", "A binary image." },
  { "output.pbm", "   A binary image with thick and thin horizontal"
    " separators." },
  { "length", "The minimum length of the lines. (default : 101)" },
  { "delta", "The lookhead distance between a line pixel and the background."
    " (default : 3)" },
  {0, 0}
};


int main(int argc, char *argv[])
{
  if (argc != 3 && argc != 4 && argc != 5)
        return scribo::debug::usage(argv,
				"Extract thick and thin horizontal lines",
				"input.pbm output.pbm [length] [delta]",
				args_desc);

  unsigned
    length = 101,
    delta = 3;

  if (argc >= 4)
    length = atoi(argv[3]);
  if (argc >= 5)
    delta = atoi(argv[4]);

  using namespace mln;
  using namespace scribo;

  typedef image2d<bool> I;

  I input;
  io::pbm::load(input, argv[1]);

  // Cleanup components on borders
  {
    typedef scribo::def::lbl_type V;
    V nlabels;
    mln_ch_value_(I,V) lbl = labeling::foreground(input, c8(), nlabels);
    mln::util::array<box2d>
      bbox = labeling::compute(accu::shape::bbox<point2d>(), lbl, nlabels);

    const box2d& b = input.domain();
    for_all_ncomponents(e, nlabels)
      if (bbox(e).pmin().row() == b.pmin().row()
	  || bbox(e).pmax().row() == b.pmax().row()
	  || bbox(e).pmin().col() == b.pmin().col()
	  || bbox(e).pmax().col() == b.pmax().col())
	data::fill(((input | bbox(e)).rw() | (pw::value(lbl) == pw::cst(e))).rw(), false);
  }

  I hseparators = primitive::extract::lines_h_thick_and_thin(
    input, length, delta);
  I vseparators = preprocessing::rotate_90(
      primitive::extract::lines_h_thick_and_thin(
	preprocessing::rotate_90(input), length, delta, 0.05, 0.80, 2), false);

  I separators = duplicate(vseparators);
  separators += hseparators;

  io::pbm::save(separators, argv[2]);
}
