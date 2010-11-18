// Copyright (C) 2007, 2008, 2009, 2010 EPITA LRDE
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

/// \file
///
/// \brief Implement the Millet LEP descriptor [millet.phd.2008.pdf]
///
/// This is an image descriptor. It works on grey level image. First,
/// Sobel is applied on the image. Then a specific window is build to
/// be convolved on the image. The aim of the convolution is to mark
/// the configuration of the neighbouring for this thresholded image.
/// Histogram of the configuration is then created and that's all.

#include <iostream>

#include <mln/accu/stat/histo1d.hh>

#include <mln/binarization/threshold.hh>

#include <mln/core/alias/w_window2d_int.hh>
#include <mln/core/image/image2d.hh>

#include <mln/data/compute.hh>
#include <mln/data/convert.hh>
#include <mln/data/transform.hh>

#include <mln/debug/println.hh>

#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/img_path.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/plot/save_image_sh.hh>

#include <mln/linear/ch_convolve.hh>
#include <mln/linear/convolve.hh>
#include <mln/linear/sobel_2d.hh>

#include <mln/make/w_window2d.hh>

#include <mln/value/rgb.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/int_u8.hh>

#include <mln/core/var.hh>

/// \brief Main entry.
///
/// Load the pgm image. Threshold it. Identifie the pixel
/// configuration with specific convolution filter. Build 1d histogram.

int main()
{
  using namespace mln;

  typedef mln::w_window2d_int                t_win2d;
  typedef mln::value::int_u8                 t_int_u8;
  typedef mln::value::int_u<9>               t_int_u9;
  typedef mln::image2d<t_int_u8>             t_image2d_int_u8;
  typedef mln::image2d<t_int_u9>             t_image2d_int_u9;
  typedef mln::image2d<int>                  t_image2d_int;
  typedef mln::image2d<unsigned>             t_image2d_unsigned;
  typedef mln::image2d<float>                t_image2d_float;
  typedef mln::image2d<double>               t_image2d_double;
  typedef mln::image2d<bool>                 t_image2d_bool;
  typedef mln::image1d<unsigned>             t_histo1d;
  typedef mln::accu::meta::stat::histo1d     t_histo1d_fun;

  t_image2d_int_u8                           input_int_u8;
  t_image2d_int                              input_int;
  t_image2d_unsigned                         input_unsigned;
  t_image2d_float                            sobel;
  t_image2d_bool                             thresh_bool;
  t_image2d_int_u8                           thresh_int_u8;
  t_image2d_float                            conf_float;
  t_image2d_int_u9                           conf_int_u9;
  t_histo1d                                  histo;

  // IMAGE LOADING PHASE
  std::cout << "Image loading phase ..." << std::endl;
  mln::io::pgm::load(input_int_u8,ICDAR_50P_PGM_IMG_PATH"/mp00082c_50p.pgm");

  sobel = mln::linear::sobel_2d_l1_norm(input_int_u8);

  //mln::io::plot::save_image_sh(sobel, "sobel.sh");
  thresh_bool   = mln::binarization::threshold(sobel, 100);
  thresh_int_u8 = mln::data::convert(mln::value::int_u8(), thresh_bool);
  //mln::io::plot::save_image_sh(thresh, "thresh.sh");

  int ws[] = { 1, 2, 4,
	       8, 256, 16,
	       32, 64, 128 };

  t_win2d win2d = mln::make::w_window2d(ws);
  conf_float    = mln::linear::convolve(thresh_int_u8, win2d);
  conf_int_u9   = mln::data::convert(t_int_u9(), conf_float);
  histo         = mln::data::compute(t_histo1d_fun(), conf_int_u9);

  mln::io::plot::save_image_sh(histo, "histo.sh");

  // PRINTING PHASE
  mln::debug::println(histo);
}
