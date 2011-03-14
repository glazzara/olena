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
#include <mln/io/pbm/all.hh>
#include <mln/value/int_u.hh>
#include <mln/literal/colors.hh>
#include <mln/draw/box.hh>

#include <scribo/core/component_set.hh>
#include <scribo/primitive/extract/components.hh>

int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;

  if (argc != 3)
  {
    std::cerr << "Usage : " << argv[0] << " input.pbm out.pbm" << std::endl;
    return 1;
  }

  typedef mln::value::int_u<30> V;
  typedef image2d<V> L;

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  V ncomponents;
  component_set<L>
    components = scribo::primitive::extract::components(input, c8(),
							ncomponents);

  image2d<bool> output;
  initialize(output, input);
  data::fill(output, false);

  for_all_comps(c, components)
    if (components(c).is_valid())
      mln::draw::box(output, components(c).bbox(), true);

  io::pbm::save(output, argv[2]);
}
