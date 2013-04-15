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


# include <iostream>
# include <fstream>

# include <mln/core/image/image2d.hh>
# include <mln/util/array.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/make/box2d.hh>
# include <mln/io/ppm/save.hh>
# include <mln/io/magick/load.hh>
# include <mln/value/rgb8.hh>

# include <scribo/debug/highlight_text_area.hh>
# include <scribo/debug/usage.hh>

const char *args_desc[][2] =
{
  { "input.*", "An image." },
  { "ocr_text.txt", "OCR result which must contain text boxes coordinates." },
  { "output.ppm", "A color image with highlighted text areas." },
  {0, 0}
};


int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 4)
    return scribo::debug::usage(argv,
				"Highlight text areas in an image.",
				"input.* ocr_text.txt output.ppm",
				args_desc);

  mln_trace("main");

  image2d<value::rgb8> input;
  io::magick::load(input, argv[1]);


  mln::util::array<box2d> bbox;

  std::ifstream file(argv[2]);
  if (! file)
  {
    std::cerr << "error: file '" << argv[2]
	      << "' not found!";
    abort();
  }


  while (! file.eof())
  {
    int
      pmin_row, pmin_col,
      pmax_row, pmax_col;

    file >> pmin_row;
    file >> pmin_col;
    file >> pmax_row;
    file >> pmax_col;

    std::string txt;
    getline (file, txt);

    bbox.append(mln::make::box2d(pmin_row, pmin_col, pmax_row, pmax_col));
  }

  io::ppm::save(scribo::debug::highlight_text_area(input, bbox), argv[3]);

}
