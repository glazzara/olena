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


const char *args_desc[][2] =
{
  { "input.*", "An image." },
  { "out.xml", "Result of the document analysis." },
  { "denoise_enabled", "1 enables denoising, 0 disables it. (enabled by default)" },
  { "pmin_row", "Row index of the top left corner of the Region of interest." },
  { "pmin_col", "Col index of the top left corner of the Region of interest." },
  { "pmax_row", "Row index of the bottom right corner of the Region of interest." },
  { "pmax_col", "Col index of the bottom right corner of the Region of interest." },
  { "language", "Language to be used for the text recognition. [eng|fra] (Default: eng)" },
  { "find_lines", "Find vertical lines. (Default 1)" },
  { "find_whitespaces", "Find whitespaces separators. (Default 1)" },
  { "K", "Sauvola's binarization threshold parameter. (Default: 0.34)" },
  { "debug_dir", "Output directory for debug image" },
  {0, 0}
};



int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc < 3 || argc > 14)
    return scribo::debug::usage(argv,
				"Find text lines and elements in a document",
				"input.* out.xml <denoise_enabled> [<pmin_row> <pmin_col> <pmax_row> <pmax_col>] [language] [find_lines] [find_whitespaces] [K] [debug_dir]",
				args_desc);

  // Enable debug output.
  if (argc == 9 || argc == 13)
  {
    scribo::debug::logger().set_filename_prefix(argv[argc - 1]);
    scribo::debug::logger().set_level(scribo::debug::All);
    scribo::make::internal::debug_filename_prefix = argv[argc - 1];
  }

  trace::entering("main");

  Magick::InitializeMagick(*argv);

  typedef image2d<scribo::def::lbl_type> L;
  image2d<value::rgb8> input;
  mln::io::magick::load(input, argv[1]);

  // Preprocess document
  image2d<bool> input_preproc;
  {
    double K = 0.34;
    if (argc == 8  || argc >= 12)
    {
      if (argc == 8)
	K = atof(argv[7]);
      else
	K = atof(argv[argc - 2]);
      std::cout << "Using K = " << K << std::endl;
    }

    input_preproc = toolchain::text_in_doc_preprocess(input, false, K);
  }

  // Optional Cropping
  point2d crop_shift = literal::origin;
  if (argc >= 12)
  {
    mln::def::coord
      minr = atoi(argv[4]),
      minc = atoi(argv[5]),
      maxr = atoi(argv[6]),
      maxc = atoi(argv[7]);

    std::cout << "> Image cropped from (" << minr << "," << minc << ")"
	      << " to (" << maxr << "," << maxc << ")" << std::endl;

    box2d roi = mln::make::box2d(minr, minc, maxr, maxc);
    input_preproc = preprocessing::crop_without_localization(input_preproc, roi);
    crop_shift = point2d(minr, minc);

    scribo::debug::logger().log_image(scribo::debug::Results, input_preproc,
				      "input_preproc_cropped.pbm");
  }

  bool denoise = (argc > 3 && atoi(argv[3]) != 0);

  std::string language = "eng";
  if (argc >= 5 && argc < 13)
    language = argv[4];
  else if (argc >= 12)
    language = argv[8];

  bool find_line_seps = true;
  if (argc >= 6 && argc < 13)
    find_line_seps = (atoi(argv[5]) != 0);
  else if (argc >= 12)
    find_line_seps = (atoi(argv[9]) != 0);

  bool find_whitespace_seps = true;
  if (argc >= 7 && argc < 13)
    find_whitespace_seps = (atoi(argv[6]) != 0);
  else if (argc >= 12)
    find_whitespace_seps = (atoi(argv[10]) != 0);

  std::cout << "Running with the following options :"
	    << " ocr_language = " << language
	    << " | find_lines_seps = " << find_line_seps
	    << " | find_whitespace_seps = " << find_whitespace_seps
	    << " | debug = " << scribo::debug::logger().is_enabled()
	    << std::endl;

  // Run document toolchain.

  // Text
  std::cout << "Analysing document..." << std::endl;
  document<L>
    doc = scribo::toolchain::content_in_doc(input, input_preproc, denoise,
					    find_line_seps, find_whitespace_seps,
					    !language.empty(), language);

  // Saving results
  std::cout << "Saving results..." << std::endl;
  scribo::io::xml::save(doc, argv[2], scribo::io::xml::PageExtended);
  scribo::io::xml::save(doc, "page.xml", scribo::io::xml::Page);
  scribo::io::xml::save(doc, "full.xml", scribo::io::xml::Full);

  trace::exiting("main");
}
