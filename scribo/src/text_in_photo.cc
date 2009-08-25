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

#include <libgen.h>
#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/labeling/colorize.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>

#include <mln/math/min.hh>

#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>

#include <mln/draw/box.hh>

#include <scribo/primitive/extract/objects.hh>
#include <scribo/primitive/group/apply.hh>
#include <scribo/primitive/link/with_single_left_link.hh>
#include <scribo/primitive/link/with_single_right_link.hh>
#include <scribo/primitive/group/from_double_link.hh>
#include <scribo/primitive/group/from_single_link.hh>
#include <scribo/filter/objects_small.hh>
#include <scribo/filter/objects_thin.hh>
#include <scribo/filter/objects_thick.hh>
#include <scribo/filter/object_groups_small.hh>

#include <scribo/make/debug_filename.hh>
#include <scribo/debug/save_bboxes_image.hh>
#include <scribo/debug/save_linked_bboxes_image.hh>

#include <scribo/debug/usage.hh>

const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'True' for objects, 'False'\
for the background." },
  {0, 0}
};


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  scribo::make::internal::debug_filename_prefix = argv[2];

  if (argc != 3)
    return scribo::debug::usage(argv,
				"Find text in a binarized photo.",
				"input.pbm output_dir",
				args_desc,
				"A color image where the text is \
highlighted.");

  trace::entering("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  typedef image2d<value::label_16> L;

  /// Finding objects.
  value::label_16 nobjects;
  object_image(L)
    objects = scribo::primitive::extract::objects(input, c8(), nobjects);


  /// First filtering.
  object_image(L) filtered_objects
    = scribo::filter::objects_small(objects, 6);

  filtered_objects
    = scribo::filter::objects_thin(filtered_objects, 1);

  filtered_objects
    = scribo::filter::objects_thick(filtered_objects,
	  math::min(input.ncols(), input.nrows()) / 5);

  /// Grouping potential objects
  object_links<L> left_link
    = primitive::link::with_single_left_link(filtered_objects, 30);
  object_links<L> right_link
    = primitive::link::with_single_right_link(filtered_objects, 30);


#ifndef NOUT
  std::cout << "BEFORE - nobjects = " << nobjects << std::endl;
  scribo::debug::save_linked_bboxes_image(input,
 					  filtered_objects,
 					  left_link, right_link,
 					  literal::red, literal::cyan,
 					  literal::yellow,
 					  literal::green,
 					  scribo::make::debug_filename("links.ppm"));
#endif

    // Trying to group objects
  object_groups<L>
    groups = primitive::group::from_double_link(filtered_objects,
						left_link, right_link);

    // Remove objects part of groups with less than 3 objects.
  mln::util::array<bool>
    to_be_kept = filter::object_groups_small(groups, 3);


  // FOR DEBUGGING PURPOSE.
  image2d<value::rgb8> decision_image = data::convert(value::rgb8(), input);


  for (unsigned i = 1; i < to_be_kept.size(); ++i)
  {
    if (!to_be_kept(i))
      mln::draw::box(decision_image, filtered_objects.bbox(i), literal::red);
    else
      mln::draw::box(decision_image, filtered_objects.bbox(i), literal::green);
  }
  // END OF DEBUG


  filtered_objects.relabel(to_be_kept);


  /// Objects have been removed we need to update object links again.
  /// This time a single link is enough since non-wanted objects have
  /// been removed.
  left_link
    = primitive::link::with_single_left_link(filtered_objects, 30);



  /// Grouping objects again.
  groups = primitive::group::from_single_link(filtered_objects, left_link);

  object_image(L)
    grouped_objects = primitive::group::apply(filtered_objects, groups);

#ifndef NOUT
  /// FOR DEBUG PURPOSE.
  for (unsigned i = 1; i <= grouped_objects.nlabels(); ++i)
    mln::draw::box(decision_image, grouped_objects.bbox(i), literal::blue);
  io::ppm::save(decision_image, scribo::make::debug_filename("decision_image.ppm"));


  std::cout << "AFTER - nobjects = " << grouped_objects.nlabels() << std::endl;
#endif

  io::ppm::save(mln::labeling::colorize(value::rgb8(),
					grouped_objects,
					grouped_objects.nlabels()),
//		"/dev/null");
		scribo::make::debug_filename("out.ppm"));


  trace::exiting("main");
}
