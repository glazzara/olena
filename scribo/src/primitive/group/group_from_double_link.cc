// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
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
#include <mln/util/array.hh>
#include <mln/literal/colors.hh>
#include <mln/io/pbm/load.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/label_16.hh>

#include <scribo/core/object_links.hh>

#include <scribo/primitive/extract/components.hh>
#include <scribo/primitive/group/apply.hh>
#include <scribo/primitive/link/with_single_left_link.hh>
#include <scribo/primitive/link/with_single_right_link.hh>
#include <scribo/debug/linked_bboxes_image.hh>
#include <scribo/primitive/group/from_double_link.hh>
#include <scribo/filter/objects_small.hh>

#include <scribo/debug/bboxes_image.hh>
#include <scribo/make/debug_filename.hh>


#include <scribo/debug/usage.hh>

const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'True' for objects, 'False' for the "
    "background." },
  { "hlmax", "Maximum distance between two grouped objects while browsing "
    "on the left." },
  { "hrmax", "Maximum distance between two grouped objects while browsing "
    "on the right." },
  { "prefix", "Output names prefix" },
  {0, 0}
};

int main(int argc, char *argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 5)
    return scribo::debug::usage(argv,
				"Group potential text objects using a double"
				"validation link.",
				"input.pbm hlmax hrmax prefix",
				args_desc);


  scribo::make::internal::debug_filename_prefix = argv[4];

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  value::label_16 nbboxes;
  typedef image2d<value::label_16> L;
  component_set<L>
    text = primitive::extract::components(input, c8(), nbboxes);

  text = filter::components_small(text, 4);

  object_links<L> left_link
    = primitive::link::with_single_left_link(text, atoi(argv[2]));
  object_links<L> right_link
    = primitive::link::with_single_right_link(text, atoi(argv[3]));

  std::cout << "BEFORE - nbboxes = " << nbboxes << std::endl;

  io::ppm::save(scribo::debug::linked_bboxes_image(input,
						   left_link, right_link,
						   literal::red,
						   literal::cyan,
						   literal::yellow,
						   literal::green,
						   anchor::MassCenter),
		scribo::make::debug_filename("links.ppm"));

  // With validation.
  object_groups<L> groups
    = primitive::group::from_double_link(left_link, right_link);

  component_set<L> grouped_text = primitive::group::apply(groups);

  io::ppm::save(mln::labeling::colorize(value::rgb8(),
					grouped_text.labeled_image(),
					grouped_text.nelements()),
		scribo::make::debug_filename("label_color.ppm"));

  std::cout << "AFTER double grouping - nbboxes = "
	    << grouped_text.nelements() << std::endl;

  io::ppm::save(scribo::debug::bboxes_image(input, grouped_text,
					    literal::red),
		scribo::make::debug_filename("bboxes.ppm"));

}
