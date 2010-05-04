// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
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
#include <fstream>
#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/dump/save.hh>

#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>

#include <mln/data/paste_without_localization.hh>

#include <scribo/core/line_set.hh>

#include <scribo/primitive/extract/components.hh>
#include <scribo/primitive/extract/vertical_separators.hh>

#include <scribo/primitive/remove/separators.hh>

#include <scribo/primitive/link/merge_double_link.hh>
#include <scribo/primitive/link/internal/dmax_width_and_height.hh>
#include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
#include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>

#include <scribo/primitive/group/from_single_link.hh>

#include <scribo/filter/object_links_bbox_h_ratio.hh>
#include <scribo/filter/objects_small.hh>


#include <scribo/debug/usage.hh>
#include <scribo/debug/save_bboxes_image.hh>
#include <scribo/debug/bboxes_enlarged_image.hh>
#include <scribo/debug/mean_and_base_lines_image.hh>
#include <scribo/debug/looks_like_a_text_line_image.hh>

#include <scribo/make/debug_filename.hh>

#include <scribo/text/recognition.hh>

#include <scribo/text/merging.hh>


#include <scribo/preprocessing/denoise_fg.hh>

#include <scribo/io/xml/save_text_lines.hh>

// #include <mln/morpho/closing/structural.hh>
// #include <mln/win/rectangle2d.hh>


const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'False' for object, 'True'\
for the background." },
  { "out.txt", "Text output" },
  { "denoise", "1 enables denoising, 0 disables it. (enabled by default)" },
  { "pmin_row", "Row index of the top left corner of the Region of interest." },
  { "pmin_col", "Col index of the top left corner of the Region of interest." },
  { "pmax_row", "Row index of the bottom right corner of the Region of interest." },
  { "pmax_col", "Col index of the bottom right corner of the Region of interest." },
  { "debug_dir", "Output directory for debug image" },
  {0, 0}
};


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 3 && argc != 4 && argc != 5 && argc != 8 && argc != 9)
    return scribo::debug::usage(argv,
				"Find text lines using left/right validation and display x-height in a binarized article.",
				"input.pbm out.txt <denoise: 0|1> [<pmin_row> <pmin_col> <pmax_row> <pmax_col>] <debug_dir>",
				args_desc);

  bool debug = false;

  // Enable debug output.
  if (argc == 5 || argc == 9)
  {
    scribo::make::internal::debug_filename_prefix = argv[argc - 1];
    debug = true;
  }

  trace::entering("main");


  image2d<bool> input;
  mln::io::pbm::load(input, argv[1]);


  // Optional Cropping
  if (argc >= 8)
  {
    def::coord
      minr = atoi(argv[4]),
      minc = atoi(argv[5]),
      maxr = atoi(argv[6]),
      maxc = atoi(argv[7]);

    box2d roi = mln::make::box2d(minr, minc, maxr, maxc);
    image2d<bool> tmp(maxr - minr + 1, maxc - minc + 1);
    data::paste_without_localization(input | roi, tmp);
    input = tmp;

    if (debug)
      mln::io::pbm::save(input,
			 scribo::make::debug_filename("input_cropped.pbm"));
  }


  typedef value::label_16 V;
  typedef image2d<V> L;


  // Add whitespace separators.
//   win::rectangle2d win = win::rectangle2d(151, 41);
//   image2d<bool> whitespaces = morpho::closing::structural(input, win);
//   logical::not_inplace(whitespaces);

  // Remove separators
  std::cout << "Find vertical separators..." << std::endl;
  image2d<bool> separators = primitive::extract::vertical_separators(input, 81);

  std::cout << "Remove separators..." << std::endl;
  image2d<bool> input_cleaned = primitive::remove::separators(input,
							      separators);

//   whitespaces += separators;

  if (debug)
    mln::io::pbm::save(separators,
		       scribo::make::debug_filename("vseparators.pbm"));
//   mln::io::pbm::save(whitespaces, "separators.pbm");

//   mln::io::pbm::save(input_cleaned, "input_no_separators.pbm");

  // Denoise
  if (argc > 3 && atoi(argv[3]) != 0)
  {
    std::cout << "Denoise..." << std::endl;
    input_cleaned = preprocessing::denoise_fg(input_cleaned, c8(), 3);
  }

//   mln::io::pbm::save(input_cleaned, "input_denoised.pbm");

  /// Finding components.
  std::cout << "Finding components..." << std::endl;
  V ncomponents;
  component_set<L>
    components = scribo::primitive::extract::components(input_cleaned, c8(),
							ncomponents);

  /// Set separator components.
  components.add_separators(separators);
//  components.add_separators(whitespaces);

  components = scribo::filter::components_small(components, 3);


  /// Linking potential objects
  std::cout << "Linking objects..." << std::endl;
  object_links<L> left_link
    = primitive::link::with_single_left_link_dmax_ratio(components,
							primitive::link::internal::dmax_width_and_height(1),
							anchor::MassCenter);
  object_links<L> right_link
    = primitive::link::with_single_right_link_dmax_ratio(components,
							 primitive::link::internal::dmax_width_and_height(1),
							 anchor::MassCenter);

  // Validating left and right links.
  object_links<L>
    merged_links = primitive::link::merge_double_link(left_link, right_link);


  // Remove links if bboxes have too different sizes.
  object_links<L> hratio_filtered_links
    = filter::object_links_bbox_h_ratio(merged_links, 2.5f);


