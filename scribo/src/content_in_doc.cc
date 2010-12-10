// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/io/pbm/save.hh>
#include <mln/io/magick/load.hh>

#include <mln/value/label_8.hh>

#include <mln/core/var.hh>

#include <mln/accu/count_value.hh>

#include <mln/draw/box_plain.hh>


#include <scribo/toolchain/text_in_doc.hh>
#include <scribo/toolchain/text_in_doc_preprocess.hh>

#include <scribo/core/document.hh>
#include <scribo/core/line_set.hh>

#include <scribo/debug/usage.hh>

#include <scribo/make/debug_filename.hh>

#include <scribo/primitive/extract/elements.hh>

#include <scribo/preprocessing/crop_without_localization.hh>
#include <scribo/preprocessing/crop.hh>

#include <scribo/io/xml/save.hh>
#include <scribo/io/text_boxes/save.hh>



const char *args_desc[][2] =
{
  { "input.*", "An image." },
  { "out.xml", "Result of the document analysis." },
  { "denoise_enabled", "1 enables denoising, 0 disables it. (enabled by default)" },
  { "pmin_row", "Row index of the top left corner of the Region of interest." },
  { "pmin_col", "Col index of the top left corner of the Region of interest." },
  { "pmax_row", "Row index of the bottom right corner of the Region of interest." },
  { "pmax_col", "Col index of the bottom right corner of the Region of interest." },
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

  if (argc < 3 || (argc > 8 &&  argc != 12))
    return scribo::debug::usage(argv,
				"Find text lines and elements in a document",
				"input.* out.xml <denoise_enabled> [<pmin_row> <pmin_col> <pmax_row> <pmax_col>] <find_lines> <find_whitespaces> <K> <debug_dir>",
				args_desc);

  bool debug = false;

  // Enable debug output.
  if (argc == 8 || argc == 12)
  {
    scribo::make::internal::debug_filename_prefix = argv[argc - 1];
    debug = true;
  }

  trace::entering("main");

  Magick::InitializeMagick(*argv);

  typedef image2d<scribo::def::lbl_type> L;
  scribo::document<L> doc(argv[1]);
  doc.open();

  // Preprocess document
  image2d<bool> input;
  {
    double K = 0.34;
    if (argc == 7  || argc == 8 || argc == 11)
    {
      if (argc == 7)
	K = atof(argv[6]);
      else
	K = atof(argv[argc - 2]);
      std::cout << "Using K = " << K << std::endl;
    }

    image2d<bool> tmp_fg;
    input = toolchain::text_in_doc_preprocess(doc.image(), false, K);
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
    input = preprocessing::crop_without_localization(input, roi);
    crop_shift = point2d(minr, minc);

    if (debug)
      mln::io::pbm::save(input,
			 scribo::make::debug_filename("input_cropped.pbm"));
  }

  bool denoise = (argc > 3 && atoi(argv[3]) != 0);

  bool find_line_seps = true;
  if (argc >= 5 && argc < 12)
    find_line_seps = (atoi(argv[4]) != 0);

  bool find_whitespace_seps = true;
  if (argc >= 6 && argc < 12)
    find_line_seps = (atoi(argv[5]) != 0);

  std::cout << "Running with the following options :"
	    << "find_lines_seps = " << find_line_seps
	    << " | find_whitespace_seps = " << find_whitespace_seps
	    << " | debug = " << debug
	    << std::endl;

  // Run document toolchain.

  // Text
  std::cout << "Extracting text" << std::endl;
  line_set<L>
    lines = scribo::toolchain::text_in_doc(input, denoise, find_line_seps,
					   find_whitespace_seps, debug);
  doc.set_text(lines);

  // Elements
  std::cout << "Extracting Elements" << std::endl;
  component_set<L> elements = scribo::primitive::extract::elements(doc, input);
  doc.set_elements(elements);


  // Saving results
  scribo::io::xml::save(doc, argv[2], true);

  trace::exiting("main");
}
