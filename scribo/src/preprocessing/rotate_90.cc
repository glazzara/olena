// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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
#include <mln/io/magick/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/util/timer.hh>

#include <scribo/debug/usage.hh>
#include <scribo/preprocessing/rotate_90.hh>


const char *args_desc[][2] =
{
  { "input.*", "An image." },
  { "output.ppm", "A rotated image." },
  { "positive", "if set to 1, performs a +90 rotation; -90 otherwise. "
    "(default: 0)" },
  {0, 0}
};



int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 3 && argc != 4)
    return scribo::debug::usage(argv,
				"Fast +90/-90 rotation",
				"input.* output.ppm <positive>",
				args_desc);

  Magick::InitializeMagick(0);

  typedef image2d<value::rgb8> I;
  I ima;
  io::magick::load(ima, argv[1]);

  bool positive = false;

  if (argc >= 4)
    positive = (atoi(argv[3]) != 0);


  mln::util::timer t;
  t.start();
  image2d<value::rgb8> out = scribo::preprocessing::rotate_90(ima, positive);
  t.stop();

  std::cout << t << std::endl;

  io::ppm::save(out, argv[2]);
}