// #ifndef NOUT
//   if (argc == 4)
//   {
//     image2d<value::rgb8>
//       hratio_decision_image = scribo::debug::decision_image(input,
// 							    merged_links,
// 							    hratio_filtered_links);
//     io::ppm::save(hratio_decision_image,
// 		  scribo::make::debug_filename("hratio_links_decision_image.ppm"));
//   }
// #endif



//   // FIXME: from_single_link should return a packed object_groups?
//   //
//   //######
  object_groups<L>
    groups = primitive::group::from_single_link(hratio_filtered_links);
//   value::label_16 n_groups;
//   mln::fun::i2v::array<value::label_16>
//     groups_packed = mln::make::relabelfun(groups,
// 					  filtered_objects.nlabels(),
// 					  n_groups);
//   //######



  // Construct a line set.
  line_set<L>
    lines = scribo::make::line_set(groups);



  //===== DEBUG =====

  if (debug)
  {

    // Bboxes image.
    scribo::debug::save_bboxes_image(input, lines,
				     scribo::make::debug_filename("step1_bboxes.ppm"));

    // Bboxes enlarged
    mln::io::ppm::save(scribo::debug::bboxes_enlarged_image(input, lines),
		       scribo::make::debug_filename("step1_bboxes_enlarged.ppm"));

    // Looks like a text line
    mln::io::ppm::save(scribo::debug::looks_like_a_text_line_image(input, lines),
		       scribo::make::debug_filename("step1_looks_like_a_text_line.ppm"));

//   // Bboxes + line infos
//   {
//     std::ofstream file(scribo::make::debug_filename("step1_bboxes_100p.txt").c_str());
// //     std::ofstream file_50p(scribo::make::debug_filename("step1_bboxes_50p.txt").c_str());

//     for_all_lines(l, lines)
//       if (lines(l).tag() != line::Merged
// 	  && lines(l).tag() != line::Ignored
// 	  && lines(l).tag() != line::Pathological)
//       {
// 	file << lines(l).bbox().pmin().row() << " "
// 	     << lines(l).bbox().pmin().col() << " "
// 	     << lines(l).bbox().pmax().row() << " "
// 	     << lines(l).bbox().pmax().col() << " "
// 	     << lines(l).card()              << " "
// 	     << lines(l).baseline()          << " "
// 	     << lines(l).x_height()          << " "
// 	     << lines(l).meanline()          << " "
// 	     << lines(l).d_height()          << " "
// 	     << lines(l).a_height()          << " "
// 	     << lines(l).char_space()        << " "
// 	     << lines(l).char_width()        << std::endl;

// // 	file_50p << lines(l).bbox().pmin().row() / 2 << " "
// // 		 << lines(l).bbox().pmin().col() / 2 << " "
// // 		 << lines(l).bbox().pmax().row() / 2 << " "
// // 		 << lines(l).bbox().pmax().col() / 2 << std::endl;
//       }

//     file.close();
// //     file_50p.close();
//   }


    // mean and base lines.
    mln::io::ppm::save(scribo::debug::mean_and_base_lines_image(input, lines),
		       scribo::make::debug_filename("step1_x_height.ppm"));

  }
  //===== END OF DEBUG =====




   std::cout << "Merging lines..." << std::endl;
   lines = scribo::text::merging(lines);



   //===== DEBUG =====

   if (debug)
   {

     // mean and base lines.
     mln::io::ppm::save(scribo::debug::mean_and_base_lines_image(input, lines),
			scribo::make::debug_filename("step2_x_height.ppm"));

     // Looks like a text line
     mln::io::ppm::save(scribo::debug::looks_like_a_text_line_image(input, lines),
			scribo::make::debug_filename("step2_looks_like_a_text_line.ppm"));

     // Bboxes image.
     scribo::debug::save_bboxes_image(input, lines,
				      scribo::make::debug_filename("step2_bboxes.ppm"));


   }


//    {
//      std::ofstream file(scribo::make::debug_filename("step2_bboxes_100p.txt").c_str());
// //     std::ofstream file_50p(scribo::make::debug_filename("step2_bboxes_50p.txt").c_str());

//      for_all_lines(l, lines)
//        if (lines(l).tag() != line::Merged
// 	   && lines(l).tag() != line::Ignored
// 	   && lines(l).tag() != line::Pathological)
//        {
// 	 file << lines(l).bbox().pmin().row() << " "
// 	      << lines(l).bbox().pmin().col() << " "
// 	      << lines(l).bbox().pmax().row() << " "
// 	      << lines(l).bbox().pmax().col() << " "
// 	      << lines(l).card()              << " "
// 	      << lines(l).baseline()          << " "
// 	      << lines(l).x_height()          << " "
// 	      << lines(l).meanline()          << " "
// 	      << lines(l).d_height()          << " "
// 	      << lines(l).a_height()          << " "
// 	      << lines(l).char_space()        << " "
// 	      << lines(l).char_width()        << std::endl;

// // 	file_50p << lines(l).bbox().pmin().row() / 2 << " "
// // 		 << lines(l).bbox().pmin().col() / 2 << " "
// // 		 << lines(l).bbox().pmax().row() / 2 << " "
// // 		 << lines(l).bbox().pmax().col() / 2 << std::endl;
//        }

//      file.close();
// //     file_50p.close();
//    }


   //===== END OF DEBUG =====


   scribo::io::xml::save_text_lines(argv[1], lines, "out.xml");



  scribo::text::recognition(lines, "fra", argv[2]);


  trace::exiting("main");
}
