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

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/all.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/magick/load.hh>
#include <mln/value/int_u.hh>
#include <mln/literal/colors.hh>
#include <mln/draw/box.hh>

#include <scribo/core/component_set.hh>
#include <scribo/primitive/extract/components.hh>

int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;

  if (argc != 4)
  {
    std::cerr << "Usage : " << argv[0] << " input.* input.pbm out.ppm" << std::endl;
    return 1;
  }

  Magick::InitializeMagick(0);

  typedef mln::value::int_u<30> V;
  typedef image2d<V> L;

  image2d<value::rgb8> input;
  io::magick::load(input, argv[1]);

  image2d<bool> input_bin;
  io::pbm::load(input_bin, argv[2]);

  V ncomponents;
  component_set<L>
    components = scribo::primitive::extract::components(input, input_bin, c8(),
							ncomponents);

  image2d<value::rgb8> output;
  initialize(output, input);
  data::fill(output, literal::white);

  const L& lbl = components.labeled_image();
  for_all_comps(c, components)
    if (components(c).is_valid())
    {
      data::fill(((output | components(c).bbox()).rw()
		  | (pw::value(lbl) == pw::cst(c))).rw(),
		 components(c).features().color);
    }

  io::ppm::save(output, argv[3]);
}
