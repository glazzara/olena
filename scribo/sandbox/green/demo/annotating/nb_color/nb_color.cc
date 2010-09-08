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
/// \brief Compute the number of colors in an image.
///
/// Build histogram of colors and count the bins different from zero.

#include <iostream>
#include <sstream>

#include <mln/img_path.hh>

#include <mln/accu/math/sum.hh>
#include <mln/accu/math/count.hh>
#include <mln/accu/stat/histo3d_rgb.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/accu/stat/variance.hh>

#include <mln/algebra/vec.hh>

#include <mln/arith/diff_abs.hh>

#include <mln/core/macros.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/routine/initialize.hh>

#include <mln/data/compute.hh>
#include <mln/data/fill.hh>
#include <mln/data/transform.hh>

#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/labeling/regional_maxima.hh>
#include <mln/labeling/mean_values.hh>
#include <mln/labeling/compute.hh>

#include <mln/literal/colors.hh>

#include <mln/morpho/opening/volume.hh>
#include <mln/morpho/elementary/dilation.hh>

#include <mln/opt/at.hh>

#include <mln/pw/cst.hh>

#include <mln/util/array.hh>
#include <mln/util/timer.hh>

#include <mln/value/label_8.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/rgb.hh>
#include <mln/value/int_u.hh>

/// \brief Count the colors.
///
/// \param[in] image the name of the image to process.
///
/// \return the number of colors.
///
/// Count the color by building histogram. Quantification is done to
/// reduce the size of the histogram.
///
/// \fixme: Strange compilation warning, I don't know how to solve it!

// n < 8, n is the degree of quantification
template <unsigned n>
unsigned count_image_color(const std::string& image)
{
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::rgb<n>                          t_rgbn;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_rgbn>                        t_image2d_rgbn;
  typedef mln::image3d<unsigned>                      t_histo3d;
  typedef mln::fun::v2v::rgb8_to_rgbn<n>              t_rgb8_to_rgbn;
  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;
  typedef mln::accu::meta::math::count                t_count_fun;

  t_image2d_rgb8                                      input_rgb8;
  t_image2d_rgbn                                      input_rgbn;
  t_image2d_rgbn                                      output_rgbn;
  t_histo3d                                           histo;
  t_histo3d                                           opened;

  mln::io::ppm::load(input_rgb8, image.c_str());

//  unsigned nb_pixel   = input_rgb8.ncols() * input_rgb8.nrows();
//   unsigned min_volume = (unsigned)(nb_pixel * 0.054);
  unsigned nb_color   = 0;

  input_rgbn = mln::data::transform(input_rgb8, t_rgb8_to_rgbn());
  histo      = mln::data::compute(t_histo3d_fun(), input_rgbn);
  nb_color   = mln::data::compute(t_count_fun(),
				  (histo | (mln::pw::value(histo) != 0)).rw());

  return nb_color;
}


/// \brief Main entry.
///
/// Print the number of colors.
int main()
{
  unsigned val=count_image_color<8>(ANNOTATING_1_PHOTO_IMG_PATH "/photo01.ppm");

  std::cout << "nb color : " << val << std::endl;

  return 0;
}

