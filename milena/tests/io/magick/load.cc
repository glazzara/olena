// Copyright (C) 2009, 2010, 2013 EPITA Research and Development
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

#include <mln/io/magick/load.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/load.hh>

#include <mln/data/compare.hh>

#include <mln/core/image/image2d.hh>

#include <mln/value/rgb8.hh>

#include "tests/data.hh"


int main(int /* argc */, char* argv[])
{
  using namespace mln;

  // Compare Milena's built-in PBM loaded and Magick++'s support for PBM.
  {
    typedef image2d<bool> I;
    I mln_lena;
    io::pbm::load(mln_lena, MLN_IMG_DIR "/tiny.pbm");
    I magick_lena;
    io::magick::load(magick_lena, MLN_IMG_DIR "/tiny.pbm");
    mln_assertion(mln_lena == magick_lena);
  }

  // Compare Milena's built-in PGM loaded and Magick++'s support for PGM.
  {
    typedef image2d<value::int_u8> I;
    I mln_lena;
    io::pgm::load(mln_lena, MLN_IMG_DIR "/tiny.pgm");
    I magick_lena;
    io::magick::load(magick_lena, MLN_IMG_DIR "/tiny.pgm");
    mln_assertion(mln_lena == magick_lena);
  }

  // Compare Milena's built-in PPM loaded and Magick++'s support for PPM.
  {
    typedef image2d<value::rgb8> I;
    I mln_lena;
    io::ppm::load(mln_lena, MLN_IMG_DIR "/tiny.ppm");
    I magick_lena;
    io::magick::load(magick_lena, MLN_IMG_DIR "/tiny.ppm");
    mln_assertion(mln_lena == magick_lena);
  }


  // Check Magick++'s support for PNG.
  {
    typedef image2d<value::rgb8> I;
    I lena_ppm;
    io::ppm::load(lena_ppm, MLN_IMG_DIR "/tiny.ppm");
    I lena_png;
    io::magick::load(lena_png, MLN_TESTS_IMG_DIR "/tiny.png");
    mln_assertion(lena_ppm == lena_png);
  }
}
