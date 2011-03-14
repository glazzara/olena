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

#include <scribo/debug/usage.hh>

#include <scribo/preprocessing/crop_without_localization.hh>

#include <scribo/io/text_boxes/save.hh>




const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'False' for object, 'True'\
for the background." },
  { "out.txt", "Text output" },
  { "denoise_enabled", "1 enables denoising, 0 disables it. (enabled by default)" },
  { "pmin_row", "Row index of the top left corner of the Region of interest." },
  { "pmin_col", "Col index of the top left corner of the Region of interest." },
  { "pmax_row", "Row index of the bottom right corner of the Region of interest." },
  { "pmax_col", "Col index of the bottom right corner of the Region of interest." },
  { "language", "Language to be used for the text recognition. [eng|fra] (Default: eng)" },
  { "find_lines", "Find vertical lines. (Default 1)" },
  { "find_whitespaces", "Find whitespaces separators. (Default 1)" },
  { "debug_dir", "Output directory for debug image" },
  {0, 0}
};


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc < 3 || argc > 12)
    return scribo::debug::usage(argv,
				"Find text lines using left/right validation and display x-height in a binarized article.",
				"input.pbm out.txt <denoise_enabled> [<pmin_row> <pmin_col> <pmax_row> <pmax_col>] <language> <find_lines> <find_whitespaces> <debug_dir>",
				args_desc);

  // Enable debug output.
  if (argc == 8 || argc == 12)
  {
    scribo::debug::logger().set_filename_prefix(argv[argc - 1]);
    scribo::debug::logger().set_level(scribo::debug::All);
    scribo::make::internal::debug_filename_prefix = argv[argc - 1];
  }

  trace::entering("main");

  typedef image2d<scribo::def::lbl_type> L;

  image2d<bool> input;
  mln::io::pbm::load(input, argv[1]);

  // Optional Cropping
  point2d crop_shift = literal::origin;
  if (argc >= 9)
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

    scribo::debug::logger().log_image(scribo::debug::Results, input,
				      "input_cropped.pbm");
  }

  bool denoise = (argc > 3 && atoi(argv[3]) != 0);

  std::string language = "eng";
  if (argc > 4 && argc < 10)
    language = argv[4];
  else if (argc >= 9)
    language = argv[8];

  bool find_line_seps = true;
  if (argc > 5 && argc < 10)
    find_line_seps = (atoi(argv[5]) != 0);
  else if (argc >= 10)
    find_line_seps = (atoi(argv[9]) != 0);

  bool find_whitespace_seps = true;
  if (argc > 6 && argc < 10)
    find_whitespace_seps = (atoi(argv[6]) != 0);
  else if (argc >= 11)
    find_whitespace_seps = (atoi(argv[10]) != 0);


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
					   find_whitespace_seps);

  scribo::document<L> doc;
  doc.set_filename(argv[1]);

  // Specify shift due to potential previous crop.
  scribo::io::text_boxes::save(lines, argv[2], crop_shift);

  trace::exiting("main");
}
