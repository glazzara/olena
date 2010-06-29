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

#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>

#include <mln/math/min.hh>

#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>
#include <mln/value/int_u16.hh>

#include <mln/draw/box.hh>
#include <mln/draw/line.hh>

#include <mln/extension/adjust.hh>

#include <mln/accu/stat/median_h.hh>

#include <scribo/draw/bounding_boxes.hh>

#include <scribo/primitive/extract/objects.hh>

#include <scribo/primitive/link/merge_double_link.hh>
#include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
#include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>

#include <scribo/primitive/group/apply.hh>
#include <scribo/primitive/group/from_double_link.hh>
#include <scribo/primitive/group/from_single_link.hh>

#include <scribo/filter/object_links_bbox_h_ratio.hh>

#include <scribo/filter/objects_small.hh>
#include <scribo/filter/objects_thin.hh>
#include <scribo/filter/objects_thick.hh>

#include <scribo/filter/object_groups_small.hh>

#include <scribo/debug/decision_image.hh>
#include <scribo/debug/save_bboxes_image.hh>
#include <scribo/debug/save_bboxes_image.hh>
#include <scribo/debug/save_linked_bboxes_image.hh>

#include <scribo/debug/usage.hh>

#include <scribo/make/debug_filename.hh>


const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'False' for objects, 'True'\
for the background." },
  { "debug_dir", "Output directory for debug image" },
  {0, 0}
};


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 3 && argc != 4)
    return scribo::debug::usage(argv,
				"Find text lines using left/right validation and display x-height in a binarized article.",
				"input.pbm out.ppm <debug_dir>",
				args_desc,
				"A color image. The following colors are used : dark blue for object bboxes, orange for single object bboxes, purple for group bboxes and light blue for x-height.");

  if (argc == 4)
    scribo::make::internal::debug_filename_prefix = argv[3];

  trace::entering("main");


  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  typedef image2d<value::label_16> L;

  /// Finding objects.
  std::cout << "Finding objects..." << std::endl;
  value::label_16 nobjects;
  object_image(L)
    objects = scribo::primitive::extract::objects(input,
						  c8(),
						  nobjects);


  /// First filtering.
  std::cout << "Filtering objects..." << std::endl;
  object_image(L) filtered_objects
    = scribo::filter::objects_small(objects, 6);

  /// Linking potential objects
  std::cout << "Linking objects..." << std::endl;
  object_links<L> left_link
    = primitive::link::with_single_left_link_dmax_ratio(filtered_objects);
  object_links<L> right_link
    = primitive::link::with_single_right_link_dmax_ratio(filtered_objects);


  // Validating left and right links.
  object_links<L>
    merged_links = primitive::link::merge_double_link(filtered_objects,
						      left_link,
						      right_link);

#ifndef NOUT
  if (argc == 4)
  {
    image2d<value::rgb8> output = data::convert(value::rgb8(), input);
    scribo::draw::bounding_box_links(output,
				     filtered_objects.bboxes(),
				     merged_links,
				     literal::green);

    util::array<bool> drawn(objects.nlabels(), 0);
    for_all_components(i, objects.bboxes())
      if (merged_links[i] == i && ! drawn(i))
      {
	mln::draw::box(output, objects.bbox(i), literal::orange);
	drawn[i] = true;
      }
      else
      {
	mln::draw::box(output, objects.bbox(i), literal::blue);
	mln::draw::box(output, objects.bbox(merged_links[i]), literal::blue);
	drawn[i] = true;
	drawn[merged_links[i]] = true;
      }

    io::ppm::save(output, scribo::make::debug_filename("links.ppm"));
  }
#endif


  // Remove links if bboxes have too different sizes.
  object_links<L> hratio_filtered_links
    = filter::object_links_bbox_h_ratio(filtered_objects,
					merged_links,
					2.0f);




