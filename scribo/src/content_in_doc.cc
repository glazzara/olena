// Copyright (C) 2010, 2011, 2012 EPITA Research and Development
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


#include <libgen.h>
#include <fstream>
#include <iostream>

#include <mln/core/image/image2d.hh>

#include <mln/io/pbm/save.hh>
#include <mln/io/magick/load.hh>

#include <scribo/toolchain/content_in_doc.hh>
#include <scribo/toolchain/text_in_doc_preprocess.hh>

#include <scribo/core/document.hh>

#include <scribo/debug/usage.hh>
#include <scribo/debug/logger.hh>

#include <scribo/preprocessing/crop_without_localization.hh>
#include <scribo/preprocessing/crop.hh>

#include <scribo/io/xml/save.hh>
#include <scribo/io/img/save.hh>

#include <scribo/debug/option_parser.hh>


static const scribo::debug::arg_data arg_desc[] =
{
  { "input.*", "An image." },
  { "out.xml", "Result of the document analysis" },
  {0, 0}
};


// --enable/disable-<name>
static const scribo::debug::toggle_data toggle_desc[] =
{
  // name, description, default value
  { "denoising", "Performs a denoising. (default: enabled)", true },
  { "find-delims", "Find text alignements and whitespaces "
    "to improve layout detection. (default: enabled)", true },
  { "find-seps", "Find separators in document (default: enabled)", true },
  { "ocr", "Performs character recognition (default: enabled)", true },
  { "deskew", "Deskew image (default: disabled)", false},
  {0, 0, false}
};


// --<name> <args>
static const scribo::debug::opt_data opt_desc[] =
{
  // name, description, arguments, check args function, number of args, default arg
  { "crop", "Crop input image before processing it.",
    "<pmin_row> <pmin_col> <pmax_row> <pmax_col>", 0, 4, 0 },
  { "debug-prefix", "Enable debug image outputs. Prefix image name with that "
    "given prefix.", "<prefix>", 0, 1, 0 },
  { "ocr-lang", "Set the language to be recognized by the OCR (Tesseract). "
    "Depending on your system, you can choose between eng (default), "
    "fra, deu, ita, nld, por, spa, vie",
    "<lang>", scribo::debug::check_ocr_lang, 1, "eng" },
  { "verbose", "Enable verbose mode", 0, 0, 0, 0 },
  { "xml-format", "Choose betwen page, page-ext and full (default: page-ext).", "<format>",
    scribo::debug::check_xml_format, 1, "page-ext" },
  { "more-xml-format", "Provide an additional xml output. Format can"
    " be chosen between page, page-ext and full (default: page-ext).", "<format>",
    scribo::debug::check_xml_format, 1, "none" },
  { "more-xml-file", "Filename of the additional xml output.", "<filename>",
    0, 1, "/dev/null" },
  { "debug-regions", "Save a debug image with all the regions.", "<filename>",
    0, 1, "/dev/null" },
  {0, 0, 0, 0, 0, 0}
};


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  scribo::debug::option_parser options(arg_desc, toggle_desc, opt_desc);

  if (!options.parse(argc, argv))
    return 1;

  // Enable debug output.
  if (options.is_set("debug-prefix"))
  {
    scribo::debug::logger().set_filename_prefix(options.opt_value("debug-prefix").c_str());
    scribo::debug::logger().set_level(scribo::debug::All);
  }

  bool verbose = options.is_set("verbose");

  mln_trace("main");

  typedef image2d<scribo::def::lbl_type> L;
  image2d<value::rgb8> input;
  mln::io::magick::load(input, options.arg("input.*"));

  bool enable_deskew = options.is_enabled("deskew");
  // Preprocess document
  image2d<bool>
    input_preproc = toolchain::text_in_doc_preprocess(input, false, 0,
						      0.34, enable_deskew, verbose);

  // Optional Cropping
  point2d crop_shift = literal::origin;
  if (options.is_set("crop"))
  {
    std::vector<const char *> values = options.opt_values("crop");
    mln::def::coord
      minr = atoi(values[0]),
      minc = atoi(values[1]),
      maxr = atoi(values[2]),
      maxc = atoi(values[3]);

    if (verbose)
      std::cout << "> Image cropped from (" << minr << "," << minc << ")"
		<< " to (" << maxr << "," << maxc << ")" << std::endl;

    box2d roi = mln::make::box2d(minr, minc, maxr, maxc);
    input_preproc = preprocessing::crop_without_localization(input_preproc, roi);
    crop_shift = point2d(minr, minc);

    scribo::debug::logger().log_image(scribo::debug::Results, input_preproc,
				      "input_preproc_cropped.pbm");
  }

  bool denoise = options.is_enabled("denoising");
  std::string language = options.opt_value("ocr-lang");
  bool find_line_seps = options.is_enabled("find-seps");
  bool find_whitespace_seps = options.is_enabled("find-delims");
  bool enable_ocr = options.is_enabled("ocr");

  if (verbose)
    std::cout << "Running with the following options :"
	      << " ocr_language = " << language
	      << " | find_lines_seps = " << find_line_seps
	      << " | find_whitespace_seps = " << find_whitespace_seps
	      << " | debug = " << scribo::debug::logger().is_enabled()
	      << std::endl;

  // Run document toolchain.

  // Text
  if (verbose)
    std::cout << "Analysing document..." << std::endl;
  document<L>
    doc = scribo::toolchain::content_in_doc(input, input_preproc, denoise,
					    find_line_seps, find_whitespace_seps,
					    enable_ocr, language, verbose);

  // Saving results
  if (verbose)
    std::cout << "Saving results..." << std::endl;

  // Default XML output
  if (options.opt_value("xml-format") == "page-ext")
    scribo::io::xml::save(doc, options.arg("out.xml"), scribo::io::xml::PageExtended);
  if (options.opt_value("xml-format") == "page")
    scribo::io::xml::save(doc, options.arg("out.xml"), scribo::io::xml::Page);
  if (options.opt_value("xml-format") == "full")
    scribo::io::xml::save(doc, options.arg("out.xml"), scribo::io::xml::Full);

  // Additional XML output
  if (options.opt_value("more-xml-format") == "page-ext")
    scribo::io::xml::save(doc, options.opt_value("more-xml-file"), scribo::io::xml::PageExtended);
  if (options.opt_value("more-xml-format") == "page")
    scribo::io::xml::save(doc, options.opt_value("more-xml-file"), scribo::io::xml::Page);
  if (options.opt_value("more-xml-format") == "full")
    scribo::io::xml::save(doc, options.opt_value("more-xml-file"), scribo::io::xml::Full);

  if (scribo::debug::logger().is_enabled())
    scribo::io::img::save(doc, mln::debug::filename("regions.png"), scribo::io::img::DebugWoImage);
  if (options.opt_value("debug-regions") != "/dev/null")
    scribo::io::img::save(doc, options.opt_value("debug-regions"), scribo::io::img::DebugWithImage);

}
