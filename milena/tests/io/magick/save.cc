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

#include <mln/core/image/image2d.hh>

#include <mln/data/compare.hh>

#include <mln/io/magick/load.hh>
#include <mln/io/magick/save.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include "tests/data.hh"


using namespace mln;


template <typename T>
image2d<T>
test(const image2d<T>& lena_mln, const std::string& temp_filename)
{
  point2d p(0,0);

  T c = lena_mln(p);

  io::magick::save(lena_mln, temp_filename);
  image2d<T> lena_im;
  io::magick::load(lena_im, temp_filename);

  mln_assertion(lena_im(p) == c);
  mln_assertion(lena_im.domain() == lena_mln.domain());
  mln_assertion(lena_im == lena_mln);

  return lena_im;
}


int main()
{
  using namespace mln;

  point2d p(0,0);

  // Grayscale values (PBM -> PBM -> PBM).
  {
    typedef image2d<bool> I;
    I lena_mln;
    io::pbm::load(lena_mln, MLN_IMG_DIR "/tiny.pbm");
    I lena_im = ::test(lena_mln, "save-tiny-temp.pbm");
    io::pbm::save(lena_im, "save-tiny.pbm");
  }

  // Grayscale values (PGM -> PGM -> PGM).
  {
    typedef image2d<value::int_u8> I;
    I lena_mln;
    io::pgm::load(lena_mln, MLN_IMG_DIR "/tiny.pgm");
    I lena_im = ::test(lena_mln, "save-tiny-temp.pgm");
    io::pgm::save(lena_im, "save-tiny.pgm");
  }

  // Color values (PPM -> PNG -> PPM).
  {
    typedef image2d<value::rgb8> I;
    I lena_mln;
    io::ppm::load(lena_mln, MLN_IMG_DIR "/tiny.ppm");
    I lena_im = ::test(lena_mln, "save-tiny-temp.png");
    io::ppm::save(lena_im, "save-tiny.ppm");
  }
}
