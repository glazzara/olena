// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_TOOLCHAIN_INTERNAL_CONTENT_IN_DOC_FUNCTOR_HH
# define SCRIBO_TOOLCHAIN_INTERNAL_CONTENT_IN_DOC_FUNCTOR_HH

# include <mln/io/ppm/save.hh>

# include <scribo/core/def/lbl_type.hh>
# include <scribo/core/document.hh>
# include <scribo/core/line_set.hh>
# include <scribo/core/paragraph_set.hh>

# include <scribo/primitive/extract/non_text.hh>
# include <scribo/primitive/extract/components.hh>
# include <scribo/primitive/extract/separators.hh>
# include <scribo/primitive/extract/separators_nonvisible.hh>

# include <scribo/primitive/identify.hh>

# include <scribo/primitive/remove/separators.hh>

# include <scribo/filter/line_links_x_height.hh>
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
# include <scribo/text/link_lines.hh>

# include <scribo/make/debug_filename.hh>

# include <scribo/debug/save_bboxes_image.hh>
# include <scribo/debug/save_linked_bboxes_image.hh>
# include <scribo/debug/bboxes_enlarged_image.hh>
# include <scribo/debug/mean_and_base_lines_image.hh>
# include <scribo/debug/looks_like_a_text_line_image.hh>

# include <scribo/toolchain/internal/toolchain_functor.hh>

# include <scribo/io/xml/save.hh>


namespace scribo
{

  namespace toolchain
  {

    namespace internal
    {


