// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
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

#include <mln/io/pgm/all.hh>

#include <scribo/binarization/niblack_threshold.hh>
#include <scribo/debug/usage.hh>

const char *args_desc[][2] =
{
  { "input.pgm", "A graylevel image." },
  { "output.pgm", "A threshold image." },
  { "w", "Window size (default 101)." },
  { "k", "Niblack's formulae parameter (default -0.2)." },
  {0, 0}
};


int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 5 && argc != 4 && argc != 3)
    return scribo::debug::usage(argv,
				"Compute a threshold image based on Niblack's algorithm.",
				"input.pgm output.pgm <w> <k>",
				args_desc);

  trace::entering("main");

  unsigned w;
  if (argc >= 4)
    w = atoi(argv[3]);
  else
    w = 101;

  double k;
  if (argc >= 5)
    k = atof(argv[4]);
  else
    k = -0.2;

  std::cout << "Using w=" << w << " and k=" << k << std::endl;

  image2d<value::int_u8> input;
  io::pgm::load(input, argv[1]);

  image2d<value::int_u8> out = scribo::binarization::niblack_threshold(input, w, k);


  io::pgm::save(out, argv[2]);


  trace::exiting("main");
}
