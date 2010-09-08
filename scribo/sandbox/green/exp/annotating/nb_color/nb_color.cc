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
#include <boost/filesystem.hpp>

#include <mln/img_path.hh>

#include <mln/accu/math/count.hh>
#include <mln/accu/stat/histo3d_rgb.hh>

#include <mln/binarization/threshold.hh>

#include <mln/core/alias/neighb3d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/compute.hh>
#include <mln/data/transform.hh>

#include <mln/fun/v2v/rgb8_to_rgbn.hh>

// #include <mln/morpho/opening/volume.hh>

#include <mln/io/ppm/load.hh>

#include <mln/pw/value.hh>
#include <mln/pw/cst.hh>

#include <mln/util/timer.hh>

#include <mln/value/rgb8.hh>
//#include <mln/value/rgb.hh>


/// \brief Count the colors.
///
/// \param[in] image the name of the image to process.
///
/// \return the number of colors.
///
/// Count the color by building histogram. Strange filtering is
/// done. We use also the technique of regional maxima in
/// comment. Quantification is done to reduce the size of the
/// histogram as well.

// n < 8, n is the degree of quantification
template <unsigned n>
unsigned count_image_color(const std::string& image)
{
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::rgb<n>                          t_rgbn;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_rgbn>                        t_image2d_rgbn;
  typedef mln::image3d<unsigned>                      t_histo3d;
  typedef mln::image3d<bool>                          t_histo3d_bool;
  typedef mln::fun::v2v::rgb8_to_rgbn<n>              t_rgb8_to_rgbn;
  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;
  typedef mln::accu::meta::math::count                t_count_fun;

  t_image2d_rgb8                                      input_rgb8;
  t_image2d_rgbn                                      input_rgbn;
  t_image2d_rgbn                                      output_rgbn;
  t_histo3d                                           histo;
//   t_histo3d                                           opened;
  t_histo3d_bool                                      filtered;

  mln::io::ppm::load(input_rgb8, image.c_str());

  unsigned nb_pixel   = input_rgb8.ncols() * input_rgb8.nrows();
  unsigned min_volume = (unsigned)(nb_pixel * 0.0001);
  unsigned nb_color   = 0;

//  input_rgbn = mln::data::transform(input_rgb8, t_rgb8_to_rgbn());
  histo      = mln::data::compute(t_histo3d_fun(), input_rgb8);
//   opened     = mln::morpho::opening::volume(histo, mln::c6(), min_volume);
  filtered   = mln::binarization::threshold(histo, min_volume);
  nb_color   = mln::data::compute(t_count_fun(),
				 (filtered|(mln::pw::value(filtered)!=0)).rw());

  return nb_color;
}


/// \brief Main entry.
///
/// The main routine makes the base driver job. It looks after many
/// directories and lists every images in it. We use the boost library
/// to do it. Statistics are computed on the number of colors. The
/// first idea was to answer the question is this descriptor allow to
/// recognize a specific base.
int main()
{
  typedef boost::filesystem::path                   t_path;
  //typedef boost::filesystem::initial_path<t_path()> t_init_path;
  typedef boost::filesystem::directory_iterator     t_iter_path;

  t_path full_path[] = {t_path(ANNOTATING_1_BILL_IMG_PATH),
			t_path(ANNOTATING_1_FAX_IMG_PATH),
			t_path(ANNOTATING_1_HANDWRITTEN_IMG_PATH),
			t_path(ANNOTATING_1_LOGO_IMG_PATH),
			t_path(ANNOTATING_1_MAP_IMG_PATH),
			t_path(ANNOTATING_1_PHOTO_IMG_PATH),
			t_path(ANNOTATING_1_SCREENSHOT_IMG_PATH),
			t_path(ANNOTATING_1_SLIDE_IMG_PATH),
			t_path(ANNOTATING_1_TYPED_IMG_PATH)};

  for (int i = 0; i < 9; ++i)
  {
    std::cerr << "entering "   << full_path[i] << std::endl;
    std::cout << "entering "   << full_path[i] << std::endl;

    if (boost::filesystem::exists(full_path[i]) &&
	boost::filesystem::is_directory(full_path[i]))
    {
      boost::filesystem::system_complete(full_path[i]);
      const t_iter_path end_iter;
      unsigned count = 0;
      unsigned sum1  = 0;
      unsigned sum2  = 0;

      for (t_iter_path dir_iter(full_path[i]); end_iter != dir_iter; ++dir_iter)
      {
	unsigned val = count_image_color<8>(dir_iter->path().string());

	++count;
	sum1 += val;
	sum2 += val*val;

	std::cout << dir_iter->path().string() << " => " << val << std::endl;
      }

      float mean = sum1 / count;
      float var  = ((float)sum2 / count) - (mean * mean);

      std::cout << "mean : " << mean << std::endl;
      std::cout << "var  : " << var  << std::endl;
    }
  }

  return 0;
}
