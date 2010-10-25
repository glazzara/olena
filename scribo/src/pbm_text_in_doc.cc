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

#include <scribo/core/line_set.hh>

#include <scribo/debug/usage.hh>

#include <scribo/make/debug_filename.hh>

#include <scribo/preprocessing/crop_without_localization.hh>

#include <scribo/io/xml/save.hh>
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
				"input.pbm out.txt <denoise_enabled> [<pmin_row> <pmin_col> <pmax_row> <pmax_col>] <debug_dir>",
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
  point2d crop_shift = literal::origin;
  if (argc >= 8)
  {
    mln::def::coord
      minr = atoi(argv[4]),
      minc = atoi(argv[5]),
      maxr = atoi(argv[6]),
      maxc = atoi(argv[7]);

    box2d roi = mln::make::box2d(minr, minc, maxr, maxc);
    input = preprocessing::crop_without_localization(input, roi);
    crop_shift = point2d(minr, minc);

    if (debug)
      mln::io::pbm::save(input,
			 scribo::make::debug_filename("input_cropped.pbm"));
  }

  bool denoise = (argc > 3 && atoi(argv[3]) != 0);


  // Run document toolchain.
  typedef image2d<scribo::def::lbl_type> L;
  line_set<L>
    lines = scribo::toolchain::text_in_doc(input, denoise, debug);

  // Saving results
  scribo::io::xml::save(argv[1], lines, "out.xml", true);

  // Specify shift due to potential previous crop.
  scribo::io::text_boxes::save(lines, argv[2], crop_shift);

  trace::exiting("main");
}
