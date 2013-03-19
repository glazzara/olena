// Copyright (C) 2009, 2010, 2011, 2013 EPITA Research and Development
// Laboratory (LRDE)
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
#include <mln/io/magick/load.hh>
#include <mln/io/ppm/save.hh>

#include <scribo/preprocessing/split_bg_fg.hh>
#include <scribo/debug/usage.hh>


const char *args_desc[][2] =
{
  { "input.*", "An image." },
  { "lambda", "Lambda value. (FIX Description)" },
  { "delta", "Delta value. (FIX Description)" },
  { "fg.ppm", "The foreground image (1st output)." },
  { "bg.ppm", "The background image (2nd output)." },
  {0, 0}
};



int main(int argc, char *argv[])
{
  mln_trace("main");
  using namespace mln;

  if (argc != 6)
    return scribo::debug::usage(argv,
				"Split background and foreground.",
				"input.* lambda delta fg.ppm bg.ppm",
				args_desc);

  typedef image2d<value::rgb8> I;
  I input;
  io::magick::load(input, argv[1]);

  util::couple<I,I>
    bg_fg = scribo::preprocessing::split_bg_fg(input,
					       atoi(argv[2]),
					       atoi(argv[3]));
  io::ppm::save(bg_fg.first(), argv[5]);
  io::ppm::save(bg_fg.second(), argv[4]);

}