#ifndef NOUT
  if (argc == 4)
  {
    image2d<value::rgb8>
      hratio_decision_image = scribo::debug::decision_image(input,
							    merged_links,
							    hratio_filtered_links);
    io::ppm::save(hratio_decision_image,
		  scribo::make::debug_filename("hratio_links_decision_image.ppm"));
  }
#endif



  // FIXME: from_single_link should return a packed object_groups?
  //
  //######
  object_groups<L>
    groups = primitive::group::from_single_link(filtered_objects,
						hratio_filtered_links);
  value::label_16 n_groups;
  mln::fun::i2v::array<value::label_16>
    groups_packed = mln::make::relabelfun(groups,
					  filtered_objects.nlabels(),
					  n_groups);
  //######


  std::cout << "Grouping objects..." << std::endl;
  object_image(L)
    grouped_objects = primitive::group::apply(filtered_objects, groups);



  // Compute min_row/max_row median
  std::cout << "Preparing output" << std::endl;
  {
    image2d<value::rgb8> output = data::convert(value::rgb8(), input);

//     scribo::draw::bounding_box_links(output,
// 				     filtered_objects.bboxes(),
// 				     hratio_filtered_links,
// 				     literal::green);

    typedef mln::value::int_u<10> median_t;
    typedef mln::accu::stat::median_h<median_t> accu_t;
    util::array<accu_t>
      med_min_row(static_cast<unsigned>(grouped_objects.nlabels()) + 1),
      med_max_row(static_cast<unsigned>(grouped_objects.nlabels()) + 1);

//   std::cout << "Find median min/max rows" << std::endl;
    for_all_components(i, filtered_objects.bboxes())
    {
      if (groups_packed(i) != 0)
      {
	med_min_row(groups_packed(i)).take(filtered_objects.bbox(i).pmin().row() - grouped_objects.bbox(groups_packed(i)).pmin().row());

	med_max_row(groups_packed(i)).take(grouped_objects.bbox(groups_packed(i)).pmax().row() - filtered_objects.bbox(i).pmax().row());
      }
    }

    // Output
    std::cout << "Drawing bboxes" << std::endl;
    util::array<bool> drawn(static_cast<unsigned>(filtered_objects.nlabels()) + 1, 0);
    util::array<bool> single(static_cast<unsigned>(grouped_objects.nlabels()) + 1);
    for_all_components(i, filtered_objects.bboxes())
      if (hratio_filtered_links[i] == i && ! drawn(i))
      {
	mln::draw::box(output, filtered_objects.bbox(i), literal::orange);
	drawn[i] = true;
	single(groups_packed(i)) = true;
      }
      else
      {
	mln::draw::box(output, filtered_objects.bbox(i), literal::blue);
	mln::draw::box(output, filtered_objects.bbox(hratio_filtered_links[i]),
		       literal::blue);
	drawn[i] = true;
	drawn[hratio_filtered_links[i]] = true;
	single(groups_packed(i)) = false;
      }

//   std::cout << "Drawing median lines" << std::endl;
    for_all_components(i, grouped_objects.bboxes())
    {
      if (! single(i))
      {
	point2d
	  b_top(grouped_objects.bbox(i).pmin().row() + med_min_row(i).to_result(), grouped_objects.bbox(i).pmin().col()),
	  e_top(grouped_objects.bbox(i).pmin().row() + med_min_row(i).to_result(), grouped_objects.bbox(i).pmax().col()),
	  b_bot(grouped_objects.bbox(i).pmax().row() - med_max_row(i).to_result(), grouped_objects.bbox(i).pmin().col()),
	  e_bot(grouped_objects.bbox(i).pmax().row() - med_max_row(i).to_result(), grouped_objects.bbox(i).pmax().col());

	mln::draw::box(output, grouped_objects.bbox(i), literal::purple);
	mln::draw::line(output, b_top, e_top, literal::cyan);
	mln::draw::line(output, b_bot, e_bot, literal::cyan);
      }
    }

    io::ppm::save(output, argv[2]);
  }

  trace::exiting("main");
}
