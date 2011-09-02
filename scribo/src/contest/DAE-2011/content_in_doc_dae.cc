// Copyright (C) 2011 EPITA Research and Development Laboratory
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

#define SCRIBO_NOCR

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
  { "output_dir", "Output directory" },

  {0, 0}
};



int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 3)
    return scribo::debug::usage(argv,
				"Find paragraph segmentation and produces images for each paragraph.",
				"input.* output_dir",
				args_desc);

  trace::entering("main");

  Magick::InitializeMagick(*argv);

  typedef image2d<scribo::def::lbl_type> L;
  image2d<value::rgb8> input;
  mln::io::magick::load(input, argv[1]);

  // Preprocess document
  image2d<bool> input_preproc;
  input_preproc = toolchain::text_in_doc_preprocess(input, false, 0.34);

  bool denoise = true;
  std::string language = "";
  bool find_line_seps = true;
  bool find_whitespace_seps = true;

  // Run document toolchain.

  // Text
  std::cout << "Analysing document..." << std::endl;
  document<L>
    doc = scribo::toolchain::content_in_doc(input, input_preproc, denoise,
					    find_line_seps, find_whitespace_seps,
					    !language.empty(), language);

  const paragraph_set<L>& par_set = doc.paragraphs();
  image2d<bool> output(input.domain());
  for_all_paragraphs(p, par_set)
  {
    data::fill(output, true);
    const paragraph_info<L>& current_par = par_set(p);
    const mln::util::array<line_id_t>& line_ids = current_par.line_ids();
    const unsigned nelements = line_ids.nelements();

    for (unsigned i = 0; i < nelements; ++i)
    {
      const line_id_t& line_id = line_ids(i);
      const line_info<L>& current_line = par_set.lines()(line_id);

      scribo::draw::line_components(output, par_set.lines(), current_line, false);
    }

    std::stringstream ss;
    ss << argv[2] << "/" << basename(argv[1]) << "." << p << ".pbm";
    mln::io::pbm::save(output, ss.str());
  }

  trace::exiting("main");
}