//                                                                  -*- c++ -*-
// Copyright (C) 2004, 2012 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#include <mln/transform/fft.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/opt/at.hh>
#include <mln/debug/println.hh>

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
  //  debug::println(fourier.transformed_image());
  io::pgm::save(fft, "fft.pgm");

  std::cout << "Test: Image == F-1(F(Image)) ... " << std::flush;
  // CHECK (im1 == im3);

  image2d<int_u8> out = fourier.transformed_image_clipped_magn<int_u8>(0.01);

  io::pgm::save(out, "fft_trans_clipped.pgm");

  out = fourier.transformed_image_log_magn<int_u8>(1, 100);

  io::pgm::save(out, "fft_trans_log.pgm");

  for (int row = 40; row < im2.nrows() - 40; ++row)
    for (int col = 0; col < im2.ncols(); ++col)
      opt::at(im2, row, col) = 0;

  for (int row = 0; row < im2.nrows(); ++row)
    for (int col = 40; col < im2.ncols() - 40; ++col)
      opt::at(im2, row, col) = 0;

  out = fourier.transform_inv<int_u8>();

  io::pgm::save(out, "fft_low_pass.pgm");

  return status;
}
