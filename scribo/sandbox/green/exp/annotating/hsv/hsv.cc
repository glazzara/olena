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
/// \brief Implement the Millet HSV operator [millet.phd.2008.pdf]
///
/// This is the Millet code for moving from RGB space to Hue
/// one. Formulae are classical, we can find them on the web.
///
/// Val = max(R,G,B).
/// Sat = (max(R,G,B) - min(R,G,B))/max(R,G,B).
/// IF R = max(R,G,B) THEN Hue = 60 * [(V-B)/(max(R,G,B)-min(R,G,B))].
/// IF G = max(R,G,B) THEN Hue = 60 * [2 + (B-R)/(max(R,G,B)-min(R,G,B))].
/// IF B = max(R,G,B) THEN Hue = 60 * [4 + (R-G)/(max(R,G,B)-min(R,G,B))].
///
/// This operator try to integrate many processing in one shot, after
/// HSV transformation.


#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

#include <mln/img_path.hh>

#include <mln/accu/stat/histo1d.hh>

#include <mln/arith/minus.hh>
#include <mln/arith/div.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/convert.hh>
#include <mln/data/compute.hh>
#include <mln/data/stretch.hh>
#include <mln/data/transform.hh>

#include <mln/literal/zero.hh>
#include <mln/literal/colors.hh>
#include <mln/literal/grays.hh>

#include <mln/math/max.hh>
#include <mln/math/min.hh>
#include <mln/math/sqr.hh>
#include <mln/math/sqrt.hh>

#include <mln/opt/at.hh>

#include <mln/geom/nsites.hh>

#include <mln/fun/v2v/rgb_to_hue_map.hh>
#include <mln/fun/v2v/rgb_to_saturation_map.hh>
#include <mln/fun/v2v/rgb_to_value_map.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/plot/save_image_sh.hh>

#include <mln/value/rgb8.hh>

//============================================================================//
//                         HISTOGRAM
//============================================================================//

/// Histogram
/// \{
/// \brief Histogram processing methods.
///
/// In this part, we define methods for histogram processing as average,
/// peak, variance etc ...


/// \brief Sum all the bins of the histogram.
///
/// \param[in] img the histogram based on image.
///
/// \return the sum of the overall bins.
///
/// Sum evry bins and return the result.
template <typename I>
mln_value(I) count_histo(const mln::Image<I>& histo_)
{
  const   I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  mln_value(I) result = mln::literal::zero;
  mln_piter(I) p(histo.domain());

  for_all(p)
    result += histo(p);

  return result;
}

/// \brief Sum the whole frequencies.
///
/// \param[in] histo_ the histogram.
///
/// \return the number of pixels in the domain.
///
/// By summing all the frequencies of the domain, we count in fact the
/// number of pixels in the image.
template <typename I>
mln_value(I) sum_frequency_histo(const mln::Image<I>& histo_)
{
  const   I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  mln_value(I) sum = mln::literal::zero;
  mln_piter(I) p(histo.domain());

  for_all(p)
    sum += histo(p);

  return sum;
}

/// \brief Count the null frequencies.
///
/// \param[in] histo_ the histogram.
///
/// \return the number of null frequencies.
///
/// It's a way to access the number of distinct colors in the
/// histogram by comparing it with the histogram domain.
template <typename I>
mln_value(I) count_null_frequency_histo(const mln::Image<I>& histo_)
{
  const   I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  mln_value(I) count = mln::literal::zero;
  mln_piter(I) p(histo.domain());

  for_all(p)
    if (0 == histo(p))
      count++;

  return count;
}

/// \brief Find the peak of the histogram.
///
/// \param[in] histo_ the histogram.
///
/// \return the bin which contains the greatest value.
///
/// Compute the position of the peak of the histogram. To do this, we
/// view evrey bin and we maintain the maxima of the values and the
/// position. At the end, we return the position which contains the
/// greatest value.
template <typename I>
mln_coord(mln_site_(I)) peak_histo(const mln::Image<I>& histo_)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the peak of the histogram
  mln_value(I)            v_max = mln::opt::at(histo, mln::literal::zero);
  mln_coord(mln_site_(I)) p_max = mln::literal::zero;
  mln_piter(I)            p(histo.domain());

  for_all(p)
  {
    if (v_max < histo(p))
    {
      v_max = histo(p);
      p_max = p.ind();
    }
  }

  return p_max;
}

