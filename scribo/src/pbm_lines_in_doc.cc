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
#include <fstream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/logical/not.hh>

#include <mln/labeling/colorize.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/dump/save.hh>

#include <mln/math/min.hh>

#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>

#include <mln/draw/box.hh>

#include <mln/extension/adjust.hh>

#include <scribo/table/erase.hh>

#include <scribo/draw/bounding_boxes.hh>

#include <scribo/primitive/extract/objects.hh>
#include <scribo/primitive/extract/lines_h_discontinued.hh>
#include <scribo/primitive/extract/lines_v_discontinued.hh>

#include <scribo/primitive/link/merge_double_link.hh>
#include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
#include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>

#include <scribo/primitive/group/apply.hh>
#include <scribo/primitive/group/from_double_link.hh>
#include <scribo/primitive/group/from_single_link.hh>

#include <scribo/filter/object_links_bottom_aligned.hh>
#include <scribo/filter/object_links_bbox_h_ratio.hh>
#include <scribo/filter/object_links_bbox_overlap.hh>

#include <scribo/filter/objects_large.hh>
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
  { "out.ppm", "Show the text found in the input." },
  { "object_label.dump", "Object labeled image." },
  { "line_label.dump", "Text lines labeled image." },
  { "bbox_100p.txt", "Text file containing pmin and pmax of text line bounding boxes." },
  { "bbox_50p.txt", "Text file containing pmin and pmax of text line bounding boxes." },
  { "output_dir", "Output directory for debug image" },
  {0, 0}
};


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 7 && argc != 8)
    return scribo::debug::usage(argv,
				"Find text lines in a document",
				"input.pbm out.ppm object_label.dump line_label.dump bbox_100p.txt bbox_50p.txt [output_dir]",
				args_desc,
				"Debug outputs. The recognized text is printed in the standard output.");

  if (argc == 8)
    scribo::make::internal::debug_filename_prefix = argv[7];

  trace::entering("main");


  bool enable_debug = (argc == 5);

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


  filtered_objects
    = scribo::filter::objects_large(filtered_objects,
				    math::min(input.ncols(), input.nrows())
				    * math::min(input.ncols(), input.nrows()) / 5);



  /// Linking potential objects
  std::cout << "Linking objects..." << std::endl;
  object_links<L> left_link
    = primitive::link::with_single_left_link_dmax_ratio(filtered_objects);
  object_links<L> right_link
    = primitive::link::with_single_right_link_dmax_ratio(filtered_objects);


#ifndef NOUT
  if (enable_debug)
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




  // Remove links if bboxes have too different sizes.
  object_links<L> hratio_filtered_links
    = filter::object_links_bbox_h_ratio(filtered_objects,
					merged_links,
					2.0f);




#ifndef NOUT
  if (enable_debug)
  {
    image2d<value::rgb8>
      hratio_decision_image = scribo::debug::decision_image(input,
							    merged_links,
							    hratio_filtered_links);
    io::ppm::save(hratio_decision_image,
		  scribo::make::debug_filename("hratio_links_decision_image.ppm"));
  }
#endif




  //Remove links if bboxes overlap too much.
  object_links<L> overlap_filtered_links
    = filter::object_links_bbox_overlap(filtered_objects,
					hratio_filtered_links,
					0.8f);




#ifndef NOUT
  if (enable_debug)
  {
    image2d<value::rgb8> overlap_decision_image
      = scribo::debug::decision_image(input,
				      hratio_filtered_links,
				      overlap_filtered_links);
    io::ppm::save(overlap_decision_image,
		  scribo::make::debug_filename("overlap_links_decision_image.ppm"));
  }
#endif




  object_groups<L>
    groups = primitive::group::from_single_link(filtered_objects,
						overlap_filtered_links);


  std::cout << "Grouping objects..." << std::endl;
  object_image(L)
    grouped_objects = primitive::group::apply(filtered_objects, groups);



  std::cout << "Saving line bboxes..." << std::endl;

  // 100p bboxes
  {
    std::ofstream out;
    out.open(argv[5], std::ios_base::trunc);

    if (! out)
    {
      std::cout << "Can't create output file!" << std::endl;
      return 1;
    }

    for_all_ncomponents(i, grouped_objects.nlabels())
    {
      out << grouped_objects.bbox(i).pmin().row() << " "
	  << grouped_objects.bbox(i).pmin().col()
	  << " "
	  << grouped_objects.bbox(i).pmax().row() << " "
	  << grouped_objects.bbox(i).pmax().col()
	  << std::endl;
    }
    out.close();

  }

  // 50p bboxes
  {
    std::ofstream out;
    out.open(argv[6], std::ios_base::trunc);

    if (! out)
    {
      std::cout << "Can't create output file!" << std::endl;
      return 1;
    }

    for_all_ncomponents(i, grouped_objects.nlabels())
    {
      out << grouped_objects.bbox(i).pmin().row() / 2 << " "
	  << grouped_objects.bbox(i).pmin().col() / 2
	  << " "
	  << grouped_objects.bbox(i).pmax().row() / 2 << " "
	  << grouped_objects.bbox(i).pmax().col() / 2
	  << std::endl;
    }
    out.close();

  }

  scribo::debug::save_bboxes_image(input, grouped_objects.bboxes(),
				   literal::red,
				   argv[2]);

  io::dump::save(filtered_objects, argv[3]);
  io::dump::save(grouped_objects, argv[4]);

  trace::exiting("main");
}
