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
/// \brief Implement the Millet clipart detection [millet.phd.2008.pdf]
///
/// The aim of this descriptor is to recognize clipart. To do this, we
/// assume that clipart have their histogram concentrated around their
/// mode. This is particularly true if the clipart is design by hand,
/// because a very small number of grey tones or colors are used to
/// draw it. But sometimes, computer assists their creation and we can
/// find some continuous set of tones and this artefact create noise
/// for the detection technique describe bellow.

#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

#include <mln/img_path.hh>

#include <mln/accu/max_site.hh>
#include <mln/accu/stat/histo1d.hh>

#include <mln/core/macros.hh>
#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>

#include <mln/data/compute.hh>
#include <mln/data/fill.hh>
#include <mln/data/transform.hh>

#include <mln/fun/v2v/rgb8_to_int_u8.hh>

#include <mln/io/ppm/load.hh>

#include <mln/math/sqr.hh>

#include <mln/opt/at.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/int_u.hh>

/// \brief The R function of Millet
///
/// \param[in] p       the position of the pic.
/// \param[in] histo_p the histo value of the pic.
/// \param[in] x       the position of the element which we compute the contrib.
/// \param[in] histo_x the histo value of that element.
///
/// \result the contribution of the element x.
///
/// This function compute the variance-like contribution of an element
/// x linked to the pic of the histogram. In fact, every thing is like
/// we compute a square distance-like between the element x and the
/// pic in the normalized histogram. Notice that the normalized
/// histogram is the histogram divide by the value of it's pic. So at
/// the pic, the value equals 1. It's a current representation of the
/// histogram in image processing, we can find it in gimp for exemple.
float r(short p, unsigned histo_p, short x, unsigned histo_x)
{
  float result = mln::math::sqr(((float)histo_x / histo_p) * (x-p));

  return result;
}

/// Brief compute the whole deviation of Millet
///
/// \param[in] image the input image to analyze.
///
/// \return the deviation.
///
/// The deviation uses the R function. It stats by finding the pic. If
/// the pic is near the rigth border of the histo, compute the R
/// function on the left neighbouring of the pic. If the pic is near
/// the left border of the histo, compute the R function on the right
/// neigbouring. Otherwise, compute the average of the right and left
/// results. The selected neighbouring is composed of five pixels at
/// the right or at the left of the pic. The detection of clipart
/// assumes that the majority of the energy of the histogram is closed
/// to the pic (five pixels around it).
float stddev_color(const std::string& image)
{
  typedef mln::point1d                                t_point1d;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
  typedef mln::image1d<unsigned>                      t_histo1d;
  typedef mln::fun::v2v::rgb8_to_int_u8               t_rgb8_to_int_u8;
  typedef mln::accu::meta::stat::histo1d              t_histo1d_fun;
  typedef mln::accu::max_site<t_histo1d>              t_max_site_fun;

  t_image2d_rgb8                                      input_rgb8;
  t_image2d_int_u8                                    input_int_u8;
  t_histo1d                                           histo;
  t_point1d                                           max_site;

  mln::io::ppm::load(input_rgb8, image.c_str());
  input_int_u8 = mln::data::transform(input_rgb8, t_rgb8_to_int_u8());
  histo        = mln::data::compute(t_histo1d_fun(), input_int_u8);

  // Find the peak of the histogram
  unsigned v_max = mln::opt::at(histo, 0);
  short    p_max = 0;

  mln_piter_(t_histo1d) p(histo.domain());

  for_all(p)
  {
    if (v_max < histo(p))
    {
      v_max = histo(p);
      p_max = p.ind();
    }
  }

  // Compute the specific stddev

  float stddev_low = 0.0;
  float stddev_up  = 0.0;
  float stddev     = 0.0;

  if (250 > p_max)
    for (short i = p_max+1; i < p_max+6; ++i)
      stddev_up += r(p_max, mln::opt::at(histo,p_max),
		     i, mln::opt::at(histo,i));

  if (5 < p_max)
    for (short i = p_max-1; i > p_max-6; --i)
      stddev_low += r(p_max, mln::opt::at(histo,p_max),
		      i, mln::opt::at(histo,i));

  stddev = (250 < p_max)? stddev_low : (5 > p_max)? stddev_up :
    (stddev_low + stddev_up)/2;

  return stddev;
}


/// \brief Main entry.
///
/// Front end for image processing. It is in the main routine where we
/// go through the image data base (using boost to do it).
int main()
{
  typedef boost::filesystem::path                   t_path;
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
      float count = 0;
      float sum1  = 0;
      float sum2  = 0;

      for (t_iter_path dir_iter(full_path[i]); end_iter != dir_iter; ++dir_iter)
      {
	float val = stddev_color(dir_iter->path().string());

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