/// \brief Find the maximum frequency of the histogram.
///
/// \param[in] histo_ the histogram.
///
/// \return the maximum frequency of the histogram.
///
/// Find the peak and return its value, not its position. We use that
/// function to normalize histograms.
template <typename I>
mln_value(I) max_frequency_histo(const mln::Image<I>& histo_)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the value of the peak from the histogram
  mln_value(I)            max = mln::opt::at(histo, mln::literal::zero);
  mln_piter(I)            p(histo.domain());

  for_all(p)
  {
    max = mln::math::max(histo(p),max);
  }

  return max;
}

/// \brief Find the mean of the histogram.
///
/// \param[in] histo_ the histogram.
///
/// \return the aveerage of the histogram.
///
/// Compute the mean.
template <typename I>
float mean_histo(const mln::Image<I>& histo_)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the mean of the histogram
  float sum   = 0;
  float mean  = 0;

  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    sum  += histo(p);
    mean += p.ind()*histo(p);
  }

  mean = mean / sum;

  return mean;
}

/// \brief Compare the histogram with the equi distributed histogram.
///
/// \param[in] histo_ the histogram.
///
/// \return the distance between the equi distributed histogram.
///
/// Compute the square euclidian distance between histogram and the
/// equi distributed histogram. The equi distributed histogram is an
/// histogram in which each bin has the same frequency (id est 1/256
/// for instance). The integral equals one for this two
/// histograms. This is a major contribution test to differentiate
/// image database.
template <typename I>
float cmp_equi_frequency_histo(const mln::Image<I>& histo_)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the mean of the histogram
  float sum   = 0;
  float var   = 0;

  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    sum += histo(p);
    var += mln::math::sqr(histo(p) - (1/256.0));
  }

  var = var / sum;

  return var;
}

/// \brief Compute the variance on the histogram.
///
/// \param[in] histo_ the histogram.
///
/// \return the variance.
///
/// Compute the variance by substracting the mean.
template <typename I>
float var_histo(const mln::Image<I>& histo_, float mean)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the mean of the histogram
  float sum   = 0;
  float var   = 0;

  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    sum  += histo(p);
    var  += mln::math::sqr(p.ind() - mean) * histo(p);
  }

  var = var / sum;

  return var;
}

/// \brief Find the mean of the histogram.
///
/// \param[in] histo_ the histogram.
///
/// \return the aveerage of the histogram.
///
/// Compute the mean.
template <typename I>
float mean_frequency_histo(const mln::Image<I>& histo_)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the mean of the histogram
  float sum   = 0;
  float mean  = 0;

  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    sum++;
    mean += histo(p);
  }

  mean = mean / sum;

  return mean;
}

/// \brief Compute the standard deviation on the histogram.
///
/// \param[in] histo_ the histogram.
///
/// \return the dstandard deviation.
///
/// Compute the standard deviation by substracting the mean
template <typename I>
float stddev_frequency_histo(const mln::Image<I>& histo_, float mean)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the var of the histogram
  float sum  = 0;
  float var  = 0;

  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    sum++;
    var += mln::math::sqr(histo(p)-mean);
  }

  var = mln::math::sqrt(var / sum);

  return var;
}
/// \}

//============================================================================//
//                         HUE TEST
//============================================================================//

/// Hue test
/// \{
/// \brief Hue test image processing stuff.
///
/// In this part, we define every thing that is correlated to the hue analysis.


/// \brief Label a grey value.
///
/// \param[in] int_u8 the grey value.
///
/// \return the reference color for this grey value.
///
/// Classify grey value in three class, white, black and medium_gray.
mln::value::rgb8 label_val(const mln::value::int_u8 val)
{
  mln::value::rgb8 result;

  if (82 > val)
    result = mln::literal::black;
  else if (179 > val)
    result= mln::literal::medium_gray;
  else
    result = mln::literal::white;

  return result;
}


/// \brief Label color with orange reference or brown reference.
///
/// \param[in] color the rgb8 color.
/// \param[in] sat the HSV saturation.
/// \param[in] val the HSV value.
///
/// \return the reference color with which it has been associated to.
mln::value::rgb8 label_orange_or_brown(const mln::value::rgb8   color,
				       const mln::value::int_u8 sat,
				       const mln::value::int_u8 val)
{
  mln::value::rgb8 result;

  if (mln::literal::orange == color)
  {
    unsigned dist_orange = mln::math::abs(sat - 184)
      + mln::math::abs(val - 65);

    unsigned dist_brown  = mln::math::abs(sat - 255)
      + mln::math::abs(val - 125);

    if (dist_orange < dist_brown)
      result = mln::literal::orange;
    else
      result = mln::literal::brown;
  }
  else
    result = color;

  return result;
}


