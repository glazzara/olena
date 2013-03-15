// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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
#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/data/convert.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>
#include <mln/draw/line.hh>
#include <mln/util/timer.hh>

#include <scribo/util/component_precise_outline.hh>
#include <scribo/debug/usage.hh>

#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>


const char *args_desc[][2] =
{
  { "input.pbm", "A binary image" },
  { "output.ppm", "Output image." },
  {0, 0}
};


int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;

  if (argc != 3)
    return scribo::debug::usage(argv,
				"Extract component precise outlines",
				"input.pbm output.pbm",
				args_desc);

  mln_trace("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  mln::util::timer t;
  t.start();

  typedef mln::p_array<mln::point2d> par_t;
  par_t par = scribo::util::component_precise_outline(input);

  t.stop();
  /* FIXME: Help the compiler to ``unproxy' the float stored in the
     timer.  There is a problem with an overload resolution of
     `mln::unproxy_rec' here.  */
  std::cout << t.read() << std::endl;

  image2d<value::rgb8> input_rgb = data::convert(value::rgb8(), input);

  point2d last_point = par[0];
  mln_piter_(par_t) p(par);
  for_all(p)
  {
    mln::draw::line(input_rgb, last_point, p, literal::red);
    last_point = p;
  }

  io::ppm::save(input_rgb, argv[2]);

}
