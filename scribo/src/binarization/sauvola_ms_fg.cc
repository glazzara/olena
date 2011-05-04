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
#include <mln/io/magick/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/data/transform.hh>
#include <mln/fun/v2v/rgb_to_luma.hh>

#include <scribo/binarization/sauvola_ms.hh>
#include <scribo/preprocessing/split_bg_fg.hh>
#include <scribo/debug/usage.hh>

bool check_args(int argc, char * argv[])
{
  if (argc != 6)
    return false;

  int s = atoi(argv[4]);

  if (s < 2 || s > 3)
  {
    std::cout << "s must be set to 2 or 3."
	      << std::endl;
    return false;
  }

  return true;
}


const char *args_desc[][2] =
{
  { "input.*", "An image." },
  { "lambda", "Lambda used to split bg/fg." },
  { "w", "Window size at scale 1. (Common value: 101)" },
  { "s", "First subsampling ratio (Common value: 3)." },
  { "output.pbm", "A binary image." },
  {0, 0}
};




int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;

  if (!check_args(argc, argv))
    return scribo::debug::usage(argv,
				"Multi-Scale Binarization based on Sauvola's algorithm. Performs a binarization on each component of the color image and merges the results.",
				"input.* output.pbm w s",
				args_desc);

  trace::entering("main");

  unsigned lambda = atoi(argv[2]);

  // Window size
  unsigned w_1 = atoi(argv[3]);  // Scale 1

  // First subsampling scale.
  unsigned s = atoi(argv[4]);


  std::cout << "Using w_1=" << w_1 << " - s=" << s << std::endl;

  Magick::InitializeMagick(0);

  // Load
  image2d<value::rgb8> input_1;
  io::magick::load(input_1, argv[1]);

  // Split foreground/background
  image2d<value::rgb8>
    fg = scribo::preprocessing::split_bg_fg(input_1, lambda, 32).first();

  // Convert to Gray level image.
  image2d<value::int_u8>
    fg_gl = data::transform(fg, mln::fun::v2v::rgb_to_luma<value::int_u8>());

  // Binarize
  image2d<bool>
    output = scribo::binarization::sauvola_ms(fg_gl, w_1, s, SCRIBO_DEFAULT_SAUVOLA_K);

  io::pbm::save(output, argv[5]);
}