/// \brief Label between yellow and green color.
///
/// \param[in] color the rgb8 color.
/// \param[in] val the HSV value.
///
/// \return the reference color associated to that color.
mln::value::rgb8 label_yellow_or_green(const mln::value::rgb8   color,
				       const mln::value::int_u8 val)
{
  mln::value::rgb8 result;

  if (mln::literal::yellow == color)
  {
    // Is it green or yellow ?
    if (80 > val)
      result = mln::literal::green;
    else
      result = mln::literal::yellow;
  }
  else
    return color;

  return result;
}


/// \brief Label hue color.
///
/// \param[in] hue the HSV hue canal for a pixel.
///
/// \return a hue segmentation of the hue wheel.
///
/// The old classification given by Millet is commented. Mine is ready
/// to use.  We need to change reference colors to make the
/// primary colors, the secondary ones, etc...
mln::value::rgb8 label_hue(const mln::value::int_u8 hue)
{
  mln::value::rgb8 result;


  if (10 > hue)
    result = mln::literal::red;
  else if (32 > hue)
    result = mln::literal::orange;
  else if (53 > hue)
    result = mln::literal::yellow;
  else if (74 > hue)
    result = mln::literal::green; // chartreuse
  else if (96 > hue)
    result = mln::literal::green;
  else if (116 > hue)
    result = mln::literal::green;// turquoise, aigue-marine
  else if (138 > hue)
    result = mln::literal::green; // cyan
  else if (159 > hue)
    result = mln::literal::blue; // azur
  else if (181 > hue)
    result = mln::literal::blue;
  else if (202 > hue)
    result = mln::literal::violet;
  else if (223 > hue)
    result = mln::literal::pink;
  else // if (244 > hue)
    result = mln::literal::red;

  return result;
}

/// \brief The hue test.
///
/// \param[in] input the name of the input image.
/// \param[in] output the name of the histogram to output.
/// \param[in] tmp the name of the hue map to output.
/// \param[in] threshold the limit to compute the histogram contribution.
///
/// \return the proportion of pixels that pass the test.
///
/// This is an augmented test. We first normalized the histogram to
/// obtain a discrete distribution. Then we compute the mean (or the
/// peak, i don't really know what is the best) and we look at the
/// proportion of the pixels around the mean or the peak. This
/// proportion tell us how histogram distribution is concentrated
/// around the modal value. If a hue modal value exist, we can say
/// that the image is colorized.
float hue_test(const std::string input,
	       const std::string output,
	       const std::string tmp,
	       const short threshold)

{
  typedef mln::fun::v2v::rgb_to_hue_map<8>   t_rgb_to_hue_map;

  mln::image2d<mln::value::rgb8>   input_rgb8;
  mln::image2d<mln::value::int_u8> map;
  mln::image1d<unsigned>           histo;
  mln::image1d<float>              histo_float;
  float                            cnt1;
  float                            cnt2;
  float                            prop;
  short                            peak;
  mln::value::rgb8                 color;
  float                            sum;
  mln::point1d                     inf;
  mln::point1d                     sup;

  mln::io::ppm::load(input_rgb8, input.c_str());

  map         = mln::data::transform(input_rgb8, t_rgb_to_hue_map());
  histo       = mln::data::compute(mln::accu::meta::stat::histo1d(), map);
  sum         = sum_frequency_histo(histo);
  histo_float = mln::data::convert(float(), histo) / sum;
  peak        = mean_histo(histo); //peak_histo(histo);
  color       = label_hue(peak);
  inf         = mln::point1d(mln::math::max(0,   peak-threshold));
  sup         = mln::point1d(mln::math::min(255, peak+threshold));
  cnt1        = count_histo(histo_float|mln::box1d(inf,sup));
  cnt2        = count_histo(histo_float);
  prop        = ((100.0 * cnt1) / cnt2);

  mln::io::plot::save_image_sh(histo_float, output.c_str());
  mln::io::pgm::save(map, tmp.c_str());
//  std::cout << "peak  = " << peak  << std::endl;
//  std::cout << "color = " << color << std::endl;

  return prop;
}
/// \}

//============================================================================//
//                         SATURATION TEST
//============================================================================//

/// Saturation test
/// \{
/// \brief Saturation test image processing stuff.
///
/// In this part, we define every thing that is correlated to the
/// saturation analysis.


/// \brief The saturation test.
///
/// \param[in] input the name of the input image.
/// \param[in] output the name of the histogram to output.
/// \param[in] tmp the name of the hue map to output.
/// \param[in] threshold for deciding which is low saturation or not.
///
/// \return the proportion of pixels which pass the test.
///
/// This is the augmented test, we normalized the histogram and then
/// we count the proportion of the histogram below the threshold.
float saturation_test(const std::string input,
		      const std::string output,
		      const std::string tmp,
		      const short threshold)

