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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/labeling/colorize.hh>
#include <mln/debug/println.hh>
#include <mln/data/convert.hh>
#include <mln/util/array.hh>
#include <mln/literal/colors.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/label_16.hh>

#include <scribo/core/object_links.hh>
#include <scribo/core/object_image.hh>

#include <scribo/primitive/extract/objects.hh>

#include <scribo/primitive/link/with_single_left_link.hh>
#include <scribo/primitive/link/with_single_right_link.hh>
#include <scribo/primitive/link/merge_double_link.hh>

#include <scribo/draw/bounding_boxes.hh>
#include <scribo/draw/bounding_box_links.hh>

#include <scribo/debug/usage.hh>



const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'True' for objects, 'False'\
for the background." },
  { "hlmax", "Maximum distance between two grouped objects while browsing on the left." },
  { "hrmax", "Maximum distance between two grouped objects while browsing on the right." },
  {0, 0}
};

int main(int argc, char *argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 5)
    return scribo::debug::usage(argv,
				"Display double validated (left and right) links between objects",
				"<input.pbm> <hlmax> <hrmax> <output.ppm>",
				args_desc,
				"A color image. Validated links are drawn in green.");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  // Finding objects.
  value::label_16 nbboxes;
  typedef image2d<value::label_16> L;
  object_image(L) objects = primitive::extract::objects(input, c8(), nbboxes);

  // Left links.
  object_links<L> left_link
    = primitive::link::with_single_left_link(objects, atoi(argv[2]));

  // Right links.
  object_links<L> right_link
    = primitive::link::with_single_right_link(objects, atoi(argv[3]));

  // Validation.
  object_links<L>
    links = primitive::link::merge_double_link(objects, left_link, right_link);


  // Saving result.
  image2d<value::rgb8> output = data::convert(value::rgb8(), input);

//   scribo::draw::bounding_boxes(output, objects, literal::blue);
  scribo::draw::bounding_box_links(output,
				   objects.bboxes(),
				   links,
				   literal::green);

  util::array<bool> drawn(objects.nlabels(), 0);
  for_all_components(i, objects.bboxes())
    if (links[i] == i && ! drawn(i))
    {
      mln::draw::box(output, objects.bbox(i), literal::orange);
      drawn[i] = true;
    }
    else
    {
      mln::draw::box(output, objects.bbox(i), literal::blue);
      mln::draw::box(output, objects.bbox(links[i]), literal::blue);
      drawn[i] = true;
      drawn[links[i]] = true;
    }

  io::ppm::save(output, argv[4]);
}
