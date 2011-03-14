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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/data/convert.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>
#include <mln/literal/colors.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/save.hh>

#include <scribo/core/def/lbl_type.hh>
#include <scribo/debug/logger.hh>
#include <scribo/primitive/extract/components.hh>
#include <scribo/filter/object_links_aligned.hh>
#include <scribo/primitive/link/with_single_right_link_dmax_ratio_aligned.hh>

#include <scribo/debug/usage.hh>



const char *args_desc[][2] =
{
  { "input.pbm", "A binary image" },
  { "dmax_ratio", "Maximum distance lookup (common value 5)" },
  { "min_alpha", "First angle used for close objects. (common value : 3)" },
  { "max_alpha", "Second angle used for further objects. (common value : 5)" },
  {0, 0}
};


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace scribo::primitive;
  using namespace mln;

  if (argc != 6)
    return scribo::debug::usage(argv,
				"Show valid or invalid links according the "
				"horizontal alignment (based on top line).",
				"input.pbm dmax_ratio min_angle max_angle "
				" output.ppm",
				args_desc);

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  // Finding components.
  typedef scribo::def::lbl_type V;
  V nbboxes;
  typedef image2d<V> L;
  component_set<L> components
    = extract::components(input, c8(), nbboxes);

  // Finding right links.
  // object_links<L>
  //   right_links = primitive::link::with_single_right_link_dmax_ratio_aligned(
  //     components, atof(argv[2]), atof(argv[3]), atof(argv[4]));

  scribo::debug::logger().set_level(scribo::debug::All);

  link::internal::single_right_dmax_ratio_aligned_functor<L,link::internal::dmax_default>
    functor(components, link::internal::dmax_default(atof(argv[2])),
	    atof(argv[3]), atof(argv[4]), anchor::StrictTopCenter);

  object_links<L> output = link::compute(functor, anchor::Top);

  scribo::debug::logger().set_level(scribo::debug::None);

  io::ppm::save(functor.debug_, argv[5]);
}
