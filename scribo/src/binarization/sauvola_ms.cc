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
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/save.hh>

#include <scribo/binarization/sauvola_ms.hh>
#include <scribo/debug/usage.hh>

bool check_args(int argc, char * argv[])
{
  if (argc < 5 || argc > 6)
    return false;

  int s = atoi(argv[3]);

  if (s < 1 || s > 3)
  {
    std::cout << "s must be set to 2 or 3."
	      << std::endl;
    return false;
  }

  return true;
}


const char *args_desc[][2] =
{
  { "input.pgm", "A graylevel image." },
  { "w", "Window size at scale 1. (Common value: 101)" },
  { "s", "First subsampling ratio (Common value: 3)." },
  { "min_area",    "Minimum object area at scale 1 (Common value: 67)" },
  {0, 0}
};




int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;

  if (!check_args(argc, argv))
    return scribo::debug::usage(argv,
				"Multi-Scale Binarization of a color image based on Sauvola's algorithm.",
				"input.pgm w s area_threshold output.pbm",
				args_desc, "A binary image.");

  trace::entering("main");

  // Window size
  unsigned w_1 = atoi(argv[2]);  // Scale 1

  // First subsampling scale.
  unsigned s = atoi(argv[3]);

  // Lambda value
  unsigned lambda_min_1 = atoi(argv[4]);


  image2d<value::int_u8> input_1;
  io::pgm::load(input_1, argv[1]);

//   {
//     unsigned max_dim = math::min(input_1.ncols() / s,
// 				 input_1.nrows() / s);
//     if ((w_1 / s * 4) > max_dim)
//     {
//       std::cout << "------------------" << std::endl;
//       std::cout << "The window is too large! Image size is only "
// 		<< input_1.nrows() << "x" << input_1.ncols()
// 		<< std::endl
// 		<< "Window size must not exceed " << max_dim * s / 4
// 		<< std::endl;
//       return 1;
//     }
//   }

  image2d<bool>
    output = scribo::binarization::sauvola_ms(input_1, w_1, s, lambda_min_1);

  io::pbm::save(output, argv[5]);
}


