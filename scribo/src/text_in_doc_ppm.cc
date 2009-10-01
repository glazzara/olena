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

#include <mln/logical/not.hh>

#include <mln/labeling/colorize.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>

#include <mln/math/min.hh>

#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>

#include <mln/draw/box.hh>

#include <mln/extension/adjust.hh>

#include <scribo/binarization/sauvola.hh>

#include <scribo/table/erase.hh>

#include <scribo/draw/bounding_boxes.hh>

#include <scribo/primitive/extract/objects.hh>
#include <scribo/primitive/extract/lines_h_discontinued.hh>
#include <scribo/primitive/extract/lines_v_discontinued.hh>

#include <scribo/primitive/link/merge_double_link.hh>
#include <scribo/primitive/link/with_single_left_link.hh>
#include <scribo/primitive/link/with_single_right_link.hh>

#include <scribo/primitive/group/apply.hh>
#include <scribo/primitive/group/from_double_link.hh>
#include <scribo/primitive/group/from_single_link.hh>

#include <scribo/filter/object_links_bottom_aligned.hh>
#include <scribo/filter/object_links_bbox_h_ratio.hh>
#include <scribo/filter/object_links_bbox_overlap.hh>

#include <scribo/filter/objects_small.hh>
#include <scribo/filter/objects_thin.hh>
#include <scribo/filter/objects_thick.hh>

#include <scribo/filter/object_groups_small.hh>

#include <scribo/text/recognition.hh>

#include <scribo/debug/decision_image.hh>
#include <scribo/debug/save_bboxes_image.hh>
#include <scribo/debug/save_bboxes_image.hh>
#include <scribo/debug/save_linked_bboxes_image.hh>

#include <scribo/debug/usage.hh>

#include <scribo/preprocessing/split_bg_fg.hh>

#include <scribo/make/debug_filename.hh>


const char *args_desc[][2] =
{
  { "input.ppm", "A color image." },
  { "out.txt", "A text file with all the recognized text" },
  { "output_dir", "Output directory for debug image" },
  {0, 0}
};


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 3 && argc != 4)
    return scribo::debug::usage(argv,
				"Find text in a color document.",
				"input.ppm out.txt <output_dir>",
				args_desc,
				"Debug outputs. The recognized text is printed in the standard output.");

  if (argc == 4)
    scribo::make::internal::debug_filename_prefix = argv[3];

  trace::entering("main");


  image2d<value::rgb8> input_rgb;
  io::ppm::load(input_rgb, argv[1]);


  unsigned lambda;
  if (argc == 5)
    lambda = atoi(argv[4]);
  else
    lambda = 1.2 * (input_rgb.nrows() + input_rgb.ncols());

  // Extract foreground
  std::cout << "Extracting foreground..." << std::endl;
  image2d<value::rgb8>
    fg = preprocessing::split_bg_fg(input_rgb,
				    lambda,
				    32).second();

  // Binarize foreground to use it in the processing chain.
  std::cout << "Binarizing foreground..." << std::endl;
  image2d<bool> input = scribo::binarization::sauvola(fg);


  typedef image2d<value::label_16> L;

  /// Extracting vertical and horizontal lines.
  std::cout << "Extracting lines..." << std::endl;
  value::label_16 nhlines, nvlines;
  object_image(L)
    lbl_v = primitive::extract::lines_v_discontinued(input, c8(),
						     nvlines, 51, 8);
  object_image(L)
    lbl_h = primitive::extract::lines_h_discontinued(input, c8(),
						     nhlines, 51, 6);

  /// Reconstruct and erase tables.
  std::cout << "Reconstructing and removing tables..." << std::endl;
  image2d<bool> input_notables
    = scribo::table::erase(input, lbl_h, lbl_v);


  io::pbm::save(input_notables, "table_erased.ppm");

  /// Finding objects.
  std::cout << "Finding objects..." << std::endl;
  value::label_16 nobjects;
  object_image(L)
    objects = scribo::primitive::extract::objects(input_notables,
						  c8(),
						  nobjects);


  /// First filtering.
  std::cout << "Filtering objects..." << std::endl;
  object_image(L) filtered_objects
    = scribo::filter::objects_small(objects, 6);

  filtered_objects
    = scribo::filter::objects_thin(filtered_objects, 1);

