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
/// This demonstrator is aimed to experiment the regional maxima
/// method.  regional_maxima.It first test the 3d histogram from an
/// image and the number of colors which results. And in fact, doesn't
/// use the regional_maxima routine. Too many parameters must be fixed
/// and parts of the code has been commented. The last action of this
/// program consists in counting the number of colors of each image in a
/// directory. This source has been used to test the annotating image base.
///
/// \fixme At this time, the source is in a transitional step. It doesn't
/// compile, something wrong with boost library. It doesn't matter, that's not
/// an important file.


#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

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


/// \brief This is the count method.
///
/// \param[in] image the colored image.
///
/// This function loads a color image (ppm) and count the colors after
/// using the min volume processing on the 3d histogram. Percentage
/// thresholding is in test. The file threshold.txt establishes the link
/// between percentage and size in image.
///
// n < 8, n is the degree of quantification
template <unsigned n>
unsigned count_image_color(const std::string& image)
{
  typedef mln::value::label_8                         t_lbl8;
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::rgb<n>                          t_rgbn;
  typedef mln::image3d<t_lbl8>                        t_image3d_lbl8;
  typedef mln::image2d<t_lbl8>                        t_image2d_lbl8;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_rgbn>                        t_image2d_rgbn;
  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
  typedef mln::image3d<unsigned>                      t_histo3d;
  typedef mln::image2d<unsigned>                      t_histo2d;
  typedef mln::fun::v2v::rgb8_to_rgbn<n>              t_rgb8_to_rgbn;
  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;
  typedef mln::accu::meta::math::count                t_count_fun;

  t_image2d_rgb8                                      input_rgb8;
  t_image2d_rgbn                                      input_rgbn;
  t_image2d_rgbn                                      output_rgbn;
  t_histo3d                                           histo;
  t_histo3d                                           opened;
  t_image3d_lbl8                                      label;
  t_image2d_lbl8                                      label_img;
  t_image3d_lbl8                                      dilated;
  t_lbl8                                              n_labels;

  mln::io::ppm::load(input_rgb8, image.c_str());

  unsigned nb_pixel   = input_rgb8.ncols() * input_rgb8.nrows();
  unsigned min_volume = (unsigned)(nb_pixel * 0.054);

  input_rgbn = mln::data::transform(input_rgb8, t_rgb8_to_rgbn());
  histo      = mln::data::compute(t_histo3d_fun(), input_rgbn);
  return mln::data::compute(t_count_fun(), histo);

  // return nb_pixel;
  // opened     = mln::morpho::opening::volume(histo, mln::c6(), min_volume);
  // label      = mln::labeling::regional_maxima(opened, mln::c6(), n_labels);
  // return n_labels;
}

/// \brief The main function aimed at looping over the whole directory.
///
/// The main function is used to take care of file part in this
/// sofware.  It manages the directory scanning aspect while the last
/// function does the image processing aspect. Statistics are
/// generated (mean and variance) about the number of colors after using
/// the min_volume routine. Some strange things happens with that routine
/// sometimes.
int main()
{
  typedef boost::filesystem::path                   t_path;
  //typedef boost::filesystem::initial_path<t_path()> t_init_path;
  typedef boost::filesystem::directory_iterator     t_iter_path;

  t_path full_path(ANNOTATING_1_TYPED_IMG_PATH);

  std::cout << "entering "   << full_path << std::endl;

  if (//1 < argc &&
      boost::filesystem::exists(full_path) &&
      boost::filesystem::is_directory(full_path))
  {
    boost::filesystem::system_complete(full_path);
    const t_iter_path end_iter;
    unsigned count = 0;
    unsigned sum1  = 0;
    unsigned sum2  = 0;

    for (t_iter_path dir_iter(full_path); end_iter != dir_iter; ++dir_iter)
    {
      unsigned val = count_image_color<5>(dir_iter->path().string());

      ++count;
      sum1 += val;
      sum2 += val*val;

      std::cout << dir_iter->path().string() << " => " << val << std::endl;
    }

    unsigned mean = sum1 / count;
    unsigned var  = sum2 / count - mean * mean;

    std::cout << "mean : " << mean << std::endl;
    std::cout << "var  : " << var  << std::endl;
  }

  return 0;
}