      template <typename I>
      struct content_in_doc_functor
	: public Toolchain_Functor
      {
	typedef value::label<30> V;
	typedef mln_ch_value(I,V) L;

	content_in_doc_functor(const char *doc_filename);

	virtual int nsteps() const;

	virtual void on_xml_saved();

	//===============
	// Core function
	//===============

	template <typename J>
	scribo::document<L> operator()(const Image<J>& original_image,
				       const Image<I>& processed_image);


	//=========
	// Options
	//=========
	bool enable_denoising;
	bool enable_line_seps;
	bool enable_whitespace_seps;
	bool enable_ocr;
	bool enable_debug;
	bool save_doc_as_xml;
	scribo::io::xml::Format xml_format;

	//============
	// Parameters
	//============

	std::string ocr_language;
	std::string output_file;

	//=========
	// Results
	//=========
	document<L> doc;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      content_in_doc_functor<I>::content_in_doc_functor(const char *doc_filename)
	: enable_denoising(true),
	  enable_line_seps(true),
	  enable_whitespace_seps(true),
	  enable_ocr(true),
	  enable_debug(false),
	  save_doc_as_xml(false),
	  xml_format(scribo::io::xml::PageExtended),
	  ocr_language("eng"),
	  output_file("/tmp/foo.xml"),
	  doc(doc_filename)
      {
      }


      //===============
      // Core function
      //===============

      template <typename I>
      template <typename J>
      scribo::document<typename content_in_doc_functor<I>::L>
      content_in_doc_functor<I>::operator()(const Image<J>& original_image,
					    const Image<I>& processed_image)
      {
	mln_precondition(exact(original_image).is_valid());
	mln_precondition(exact(processed_image).is_valid());

	doc.set_image(exact(original_image));
	doc.set_binary_image(exact(processed_image));

	// Remove separators
	mln_ch_value(I,bool)
	  separators,
	  input_cleaned = exact(processed_image);
	if (enable_line_seps)
	{
	  // FIXME: SLOW
	  on_new_progress_label("Find vertical and horizontal separators...");

	  // Vertical and horizontal separators
	  {
	    mln_ch_value(I,bool)
	      vseparators = primitive::extract::vertical_separators(processed_image, 81),
	      hseparators = primitive::extract::horizontal_separators(processed_image, 81);

	    doc.set_vline_separators(vseparators);
	    doc.set_hline_separators(hseparators);

	    separators = vseparators;
	    separators += hseparators;

	    border::resize(processed_image, border::thickness);
	  }

	  on_progress();

	  on_new_progress_label("Remove separators...");

	  input_cleaned = primitive::remove::separators(processed_image,
							separators);

	  on_progress();
	}

	mln_ch_value(I,bool) whitespaces;
	if (enable_whitespace_seps)
	{
	  // Whitespace separators
	  on_new_progress_label("Find whitespace separators...");

	  whitespaces = primitive::extract::separators_nonvisible(input_cleaned);

	  on_progress();
	}

	if (enable_debug)
	{
	  if (enable_whitespace_seps)
	    mln::io::pbm::save(whitespaces,
			       scribo::make::debug_filename("whitespaces.pbm"));

	  if (enable_line_seps)
	  {
	    mln::io::pbm::save(separators,
			       scribo::make::debug_filename("vseparators.pbm"));

	    mln::io::pbm::save(input_cleaned,
			       scribo::make::debug_filename("input_wo_vseparators.pbm"));
	  }
	}

	// Denoise
	if (enable_denoising)
	{
	  on_new_progress_label("Denoise...");

	  input_cleaned = preprocessing::denoise_fg(input_cleaned, c8(), 3);

	  if (enable_debug)
	    mln::io::pbm::save(input_cleaned,
			       scribo::make::debug_filename("denoised.pbm"));

	  on_progress();
	}

	/// Finding components.
	on_new_progress_label("Finding components...");

	V ncomponents;
	component_set<L>
	  components = scribo::primitive::extract::components(input_cleaned, c8(),
							      ncomponents);

	on_progress();

	/// Set separator components.
	if (enable_line_seps)
	  components.add_separators(separators);
	if (enable_whitespace_seps)
	{
	  components.add_separators(whitespaces);
	  doc.set_whitespace_separators(whitespaces);
	}

	if (enable_debug)
	  mln::io::pbm::save(components.separators(),
			     scribo::make::debug_filename("all_separators.pbm"));


	on_new_progress_label("Filtering components");

	components = scribo::filter::components_small(components, 3);

	on_progress();


	/// Linking objects
	on_new_progress_label("Linking objects...");

	object_links<L> left_link
	  = primitive::link::with_single_left_link_dmax_ratio(components,
							      primitive::link::internal::dmax_width_and_height(1),
							      anchor::MassCenter);
	object_links<L> right_link
	  = primitive::link::with_single_right_link_dmax_ratio(components,
							       primitive::link::internal::dmax_width_and_height(1),
							       anchor::MassCenter);

	if (enable_debug)
	{
	  debug::save_linked_bboxes_image(processed_image, left_link, right_link,
					  literal::blue,
					  literal::cyan,
					  literal::yellow,
					  literal::green,
					  anchor::MassCenter,
					  scribo::make::debug_filename("object_links.ppm"));
	}


	// Validating left and right links.
	object_links<L>
	  merged_links = primitive::link::merge_double_link(left_link,
							    right_link);

	on_progress();


	on_new_progress_label("Filtering objects");

	// Remove links if bboxes have too different sizes.
	object_links<L> hratio_filtered_links
	  = filter::object_links_bbox_h_ratio(merged_links, 2.5f);


	if (enable_debug)
	{
	  mln_ch_value(I,value::rgb8)
	    hratio_decision_image = scribo::debug::decision_image(processed_image,
								  merged_links,
								  hratio_filtered_links,
								  anchor::MassCenter);
	  mln::io::ppm::save(hratio_decision_image,
			     scribo::make::debug_filename("hratio_links_decision_image.ppm"));
	}

	on_progress();


	on_new_progress_label("Rebuilding lines");

	object_groups<L>
	  groups = primitive::group::from_single_link(hratio_filtered_links);



	// Construct a line set.
	line_set<L>
	  lines = scribo::make::line_set(groups);


	//===== DEBUG =====

	if (enable_debug)
	{

	  // Bboxes image.
	  scribo::debug::save_bboxes_image(processed_image, lines,
					   scribo::make::debug_filename("step1_bboxes.ppm"));

	  // Bboxes enlarged
	  mln::io::ppm::save(scribo::debug::bboxes_enlarged_image(processed_image, lines),
			     scribo::make::debug_filename("step1_bboxes_enlarged.ppm"));

	  // Looks like a text line
	  mln::io::ppm::save(scribo::debug::looks_like_a_text_line_image(processed_image, lines),
			     scribo::make::debug_filename("step1_looks_like_a_text_line.ppm"));


	  // mean and base lines.
	  mln::io::ppm::save(scribo::debug::mean_and_base_lines_image(processed_image, lines),
			     scribo::make::debug_filename("step1_x_height.ppm"));

	}
	//===== END OF DEBUG =====



	lines = scribo::text::merging(lines);


	//===== DEBUG =====
	if (enable_debug)
	{

	  // mean and base lines.
	  mln::io::ppm::save(scribo::debug::mean_and_base_lines_image(processed_image, lines),
			     scribo::make::debug_filename("step2_x_height.ppm"));

	  // Looks like a text line
	  mln::io::ppm::save(scribo::debug::looks_like_a_text_line_image(processed_image, lines),
			     scribo::make::debug_filename("step2_looks_like_a_text_line.ppm"));

	  // Bboxes image.
	  scribo::debug::save_bboxes_image(processed_image, lines,
					   scribo::make::debug_filename("step2_bboxes.ppm"));


	}
	//===== END OF DEBUG =====

	on_progress();


	// Text recognition
	if (enable_ocr)
	{
	  on_new_progress_label("Recognizing text");

	  scribo::text::recognition(lines, ocr_language.c_str());

	  on_progress();
	}

	// Link text lines
	on_new_progress_label("Linking text lines");
	line_links<L> llinks = scribo::text::link_lines(lines);


	//===== DEBUG =====
	if (enable_debug)
	{
	  image2d<value::rgb8> debug = data::convert(value::rgb8(), original_image);
	  for_all_lines(l, lines)
	  {
	    if (! lines(l).is_textline())
	      continue;

	    mln::draw::box(debug, lines(l).bbox(), literal::blue);
	    mln::draw::line(debug, lines(l).bbox().pcenter(), lines(llinks(l)).bbox().pcenter(), literal::green);
	  }

	  mln::io::ppm::save(debug, scribo::make::debug_filename("links_raw.ppm"));
	}
	//===== END OF DEBUG =====

	on_progress();


	// Filter line links.
	on_new_progress_label("Filter line links");
	llinks = scribo::filter::line_links_x_height(llinks);

	//===== DEBUG =====
	if (enable_debug)
	{
	  image2d<value::rgb8> debug = data::convert(value::rgb8(), original_image);
	  for_all_links(i, llinks)
	    if (llinks(i) && llinks(i) != i)
	      mln::draw::line(debug, lines(i).bbox().pcenter(),
			      lines(llinks(i)).bbox().pcenter(), literal::red);

	  mln::io::ppm::save(debug, scribo::make::debug_filename("links.ppm"));


	  for (unsigned i = 1; i < llinks.nelements(); ++i)
	    llinks(i) = scribo::make::internal::find_root(llinks, i);

	  debug = data::convert(value::rgb8(), original_image);
	  mln::util::array<accu::shape::bbox<point2d> > nbbox(llinks.nelements());
	  for_all_lines(i, lines)
	  {
	    if (! lines(i).is_textline())
	      continue;

	    mln::draw::box(debug, lines(i).bbox(), literal::red);
	    nbbox(llinks(i)).take(lines(i).bbox());
	  }

	  for (unsigned i = 1; i < nbbox.nelements(); ++i)
	    if (nbbox(i).is_valid())
	    {
	      box2d b = nbbox(i).to_result();
	      mln::draw::box(debug, b, literal::green);
	      b.enlarge(1);
	      mln::draw::box(debug, b, literal::green);
	      b.enlarge(1);
	      mln::draw::box(debug, b, literal::green);
	    }

	  mln::io::ppm::save(debug, scribo::make::debug_filename("par.ppm"));
	}
	//===== END OF DEBUG =====

	on_progress();


	// Construct paragraphs
	on_new_progress_label("Constructing paragraphs");
	scribo::paragraph_set<L> parset = scribo::make::paragraph(llinks);
	doc.set_paragraphs(parset);

	on_progress();


	// Extract other Elements
	on_new_progress_label("Extracting Elements");
	component_set<L>
	  elements = scribo::primitive::extract::non_text(doc, original_image);

	on_progress();


	// Identify other Elements
	on_new_progress_label("Identifying Elements");
	elements = scribo::primitive::identify(elements);
	doc.set_elements(elements);

	on_progress();



	// Saving results
	if (save_doc_as_xml)
	{
	  on_new_progress_label("Saving results");

	  scribo::io::xml::save(doc, output_file, xml_format);
	  on_xml_saved();

	  on_progress();
	}

	return doc;
      }



      template<typename I>
      int
      content_in_doc_functor<I>::nsteps() const
      {
	return 10 + enable_denoising + enable_line_seps
	  + enable_whitespace_seps + enable_ocr + save_doc_as_xml;
      }


      template<typename I>
      void
      content_in_doc_functor<I>::on_xml_saved()
      {
	// Nothing
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::toolchain::internal

  } // end of namespace scribo::toolchain

} // end of namespace scribo

#endif // ! SCRIBO_TOOLCHAIN_INTERNAL_CONTENT_IN_DOC_FUNCTOR_HH
