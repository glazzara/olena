// Copyright (C) 2011, 2012 EPITA Research and Development Laboratory
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

#include <scribo/preprocessing/crop_without_localization.hh>
#include <scribo/preprocessing/crop.hh>

#include <scribo/io/xml/save.hh>
#include <scribo/io/img/save.hh>


const char *args_desc[][2] =
{
  { "input.*", "An image." },
  { "non_text_comps.pbm", "Non text components mask." },
  { "enable_debug", "Enable debug image output. Set to 1 or 0." },
  { "enable_tabstops", "Enable tabstops detection. Set to 1 or 0." },
  {0, 0}
};



int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 4 && argc != 3 && argc != 5)
    return scribo::debug::usage(argv,
				"Extract non text components mask/",
				"input.* non_text_comps.pbm [enable_debug] [enable_tabstops]",
				args_desc);

  std::string out_img = basename(argv[1]);
  out_img.erase(out_img.size() - 4);

  std::string filename_prefix = out_img + "_debug";
  scribo::debug::logger().set_filename_prefix(filename_prefix.c_str());
  if (argc > 3 && atoi(argv[3]))
    scribo::debug::logger().set_level(scribo::debug::Special);
  else
    scribo::debug::logger().set_level(scribo::debug::None);

  trace::entering("main");

  Magick::InitializeMagick(*argv);

  typedef image2d<scribo::def::lbl_type> L;
  image2d<value::rgb8> input;
  mln::io::magick::load(input, argv[1]);

  mln::util::timer t;
  t.start();

  // Preprocess document
  image2d<bool>
    input_preproc = toolchain::text_in_doc_preprocess(input, false, 0, 0.34,
						      false, false);


  bool denoise = true;
  std::string language = "";
  bool find_line_seps = true;
  bool find_whitespace_seps = (argc > 4 && atoi(argv[4]));

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
  t.stop();
  std::cout << t << std::endl;

  mln::io::pbm::save(data::convert(bool(), doc.elements().labeled_image()), argv[2]);

  scribo::io::img::save(doc, out_img + "_debug_wo_image.png", scribo::io::img::DebugWoImage);
  scribo::io::img::save(doc, out_img + "_debug_with_image.png", scribo::io::img::DebugWithImage);

  trace::exiting("main");
}
