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
#include <mln/value/rgb8.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pbm/save.hh>

#include <scribo/binarization/sauvola_ms_split.hh>
#include <scribo/debug/usage.hh>

bool check_args(int argc, char * argv[])
{
  if (argc < 3 || argc > 7)
    return false;

  if (argc >= 5)
  {
    int s = atoi(argv[4]);

    if (s < 1 || s > 3)
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
  { "w", "Window size at scale 1. (default: 101)" },
  { "s", "First subsampling ratio (default: 3)." },
  { "min_ntrue",   "The number of components in which a site must be set to 'True' in order to be set to 'True' in the output (Possible values: 1, 2, 3).  (default: 2)" },
  { "K", "Sauvola's formula parameter (default: 0.34)." },
  {0, 0}
};




int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;

  if (!check_args(argc, argv))
    return scribo::debug::usage(argv,
				"Multi-Scale Binarization of a color image based on Sauvola's algorithm. Performs a binarization on each component of the color image and merges the results.",
				"input.ppm output.pbm <w> <s> <min_ntrue> <K>",
				args_desc);

  trace::entering("main");

  // Window size
  unsigned w_1;
  if (argc >= 4)
    w_1 = atoi(argv[3]);  // Scale 1
  else
    w_1 = 101u;

  // First subsampling scale.
  unsigned s;
  if (argc >= 5)
    s = atoi(argv[4]);
  else
    s = 3u;

  // min_ntrue
  unsigned min_ntrue;
  if (argc >= 6)
    min_ntrue = atoi(argv[5]);
  else
    min_ntrue = 2;

  // Lambda value
  unsigned lambda_min_1 = 67; // FIXME: should be adapted to the
			      // window size.

  double k;
  if (argc >= 7)
    k = atof(argv[6]);
  else
    k = 0.34f;



  image2d<value::rgb8> input_1;
  io::ppm::load(input_1, argv[1]);

  std::cout << "Using w=" << w_1 << " - s=" << s << " - min_ntrue=" << min_ntrue << " - k=" << k << std::endl;

  image2d<bool>
    output = scribo::binarization::sauvola_ms_split(input_1, w_1, s, lambda_min_1, min_ntrue, k);

  io::pbm::save(output, argv[2]);
}


