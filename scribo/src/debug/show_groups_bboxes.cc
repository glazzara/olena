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

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pbm/all.hh>
#include <mln/value/int_u.hh>
#include <mln/literal/colors.hh>
#include <mln/draw/box.hh>

#include <scribo/core/component_set.hh>
#include <scribo/core/line_set.hh>
#include <scribo/primitive/extract/components.hh>
#include <scribo/primitive/group/from_single_link.hh>
#include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
#include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>
#include <scribo/primitive/link/merge_double_link.hh>
#include <scribo/primitive/link/internal/dmax_width_and_height.hh>

int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;
  using namespace scribo::primitive;

  if (argc != 3)
  {
    std::cerr << "Usage : " << argv[0] << " input.* out.pbm" << std::endl;
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


  object_links<L>
    left_link = link::with_single_left_link_dmax_ratio(
      components,
//      primitive::link::internal::dmax_width_and_height(1),
      link::internal::dmax_default(1),
      anchor::MassCenter);

  object_links<L>
    right_link = primitive::link::with_single_right_link_dmax_ratio(
      components,
//      primitive::link::internal::dmax_width_and_height(1),
      primitive::link::internal::dmax_default(1),
      anchor::MassCenter);

  // Validating left and right links.
  object_links<L>
    merged_links = primitive::link::merge_double_link(left_link,
						      right_link);


  object_groups<L>
    groups = group::from_single_link(merged_links);

  line_set<L> lines(groups);

  image2d<bool> output;
  initialize(output, input);
  data::fill(output, false);

  for_all_lines(l, lines)
    if (lines(l).is_valid())
      mln::draw::box(output, lines(l).bbox(), true);

  io::pbm::save(output, argv[2]);
}
