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

#include <scribo/core/line_set.hh>

#include <scribo/primitive/extract/components.hh>
#include <scribo/primitive/extract/vertical_separators.hh>

#include <scribo/primitive/remove/separators.hh>

#include <scribo/primitive/link/merge_double_link.hh>
#include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
#include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>

#include <scribo/primitive/group/from_single_link.hh>

#include <scribo/filter/object_links_bbox_h_ratio.hh>


#include <scribo/debug/usage.hh>
#include <scribo/debug/save_bboxes_image.hh>
#include <scribo/debug/bboxes_enlarged_image.hh>
#include <scribo/debug/mean_and_base_lines_image.hh>
#include <scribo/debug/looks_like_a_text_line_image.hh>

#include <scribo/make/debug_filename.hh>

#include <scribo/text/recognition.hh>

#include <scribo/text/merging.hh>


#include <scribo/preprocessing/denoise_fg.hh>


// #include <mln/morpho/closing/structural.hh>
// #include <mln/win/rectangle2d.hh>


const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'False' for object, 'True'\
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
  mln::io::pbm::load(input, argv[1]);

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

  mln::io::pbm::save(separators, "vseparators.pbm");
//   mln::io::pbm::save(whitespaces, "separators.pbm");

//   mln::io::pbm::save(input_cleaned, "input_no_separators.pbm");

  // Denoise
  std::cout << "Denoise..." << std::endl;
  input_cleaned = preprocessing::denoise_fg(input_cleaned, c8(), 3);

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


  /// Linking potential objects
  std::cout << "Linking objects..." << std::endl;
  object_links<L> left_link
    = primitive::link::with_single_left_link_dmax_ratio(components, 2);
  object_links<L> right_link
    = primitive::link::with_single_right_link_dmax_ratio(components, 2);

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

  // Bboxes image.
  scribo::debug::save_bboxes_image(input, lines,
				   scribo::make::debug_filename("step1_bboxes.ppm"));

  // Bboxes enlarged
  mln::io::ppm::save(scribo::debug::bboxes_enlarged_image(input, lines),
		     scribo::make::debug_filename("step1_bboxes_enlarged.ppm"));

  // Looks like a text line
  mln::io::ppm::save(scribo::debug::looks_like_a_text_line_image(input, lines),
		     scribo::make::debug_filename("step1_looks_like_a_text_line.ppm"));


  // Bboxes + line infos
  {
    std::ofstream file(scribo::make::debug_filename("step1_bboxes_100p.txt").c_str());
    std::ofstream file_50p(scribo::make::debug_filename("step1_bboxes_50p.txt").c_str());

    for_all_lines(l, lines)
      if (lines(l).tag() != line::Merged
	  && lines(l).tag() != line::Ignored
	  && lines(l).tag() != line::Pathological)
      {
	file << lines(l).bbox().pmin().row() << " "
	     << lines(l).bbox().pmin().col() << " "
	     << lines(l).bbox().pmax().row() << " "
	     << lines(l).bbox().pmax().col() << " "
	     << lines(l).card()              << " "
	     << lines(l).baseline()          << " "
	     << lines(l).x_height()          << " "
	     << lines(l).meanline()          << " "
	     << lines(l).d_height()          << " "
	     << lines(l).a_height()          << " "
	     << lines(l).char_space()        << " "
	     << lines(l).char_width()        << std::endl;

	file_50p << lines(l).bbox().pmin().row() / 2 << " "
		 << lines(l).bbox().pmin().col() / 2 << " "
		 << lines(l).bbox().pmax().row() / 2 << " "
		 << lines(l).bbox().pmax().col() / 2 << std::endl;
      }

    file.close();
    file_50p.close();
  }


  // mean and base lines.
  mln::io::ppm::save(scribo::debug::mean_and_base_lines_image(input, lines),
		     scribo::make::debug_filename("step1_x_height.ppm"));

  //===== END OF DEBUG =====




   std::cout << "Merging lines..." << std::endl;
   lines = scribo::text::merging(lines);



   //===== DEBUG =====

   // mean and base lines.
   mln::io::ppm::save(scribo::debug::mean_and_base_lines_image(input, lines),
		      scribo::make::debug_filename("step2_x_height.ppm"));

  // Looks like a text line
  mln::io::ppm::save(scribo::debug::looks_like_a_text_line_image(input, lines),
		     scribo::make::debug_filename("step2_looks_like_a_text_line.ppm"));

  // Bboxes image.
  scribo::debug::save_bboxes_image(input, lines, argv[2]);

  //===== END OF DEBUG =====



  scribo::text::recognition(lines, "fra", "out.txt");


//   // Display median character space.
//   {
//     image2d<value::rgb8> output = data::convert(value::rgb8(), input);
//     typedef mln::value::int_u<8> median_t;
//     typedef mln::accu::stat::median_h<median_t> accu_t;
//     util::array<accu_t>
//       lspace_med(static_cast<unsigned>(grouped_objects.nlabels()) + 1);

//     for_all_components(i, filtered_objects.bboxes())
//       if (groups_packed(i) != 0)
//       {
// 	if (hratio_filtered_links(i) != i)
// 	{
// 	  unsigned
// 	    space = filtered_objects.bbox(i).pmin().col() - filtered_objects.bbox(hratio_filtered_links(i)).pmax().col();

// 	  lspace_med(groups_packed(i)).take(space);

// 	}
//       }

//     std::cout << "Drawing median character space" << std::endl;
//     for_all_components(i, filtered_objects.bboxes())
//       if (groups_packed(i) != 0 && lspace_med(groups_packed(i)).card() > 1)
//       {
// 	unsigned med = lspace_med(groups_packed(i)).to_result();

// 	mln::draw::box(output, grouped_objects.bbox(groups_packed(i)),
// 		       literal::purple);

// 	point2d
// 	  beg = filtered_objects.bbox(i).pmax(),
// 	  end = beg;
// 	beg.row() = filtered_objects.bbox(i).pmin().row();
// 	mln::draw::line(output, beg, end, literal::cyan);
// 	beg.col() += med;
// 	end.col() += med;
// 	mln::draw::line(output, beg, end, literal::cyan);

//       }

//     io::ppm::save(output, "median_wspace.ppm");

//   }

  trace::exiting("main");
}
