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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <libgen.h>
#include <fstream>
#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/pbm/all.hh>


#include <scribo/toolchain/text_in_doc.hh>

#include <scribo/core/document.hh>
#include <scribo/core/line_set.hh>

#include <scribo/debug/option_parser.hh>

#include <scribo/preprocessing/crop_without_localization.hh>

#include <scribo/io/text_boxes/save.hh>


static const scribo::debug::arg_data arg_desc[] =
{
  { "input.pbm", "A binary image." },
  { "output.txt", "Text output." },
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
    "According to your system, you can choose between eng (default), "
    "fra, deu, ita, nld, por, spa, vie",
    "<lang>", scribo::debug::check_ocr_lang, 1, "eng" },
  { "verbose", "Enable verbose mode", 0, 0, 0, 0 },
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
    scribo::make::internal::debug_filename_prefix = options.opt_value("debug-prefix").c_str();
  }

  bool verbose = options.is_set("verbose");

  trace::entering("main");

  typedef image2d<scribo::def::lbl_type> L;

  image2d<bool> input;
  const char *input_name = options.arg("input.pbm");
  mln::io::pbm::load(input, input_name);

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
    input = preprocessing::crop_without_localization(input, roi);
    crop_shift = point2d(minr, minc);

    scribo::debug::logger().log_image(scribo::debug::Results, input,
				      "input_cropped.pbm");
  }

  bool denoise = options.is_enabled("denoising");
  std::string language = options.opt_value("ocr-lang");
  bool find_line_seps = options.is_enabled("find-seps");
  bool find_whitespace_seps = options.is_enabled("find-delims");

  if (verbose)
    std::cout << "Running with the following options :"
	      << " ocr language = " << language
	      << " | find_lines_seps = " << find_line_seps
	      << " | find_whitespace_seps = " << find_whitespace_seps
	      << " | debug = " << scribo::debug::logger().is_enabled()
	      << std::endl;

  // Run document toolchain.
  line_set<L>
    lines = scribo::toolchain::text_in_doc(input, denoise,
					   language, find_line_seps,
					   find_whitespace_seps, verbose);

  scribo::document<L> doc(input_name);

  // Specify shift due to potential previous crop.
  scribo::io::text_boxes::save(lines, options.arg("output.txt"), crop_shift);

  trace::exiting("main");
}