{
  typedef mln::fun::v2v::rgb_to_saturation_map<8>    t_rgb_to_saturation_map;

  mln::image2d<mln::value::rgb8>   input_rgb8;
  mln::image2d<mln::value::int_u8> map;
  mln::image1d<unsigned>           histo;
  mln::image1d<float>              histo_float;
  float                            cnt1;
  float                            cnt2;
  float                            sum;
  float                            prop;

  mln::io::ppm::load(input_rgb8, input.c_str());

  map         = mln::data::transform(input_rgb8, t_rgb_to_saturation_map());
  histo       = mln::data::compute(mln::accu::meta::stat::histo1d(), map);
  sum         = sum_frequency_histo(histo);
  histo_float = mln::data::convert(float(), histo) / sum;
  cnt1        = count_histo(histo_float | mln::box1d(mln::point1d(0),
						     mln::point1d(threshold)));
  cnt2        = count_histo(histo_float);
  prop        = ((100.0 * cnt1) / cnt2);

  mln::io::plot::save_image_sh(histo_float, output.c_str());
  mln::io::pgm::save(map, tmp.c_str());

  return prop;
}
/// \}

//============================================================================//
//                         VALUE TEST
//============================================================================//

/// Value test
/// \{
/// \brief Value test image processing stuff.
///
/// In this part, we define every thing that is correlated to the
/// value analysis.



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

/// \brief The stddev3 is an internal stuff.
///
/// \param[in] histo_ the image which represents the histogram.
/// \param[in] peak   the position of the histogram peak.
///
/// \return simple computing of deviation.
///
/// This is an internal stuff. It splits the computing for easy
/// reusing practice. Sum the R contribution.
template <typename I>
float stddev3(const mln::Image<I>& histo_, unsigned peak)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Compute stddev

  float stddev = 0.0;

  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    stddev  += r((short)peak, mln::opt::at(histo,peak), p.ind(), histo(p));
  }

  return stddev;
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
/// to the pic (five pixels around it). The test is generalized by
/// making constants as parameters.
template <typename I>
float stddev2(const mln::Image<I>& histo_, unsigned peak, unsigned limit)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  float stddev_low = 0.0;
  float stddev_up  = 0.0;
  float ret        = 0.0;

  // A transformer avec des iterators

  if (250 > peak)
    stddev_up  = stddev3(histo |mln::box1d(mln::point1d(peak+1),
					   mln::point1d(peak+limit)), peak);

  if (5 < peak)
    stddev_low = stddev3(histo |mln::box1d(mln::point1d(peak-limit),
					   mln::point1d(peak-1)), peak);

  ret = (250 < peak)? stddev_low : (5 > peak)? stddev_up :
    (stddev_low + stddev_up)/2;

  return ret;
}


/// \brief This is the Value test.
///
/// \param[in] input the name of the input image.
/// \param[in] output the name of result histogram.
/// \param[in] tmp the name of the output value (HSV) map.
/// \param[in] threshold the range around the peak where R is computed.
///
/// \return the proportion of pixels that pass the test.
///
/// This is the augmented version of the Millet test. This code is not
/// stable.  There is a lots of tests on. The Millet test is computed
/// with stddev2 call.  We test simple counting around the pic as we
/// do for other tests but it's not really insteresting. The prop4 is
/// the best for me. We test the difference between the distribution and the
/// equi-distribution.
float value_test(const std::string input,
		 const std::string output,
		 const std::string tmp,
		 const short threshold)

