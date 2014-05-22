// Copyright (C) 2004, 2012, 2014 EPITA Research and Development Laboratory.
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

#include <mln/transform/fft.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/opt/at.hh>
#include <mln/debug/println.hh>

#include <mln/core/image/flat_image.hh>
#include <mln/fun/vv2b/le.hh>
#include <mln/fun/vv2v/diff_abs.hh>
#include <mln/data/transform.hh>
#include <mln/test/predicate.hh>

#include "tests/data.hh"

#define CHECK(Condition)			\
    if (Condition)				\
      std::cout << "OK" << std::endl;		\
    else					\
      {						\
	std::cout << "FAIL" << std::endl;	\
	status = 1;				\
      }

using namespace mln;
using namespace transform;
using namespace value;

int main ()
{
  int status = 0;

  image2d<int_u8> im1;
  io::pgm::load(im1, MLN_IMG_DIR "/lena.pgm");

  fft<double> fourier(im1);

  image2d< std::complex<double> > im2 = fourier.transform();

  image2d<int_u8> im3 = fourier.transform_inv<int_u8>();

  io::pgm::save(im3, "fft_copy.pgm");

  image2d<int_u8> fft = fourier.transformed_image_log_magn<int_u8>(true);
  io::pgm::save(fft, "fft.pgm");

  std::cout << "Test: Image == F-1(F(Image)) ... " << std::flush;
  /* FIXME: Milena lacks some feature to make write the following in a
     shorter fashion (fun-morpher accepting binary (vv2v) functions or
     function composition in pw-functions.  */
  CHECK(test::predicate(data::transform(im1, im3,
					fun::vv2v::diff_abs<int_u8>()),
			flat_image<int_u8, box2d>(1, im1.domain()),
			fun::vv2b::le<int_u8>()));

  image2d<int_u8> out = fourier.transformed_image_clipped_magn<int_u8>(0.01);

  io::pgm::save(out, "fft_trans_clipped.pgm");

  out = fourier.transformed_image_log_magn<int_u8>(1, 100);

  io::pgm::save(out, "fft_trans_log.pgm");

  int nrows = im2.nrows();
  int ncols = im2.ncols();
  int hlen = 40;
  for (int row = hlen; row < nrows - hlen; ++row)
    for (int col = 0; col < ncols; ++col)
      opt::at(im2, row, col) = 0;

  for (int row = 0; row < nrows; ++row)
    for (int col = hlen; col < ncols - hlen; ++col)
      opt::at(im2, row, col) = 0;

  fft = fourier.transformed_image_log_magn<int_u8>(true);
  io::pgm::save(fft, "fft_trans_cropped.pgm");

  out = fourier.transform_inv<int_u8>();

  io::pgm::save(out, "fft_low_pass.pgm");

  return status;
}
