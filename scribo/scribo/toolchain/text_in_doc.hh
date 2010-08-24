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

#ifndef SCRIBO_TOOLCHAIN_TEXT_IN_DOC_HH
# define SCRIBO_TOOLCHAIN_TEXT_IN_DOC_HH

/// \file
///
/// Extract text from a document.

# include <mln/io/ppm/save.hh>

# include <scribo/core/def/lbl_type.hh>

# include <scribo/primitive/extract/components.hh>
# include <scribo/primitive/extract/vertical_separators.hh>

# include <scribo/primitive/remove/separators.hh>

# include <scribo/filter/object_links_bbox_h_ratio.hh>
# include <scribo/filter/objects_small.hh>

# include <scribo/primitive/group/from_single_link.hh>

# include <scribo/primitive/link/merge_double_link.hh>
# include <scribo/primitive/link/internal/dmax_width_and_height.hh>
# include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
# include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>

# include <scribo/preprocessing/denoise_fg.hh>

# include <scribo/text/recognition.hh>
# include <scribo/text/merging.hh>

# include <scribo/make/debug_filename.hh>

# include <scribo/debug/save_bboxes_image.hh>
# include <scribo/debug/bboxes_enlarged_image.hh>
# include <scribo/debug/mean_and_base_lines_image.hh>
# include <scribo/debug/looks_like_a_text_line_image.hh>



namespace scribo
{

  namespace toolchain
  {

    template <typename I>
    line_set<mln_ch_value(I, def::lbl_type)>
    text_in_doc(const Image<I>& input, bool denoise, bool debug = false);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    line_set<mln_ch_value(I, def::lbl_type)>
    text_in_doc(const Image<I>& input, bool denoise, bool debug = false)
    {
      typedef value::label<30> V;
      typedef image2d<V> L;

      // Add whitespace separators.
      //   win::rectangle2d win = win::rectangle2d(151, 41);
      //   image2d<bool> whitespaces = morpho::closing::structural(input, win);
      //   logical::not_inplace(whitespaces);

      // Remove separators
      if (debug)
	std::cout << "Find vertical separators..." << std::endl;

      image2d<bool>
	separators = primitive::extract::vertical_separators(input, 81);

      if (debug)
	std::cout << "Remove separators..." << std::endl;

      image2d<bool> input_cleaned = primitive::remove::separators(input,
								  separators);

//   whitespaces += separators;

      if (debug)
      {
	mln::io::pbm::save(separators,
			   scribo::make::debug_filename("vseparators.pbm"));
//   mln::io::pbm::save(whitespaces, "separators.pbm");

	mln::io::pbm::save(input_cleaned, scribo::make::debug_filename("input_wo_vseparators.pbm"));
      }

      // Denoise
      if (denoise)
      {
	if (debug)
	  std::cout << "Denoise..." << std::endl;

	input_cleaned = preprocessing::denoise_fg(input_cleaned, c8(), 3);

	if (debug)
	  mln::io::pbm::save(input_cleaned,
			     scribo::make::debug_filename("denoised.pbm"));
      }

      /// Finding components.
      if (debug)
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
      if (debug)
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
	merged_links = primitive::link::merge_double_link(left_link,
							  right_link);


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


      object_groups<L>
	groups = primitive::group::from_single_link(hratio_filtered_links);


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


	// mean and base lines.
	mln::io::ppm::save(scribo::debug::mean_and_base_lines_image(input, lines),
			   scribo::make::debug_filename("step1_x_height.ppm"));

      }
      //===== END OF DEBUG =====



      if (debug)
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



      if (debug)
      {
	std::ofstream file(scribo::make::debug_filename("step2_bboxes_100p.txt").c_str());

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

	  }

	file.close();
      }


      //===== END OF DEBUG =====



      scribo::text::recognition(lines, "fra");

      return lines;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::toolchain

} // end of namespace scribo


#endif // SCRIBO_TOOLCHAIN_TEXT_IN_DOC_HH