//   filtered_objects
//     = scribo::filter::objects_thick(filtered_objects,
// 	  math::min(input.ncols(), input.nrows()) / 5);



  /// Linking potential objects
  std::cout << "Linking objects..." << std::endl;
  object_links<L> left_link
    = primitive::link::with_single_left_link(filtered_objects, 30);
  object_links<L> right_link
    = primitive::link::with_single_right_link(filtered_objects, 30);


#ifndef NOUT
  if (argc == 4)
  {
    std::cerr << "BEFORE - nobjects = " << nobjects << std::endl;
    scribo::debug::save_linked_bboxes_image(input,
					    filtered_objects,
					    left_link, right_link,
					    literal::red, literal::cyan,
					    literal::yellow,
					    literal::green,
					    scribo::make::debug_filename("links.ppm"));
  }
#endif


  // Validating left and right links.
  object_links<L>
    merged_links = primitive::link::merge_double_link(filtered_objects,
						      left_link,
						      right_link);




  // Remove links if bboxes are not aligned.
  std::cout << "Filtering object links..." << std::endl;
  object_links<L> bottom_filtered_links
    = filter::object_links_bottom_aligned(filtered_objects,
					  merged_links,
					  5);



#ifndef NOUT
  if (argc == 4)
  {
    image2d<value::rgb8>
      bottom_decision_image = scribo::debug::decision_image(input,
							    merged_links,
							    bottom_filtered_links);

    io::ppm::save(bottom_decision_image,
		  scribo::make::debug_filename("bottom_links_decision_image.ppm"));
  }
#endif





//   // Remove links if bboxes have too different sizes.
//   object_links<L> hratio_filtered_links
//     = filter::object_links_bbox_h_ratio(filtered_objects,
// 					bottom_filtered_links,
// 					0.6f);




// #ifndef NOUT
//   if (argc == 4)
//   {
//     image2d<value::rgb8>
//       hratio_decision_image = scribo::debug::decision_image(input,
// 							    bottom_filtered_links,
// 							    hratio_filtered_links);
//     io::ppm::save(hratio_decision_image,
// 		  scribo::make::debug_filename("hratio_links_decision_image.ppm"));
//   }
// #endif




  //Remove links if bboxes overlap too much.
  object_links<L> overlap_filtered_links
    = filter::object_links_bbox_overlap(filtered_objects,
					bottom_filtered_links,
					0.80f);




#ifndef NOUT
  if (argc == 4)
  {
    image2d<value::rgb8> overlap_decision_image
      = scribo::debug::decision_image(input,
				      bottom_filtered_links,
				      overlap_filtered_links);
    io::ppm::save(overlap_decision_image,
		  scribo::make::debug_filename("overlap_links_decision_image.ppm"));
  }
#endif




  object_groups<L>
    groups = primitive::group::from_single_link(filtered_objects,
						overlap_filtered_links);


//   // Remove objects part of groups with strictly less than 3 objects.
//   object_groups<L> filtered_groups = filter::object_groups_small(groups, 3);


// #ifndef NOUT
//   image2d<value::rgb8>
//     decision_image = scribo::debug::decision_image(input,
// 						   groups, filtered_groups);
// #endif

  std::cout << "Grouping objects..." << std::endl;
  object_image(L)
    grouped_objects = primitive::group::apply(filtered_objects, groups);


#ifndef NOUT
  if (argc == 4)
    io::ppm::save(mln::labeling::colorize(value::rgb8(),
					  grouped_objects,
					  grouped_objects.nlabels()),
		  scribo::make::debug_filename("out.ppm"));
#endif

  std::cout << "Text recognition..." << std::endl;
  text::recognition(grouped_objects, "fra", argv[2]);

  trace::exiting("main");
}
