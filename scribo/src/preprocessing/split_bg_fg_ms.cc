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
#include <mln/io/ppm/all.hh>

#include <mln/debug/filename.hh>

#include <mln/subsampling/subsampling.hh>

#include <scribo/preprocessing/split_bg_fg.hh>
#include <scribo/debug/usage.hh>


const char *args_desc[][2] =
{
  { "input.pbm", "A color image." },
  { "lambda", "Lambda value. (FIX Description)" },
  { "delta", "Delta value. (FIX Description)" },
  { "lambda_sub2", "Lambda value. (FIX Description)" },
  { "delta_sub2", "Delta value. (FIX Description)" },
  { "lambda_sub4", "Lambda value. (FIX Description)" },
  { "delta_sub4", "Delta value. (FIX Description)" },
  {0, 0}
};



int main(int argc, char *argv[])
{
  mln::trace::entering("main");
  using namespace mln;
  dpoint2d none(0, 0);

  if (argc != 9)
    return scribo::debug::usage(argv,
				"Split background and foreground.",
				"input.pbm lambda delta lambda_sub2 delta_sub2 lambda_sub3 delta_sub3 output_prefix",
				args_desc, "The background and foreground images.");

  mln::debug::internal::filename_prefix = argv[8];

  typedef image2d<value::rgb8> I;
  I input;
  io::ppm::load(input, argv[1]);

  util::couple<I,I>
    bg_fg = scribo::preprocessing::split_bg_fg(input,
					       atoi(argv[2]),
					       atoi(argv[3]));

  io::ppm::save(bg_fg.first(), mln::debug::filename("out_bg.ppm"));
  io::ppm::save(bg_fg.second(), mln::debug::filename("out_fg.ppm"));


  {
    // 1/2
    std::cout << "1/2" << std::endl;
    I input_sub2x = mln::subsampling::subsampling(input, none, 2);

    util::couple<I,I>
      bg_fg = scribo::preprocessing::split_bg_fg(input_sub2x,
						 atoi(argv[4]),
						 atoi(argv[5]));


    io::ppm::save(bg_fg.first(), mln::debug::filename("out_bg_sub2x.ppm"));
    io::ppm::save(bg_fg.second(), mln::debug::filename("out_fg_sub2x.ppm"));
  }

  {
    // 1/4
    std::cout << "1/4" << std::endl;
    I input_sub4x = mln::subsampling::subsampling(input, none, 4);

    util::couple<I,I>
      bg_fg = scribo::preprocessing::split_bg_fg(input_sub4x,
						 atoi(argv[6]),
						 atoi(argv[7]));


    io::ppm::save(bg_fg.first(), mln::debug::filename("out_bg_sub4x.ppm"));
    io::ppm::save(bg_fg.second(), mln::debug::filename("out_fg_sub4x.ppm"));
  }

  mln::trace::exiting("main");
}
