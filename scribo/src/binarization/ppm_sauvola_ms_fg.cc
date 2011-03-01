// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
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

/// \file

#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/data/transform.hh>
#include <mln/fun/v2v/rgb_to_luma.hh>

#include <scribo/binarization/sauvola_ms.hh>
#include <scribo/preprocessing/split_bg_fg.hh>
#include <scribo/debug/usage.hh>

bool check_args(int argc, char * argv[])
{
  if (argc < 3 || argc > 7)
    return false;

  if (argc >= 6)
  {
    int s = atoi(argv[5]);

    if (s < 2 || s > 3)
    {
      std::cout << "s must be set to 2 or 3."
		<< std::endl;
      return false;
    }
  }

  return true;
}


const char *args_desc[][2] =
{
  { "input.ppm", "A color image." },
  { "output.pbm", "A binary image." },
  { "lambda", "Lambda used to split bg/fg." },
  { "w", "Window size at scale 1. (default: 101)" },
  { "s", "First subsampling ratio. (default: 3)" },
  { "k", "Sauvola's formula parameter. (default: 0.34)" },
  {0, 0}
};




int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;

  if (!check_args(argc, argv))
    return scribo::debug::usage(argv,
				"Multi-Scale Binarization of a color image based on Sauvola's algorithm. Performs a binarization on each component of the color image and merges the results.",
				"input.ppm output.pbm lambda <w> <s> <k>",
				args_desc);

  trace::entering("main");

  unsigned lambda = atoi(argv[3]);

  // Window size
  unsigned w_1;
  if (argc >= 5)
    w_1 = atoi(argv[4]);  // Scale 1
  else
    w_1 = 101u;

  // First subsampling scale.
  unsigned s;
  if (argc >= 6)
    s = atoi(argv[5]);
  else
    s = 3u;

  double k;
  if (argc >= 7)
    k = atof(argv[6]);
  else
    k = 0.34f;

  std::cout << "Using w_1=" << w_1 << " - s=" << s
	    << " - k=" << k << std::endl;

  // Load
  image2d<value::rgb8> input_1;
  io::ppm::load(input_1, argv[1]);


  // Split foreground/background
  image2d<value::rgb8>
    fg = scribo::preprocessing::split_bg_fg(input_1, lambda, 32).second();


  // Convert to Gray level image.
  image2d<value::int_u8>
    fg_gl = data::transform(fg, mln::fun::v2v::rgb_to_luma<value::int_u8>());

  // Binarize
  image2d<bool>
    output = scribo::binarization::sauvola_ms(fg_gl, w_1, s, k);

  io::pbm::save(output, argv[2]);
}


