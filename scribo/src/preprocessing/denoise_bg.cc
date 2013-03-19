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
#include <mln/io/pbm/all.hh>

#include <scribo/debug/usage.hh>
#include <scribo/preprocessing/denoise.hh>



const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'True' for objects, 'False'\
for the background." },
  { "bg_min_card", "The minimum neighbor count to be set to false." },
  {0, 0}
};


int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 4)
    return scribo::debug::usage(argv,
				"Remove noise in background using c4 neighbor.",
				"input.pbm bg_min_card output.pbm",
				args_desc);

  mln_trace("main");

  typedef image2d<bool> I;
  I input;
  io::pbm::load(input, argv[1]);

  unsigned bg_min_card = atoi(argv[2]);

  I output = scribo::preprocessing::denoise_bg(input, c4(), bg_min_card);

  io::pbm::save(output, argv[3]);

}
