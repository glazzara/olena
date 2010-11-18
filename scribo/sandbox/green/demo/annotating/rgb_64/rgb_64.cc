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
/// \brief Implement the Millet RGB-64 descriptor [millet.phd.2008.pdf]
///
/// This is an image descriptor. It works by making 2 bits
/// quantification on each channel. It result a processing in RGB-64
/// space. Then we build the color histogram.

#include <iostream>

#include <mln/accu/stat/histo3d_rgb.hh>

#include <mln/core/image/image2d.hh>

#include <mln/data/compute.hh>
#include <mln/data/transform.hh>

#include <mln/debug/println.hh>

#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/img_path.hh>

#include <mln/io/ppm/load.hh>

#include <mln/value/rgb.hh>
#include <mln/value/rgb8.hh>

/// \brief Main entry.
///
/// Loading, Quantifiying in two bits each channel, then building
/// color histogram.
int main()
{
  typedef mln::fun::v2v::rgb8_to_rgbn<2>     t_rgb8_to_rgb2;
  typedef mln::value::rgb8                   t_rgb8;
  typedef mln::value::rgb<2>                 t_rgb2;
  typedef mln::image2d<t_rgb8>               t_image2d_rgb8;
  typedef mln::image2d<t_rgb2>               t_image2d_rgb2;
  typedef mln::image3d<unsigned>             t_histo3d;
  typedef mln::accu::meta::stat::histo3d_rgb t_histo3d_fun;

  t_image2d_rgb8                             input_rgb8;
  t_image2d_rgb2                             input_rgb2;
  t_histo3d                                  histo;

  // IMAGE LOADING PHASE
  std::cout << "Image loading phase ..." << std::endl;
  mln::io::ppm::load(input_rgb8, ICDAR_50P_PPM_IMG_PATH"/mp00082c_50p.ppm");
  input_rgb2 = mln::data::transform(input_rgb8, t_rgb8_to_rgb2());
  histo      = mln::data::compute(t_histo3d_fun(), input_rgb2);

  // PRINTING PHASE
  mln::debug::println(histo);
}
