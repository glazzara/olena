// Copyright (C) 2010, 2013 EPITA Research and Development Laboratory
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

#include <cctype>
#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/data/transform.hh>
#include <mln/fun/v2v/rgb_to_int_u.hh>

#include <scribo/binarization/sauvola_ms.hh>
#include <scribo/postprocessing/fill_object_holes.hh>
#include <scribo/preprocessing/denoise.hh>

int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;


  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " <input.{pgm,ppm}> <output.pbm>" << std::endl;
    std::cout << "--------------" << std::endl;
    std::cout << "EPITA/LRDE - 2010" << std::endl;
    return 1;
  }

  std::string ext = argv[1];
  ext = ext.substr(ext.length() - 3, 3);
  for (unsigned i = 0; i < ext.length(); ++i)
    ext[i] = tolower(ext[i]);

  // Load
  image2d<value::int_u8> input;

  if (ext == "pgm")
    io::pgm::load(input, argv[1]);
  else if (ext == "ppm")
  {
    image2d<value::rgb8> input_color;
    io::ppm::load(input_color, argv[1]);

    // Convert to Gray level image.
    input = data::transform(input_color, mln::fun::v2v::rgb_to_int_u<8>());
  }
  else
  {
    std::cout << "Invalid input file format! Expecting PGM or PPM images." << std::endl;
    return 1;
  }

  // Binarize
  image2d<bool>
    output = scribo::binarization::sauvola_ms(input, 101, 3, 0.4, 0.4, 0.4);

  output = scribo::preprocessing::denoise_bg(output, c8(), 4);
  output = scribo::postprocessing::fill_object_holes(output, 0.01f);

  io::pbm::save(output, argv[2]);
}


