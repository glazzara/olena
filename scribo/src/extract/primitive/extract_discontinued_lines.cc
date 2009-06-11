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
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pbm/all.hh>
#include <mln/data/convert.hh>

#include <scribo/extract/primitive/lines_h_discontinued.hh>
#include <scribo/extract/primitive/lines_v_discontinued.hh>
#include <scribo/debug/usage.hh>

const char *args_desc[][2] =
{
  { "input.pbm", "A binary image." },
  { "length", "   Minimum line length." },
  { "rank", "     Filter rank." },
  {0, 0}
};


int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 5)
    return usage(argv, "Extract discontinued horizontal and vertical lines",
		 "input.pbm length rank output.pbm",
		 args_desc, "A binary image of horizontal and vertical lines.");

  trace::entering("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  value::label_16 nhlines;
  image2d<bool> hlines
    = data::convert(bool(),
		     scribo::extract::primitive::lines_h_discontinued(input,
								      c8(),
								      nhlines,
								      atoi(argv[2]),
								      atoi(argv[3])));
  value::label_16 nvlines;
  image2d<bool> vlines
    = data::convert(bool(),
		     scribo::extract::primitive::lines_v_discontinued(input,
								      c8(),
								      nvlines,
								      atoi(argv[2]),
								      atoi(argv[3])));

  data::fill((hlines | pw::value(vlines)).rw(), true);
  io::pbm::save(hlines, argv[4]);

  trace::exiting("main");
}