{
  typedef mln::fun::v2v::rgb_to_value_map<8>   t_rgb_to_value_map;

  mln::image2d<mln::value::rgb8>   input_rgb8;
  mln::image2d<mln::value::int_u8> map;
  mln::image1d<unsigned>           histo;
  mln::image1d<float>              histo_float;
  float                            cnt1;
  float                            cnt2;
  float                            prop;
  float                            sum;
  float                            prop4;
  short                            peak;
  mln::point1d                     inf;
  mln::point1d                     sup;


  mln::io::ppm::load(input_rgb8, input.c_str());

  map         = mln::data::transform(input_rgb8, t_rgb_to_value_map());
  histo       = mln::data::compute(mln::accu::meta::stat::histo1d(), map);
  sum         = sum_frequency_histo(histo);
  histo_float = mln::data::convert(float(), histo) / sum;
  prop4       = cmp_equi_frequency_histo(histo_float);
  peak        = peak_histo(histo); // mean_histo(histo);
  //prop        = stddev2(histo, peak, threshold);
  inf         = mln::point1d(mln::math::max(0,   peak-threshold));
  sup         = mln::point1d(mln::math::min(255, peak+threshold));
  cnt1        = count_histo(histo_float|mln::box1d(inf,sup));
  cnt2        = count_histo(histo_float);
  prop        = ((100.0 * cnt1) / cnt2);

  std::cerr << "peak = " << peak << std::endl;
  std::cerr << "inf  = " << inf  << std::endl;
  std::cerr << "sup  = " << sup  << std::endl;
  std::cerr << "cnt1 = " << cnt1 << std::endl;
  std::cerr << "cnt2 = " << cnt2 << std::endl;
  std::cerr << "prop = " << prop << std::endl;
  std::cerr << "prop4= " << prop4 << std::endl;

  mln::io::plot::save_image_sh(histo_float, output.c_str());
  mln::io::pgm::save(map, tmp.c_str());

  return prop;
}

//============================================================================//
//                         MAIN
//============================================================================//


/// \brief The main entry.
///
/// This is a front end for launching image processing test. We deal
/// with boost library to walk on image database directories. Nothing
/// very interesting here, it is a lot of plumberies.
int main()
{
  typedef boost::filesystem::path                   t_path;
  typedef boost::filesystem::directory_iterator     t_iter_path;

//   t_path full_path[] = {t_path(ICDAR_20P_TEXT_ONLY_IMG_PATH),
//  			t_path(ICDAR_20P_TEXT_COLOR_IMG_PATH),
//  			t_path(ICDAR_20P_TEXT_PHOTO_IMG_PATH)};

  t_path full_path[] = {t_path(AFP_PPM_IMG_PATH)};

  std::cout << "#!/usr/bin/gnuplot"                       << std::endl;
  std::cout << "set terminal x11 persist 1"               << std::endl;
  std::cout << "#HUE - SATURATION - VALUE"                << std::endl;
  std::cout << "plot '-' using 1:2 with point notitle,\\" << std::endl;
  std::cout << "     '-' using 1:2 with point notitle,\\" << std::endl;
  std::cout << "     '-' using 1:2 with point notitle"    << std::endl;

  for (int i = 0; i < 1; ++i)
  {
    if (boost::filesystem::exists(full_path[i]) &&
	boost::filesystem::is_directory(full_path[i]))
    {
      boost::filesystem::system_complete(full_path[i]);
      const t_iter_path end_iter;
      float             prop = 0.0;
      t_path            directory;
      t_path            leaf;
      t_path            output;
      t_path            tmp;

      std::cerr << "entering "   << full_path[i] << std::endl;

      for (t_iter_path dir_iter(full_path[i]); end_iter != dir_iter; ++dir_iter)
      {
	std::cerr << dir_iter->path() << std::endl;
	// concatenation de chaine
//	directory = (ANNOTATING_ICDAR_H_INPUT_RET_PATH);
	directory = (ANNOTATING_AFP_H_INPUT_RET_PATH);
	leaf      = dir_iter->path().leaf();
	output    = change_extension(directory / leaf, ".sh");
	tmp       = change_extension(directory / leaf, ".pgm");

	prop = hue_test(dir_iter->path().string(),
			output.string(),
			tmp.string(),
			20);

	std::cout << prop << " ";

//	directory = (ANNOTATING_ICDAR_S_INPUT_RET_PATH);
	directory = (ANNOTATING_AFP_S_INPUT_RET_PATH);
	leaf      = dir_iter->path().leaf();
	output    = change_extension(directory / leaf, ".sh");
	tmp       = change_extension(directory / leaf, ".pgm");

	prop = saturation_test(dir_iter->path().string(),
			       output.string(),
			       tmp.string(),
			       25);

	std::cout << prop << " ";

//	directory = (ANNOTATING_ICDAR_V_INPUT_RET_PATH);
	directory = (ANNOTATING_AFP_V_INPUT_RET_PATH);
	leaf      = dir_iter->path().leaf();
	output    = change_extension(directory / leaf, ".sh");
	tmp       = change_extension(directory / leaf, ".pgm");

	prop = value_test(dir_iter->path().string(),
			  output.string(),
			  tmp.string(),
			  15);

	std::cout << prop  << " ";
	std::cout << "# "  << dir_iter->path().leaf() << std::endl;
      }
      std::cout << "e"   << std::endl;
    }
  }

  return 0;
}
