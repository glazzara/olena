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
#include <iostream>

#include <mln/core/image/image2d.hh>

#include <mln/io/magick/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/value/rgb8.hh>

#include <scribo/debug/usage.hh>

#include <scribo/toolchain/text_in_doc_preprocess.hh>


const char *args_desc[][2] =
{
  { "input.*", "An image." },
  { "output.pbm", "A text file with all the recognized text" },
  { "enable fg/bg", "If set to 1 enables foreground extraction. (disabled by default)" },
  { "lambda", "Lambda used in remove fg/bg (Automaticaly deduced by default)." },
  {0, 0}
};


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 3 && argc != 4 && argc != 5)
    return scribo::debug::usage(argv,
				"Find text in a color document.",
				"input.* output.pbm <enable fg/bg> <lambda>",
				args_desc);

  image2d<value::rgb8> input_rgb;
  io::magick::load(input_rgb, argv[1]);

  unsigned lambda;
  if (argc == 5)
    lambda = atoi(argv[4]);

  bool remove_bg = false;
  if (argc >= 4)
    remove_bg = (atoi(argv[3]) == 1);

  image2d<bool> output;

  if (argc == 5 && remove_bg)
    output = toolchain::text_in_doc_preprocess(input_rgb, lambda);
  else
    output = toolchain::text_in_doc_preprocess(input_rgb, remove_bg);

  mln::io::pbm::save(output, argv[2]);
}
