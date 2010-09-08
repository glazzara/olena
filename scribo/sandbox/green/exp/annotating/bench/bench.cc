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
/// This file is the final work to differentiate between the two image
/// databases AFP and ICDAR. Several tests are used to show
/// improvement of detection. The jonathan's tests are put together to
/// compare them to the others (in fact, to the low saturation one).

#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

#include <mln/algebra/vec.hh>

#include <mln/img_path.hh>

#include <mln/accu/stat/mean.hh>
#include <mln/accu/stat/histo1d.hh>

#include <mln/arith/minus.hh>
#include <mln/arith/times.hh>
#include <mln/arith/diff_abs.hh>
#include <mln/arith/div.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/alias/point1d.hh>
#include <mln/core/alias/box1d.hh>

#include <mln/data/transform.hh>
#include <mln/data/compute.hh>
#include <mln/data/convert.hh>
#include <mln/data/stretch.hh>
#include <mln/data/fill.hh>

#include <mln/fun/v2v/component.hh>
#include <mln/fun/v2v/rgb_to_hue_map.hh>
#include <mln/fun/v2v/rgb_to_saturation_map.hh>
#include <mln/fun/v2v/rgb_to_value_map.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/plot/save_image_sh.hh>

#include <mln/literal/zero.hh>

#include <mln/math/ceil.hh>
#include <mln/math/floor.hh>

#include <mln/opt/at.hh>

#include <mln/trait/value_.hh>

#include <mln/value/rgb8.hh>


#include <mln/value/int_u8.hh>

/// Histogram part.
///
/// We regroup here the routines that work on histogram, count_histo,
/// sum_frequency_histo, peak_histo, count_null_frequency_histo,
/// cmp_equi_frequency_histo and others (variance
/// operators). count_histo and sum_frequency_histo count the overall
/// frequency in the domain.  peak_histo find the peak of the
/// histogram.  count_null_frequency_histo compute the number of non
/// grey levels.  cmp_equi_frequency_histo compare a histogram with
/// the one with the same frequency everywhere.
/// \{
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

template <typename I>
float cmp_equi_frequency_histo(const mln::Image<I>& histo_)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  float sum   = 0;
  float var   = 0;

  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    sum ++;
    var += mln::math::sqr(histo(p) - (1/256.0));
  }

  var = var / sum;

  return var;
}

template <typename I>
mln_value(I) cnt_histo(const mln::Image<I>& histo_)
{
  const   I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  mln_value(I) sum = mln::literal::zero;
  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    sum += histo(p);
  }

  return sum;
}

template <typename I>
mln_value(I) sum_histo(const mln::Image<I>& histo_)
{
  const   I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  mln_value(I) pos = mln::literal::zero;
  mln_value(I) sum = mln::literal::zero;
  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    pos =  p.ind();
    sum += pos*histo(p);
  }

  return sum;
}

template <typename I>
mln_value(I) avg_histo(const mln::Image<I>& histo_)
{
  const   I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  mln_value(I) pos = mln::literal::zero;
  mln_value(I) sum = mln::literal::zero;
  mln_value(I) cnt = mln::literal::zero;
  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    pos =  p.ind();
    cnt += histo(p);
    sum += pos*histo(p);
  }

  return (0 == cnt)? 0 : sum/cnt;
}

template <typename I>
mln_value(I) var3_histo(const mln::Image<I>& histo_, float mean)
{
  const   I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  mln_value(I) pos = mln::literal::zero;
  mln_value(I) sum = mln::literal::zero;
  mln_value(I) cnt = mln::literal::zero;
  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    cnt += histo(p);
    sum += (mln::math::sqr(p.ind()-mean)*histo(p));
  }

  return (0 == cnt)? 0 : sum/cnt;
}

template <typename I>
mln_value(I) var2_histo(const mln::Image<I>& histo_, float mean)
{
  const   I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  mln_value(I) pos = mln::literal::zero;
  mln_value(I) sum = mln::literal::zero;
  mln_value(I) sqr = mln::literal::zero;
  mln_value(I) cnt = mln::literal::zero;
  mln_value(I) dlt = mln::literal::zero;
  mln_value(I) mxt = mln::literal::zero;
  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    pos =  p.ind();
    cnt += (histo(p));
    sum += (histo(p)*pos);
    mxt += (histo(p)*pos*mean);
    sqr += (histo(p)*mln::math::sqr(pos));
    dlt += (histo(p)*mln::math::sqr(pos - mean));

    std::cout << "p          = " << pos                       << std::endl;
    std::cout << "p²         = " << mln::math::sqr(pos)       << std::endl;
    std::cout << "p*mean     = " << (pos*mean)                << std::endl;
    std::cout << "p-mean     = " << (pos-mean)                << std::endl;
    std::cout << "(p-mean)²  = " << mln::math::sqr(pos-mean)  << std::endl;
    std::cout << "histo(p)   = " << histo(p)                  << std::endl;
    std::cout << "histo(p)*p = " << (pos*histo(p))            << std::endl;
    std::cout << "histo(p)*p²= " << (mln::math::sqr(pos)*histo(p))
	      << std::endl;
    std::cout << "cnt        = " << cnt      << std::endl;
    std::cout << "sum        = " << sum      << std::endl;
    std::cout << "sqr        = " << sqr      << std::endl;
    std::cout << "dlt        = " << dlt      << std::endl;
    std::cout << "mxt        = " << mxt      << std::endl;
    std::cout << std::endl;
  }

  std::cout << "sqr/cnt      = " << (sqr/cnt)               << std::endl;
  std::cout << "sum/cnt      = " << (sum/cnt)               << std::endl;
  std::cout << "(sum/cnt)²   = " << mln::math::sqr(sum/cnt) << std::endl;
  std::cout << "dlt/cnt      = " << dlt/cnt                 << std::endl;
  std::cout << "mxt/cnt      = " << mxt/cnt                 << std::endl;
  std::cout << std::endl;

  std::cout << "sqr                     = "
	    << (sqr)                                          << std::endl;
  std::cout << "dlt                     = "
	    << (dlt)                                          << std::endl;
  std::cout << "cnt*avg²                = "
	    << (mln::math::sqr(sum/cnt)*cnt)                  << std::endl;
  std::cout << "2*mxt                   = "
	    << (2*mxt)                                        << std::endl;
  std::cout << "sqr - cnt*avg²          = "
	    << (sqr/cnt - mln::math::sqr(sum/cnt))            << std::endl;
  std::cout << "(sqr -2*mxt + cnt*avg²) = "
	    <<  ((sqr - 2*mxt + mln::math::sqr(sum/cnt))/cnt) << std::endl;
  std::cout << std::endl;
  return (0 == cnt)? 0 : sqr/cnt - mln::math::sqr(sum/cnt);
}

template <typename I>
mln_value(I) var_histo(const mln::Image<I>& histo_)
{
  const   I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  mln_value(I) pos = mln::literal::zero;
  mln_value(I) sum = mln::literal::zero;
  mln_value(I) sqr = mln::literal::zero;
  mln_value(I) cnt = mln::literal::zero;
  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    pos =  p.ind();
    cnt += (histo(p));
    sum += (histo(p)*pos);
    sqr += (histo(p)*mln::math::sqr(pos));
  }

  return (0 == cnt)? 0 : sqr/cnt - mln::math::sqr(sum/cnt);
}

template <typename I>
mln_value(I) sqr_histo(const mln::Image<I>& histo_)
{
  const   I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  mln_value(I) sum = mln::literal::zero;
  mln_piter(I) p(histo.domain());

  for_all(p)
    sum += (mln::math::sqr(p.ind())*histo(p));

  return sum;
}
/// \}


/// \brief Millet Hue detector.
///
/// \param[in] histo the histogram image on which applying the detector.
/// \param[in] threshold a way to adapt the test to the population.
///
/// \return the proportion of population that verify the hue detector.
///
/// This test is used for discrimination between black and white pictures and
/// color ones. Some colored Black and white pictures have their energy near
/// the peak.
float hue1_descriptor(mln::image1d<unsigned> histo, const short threshold)
{
  float                            cnt1;
  float                            cnt2;
  float                            prop;
  short                            peak;
  mln::point1d                     inf;
  mln::point1d                     sup;

  peak = peak_histo(histo);
  inf  = mln::point1d(mln::math::max(0,   peak-threshold));
  sup  = mln::point1d(mln::math::min(255, peak+threshold));
  cnt1 = count_histo(histo|mln::box1d(inf,sup));
  cnt2 = count_histo(histo);
  prop = ((255.0 * cnt1) / cnt2);

  return prop;
}


/// \brief Saturation detector.
///
/// \param[in] histo the histogram image on which applying the detector.
/// \param[in] threshold a way to adapt the test to the population.
///
/// \return the proportion of population that verify the hue detector.
///
/// This test is used for discrimination between black and white pictures and
/// color ones. Black and white pictures have their energy in the low saturation
/// band.
float sat1_descriptor(mln::image1d<unsigned> histo, const short threshold)
{
  float                            cnt1;
  float                            cnt2;
  float                            result;

  cnt1        = count_histo(histo | mln::box1d(mln::point1d(0),
					       mln::point1d(threshold)));
  cnt2        = count_histo(histo);
  result      = ((255.0 * cnt1) / cnt2);

  return result;
}


/// \brief Counting grey levels descriptor.
///
/// \param[in] histo the histogram image on which applying the detector.
///
/// \return the number of grey levels.
///
/// This test aims at compute the number of grey levels. Photographies tends to
/// use all the levels or many of them.
float lvl0_descriptor(mln::image1d<unsigned> histo)
{
  float                            result;

  // FIXME 255
  result      = 255-count_null_frequency_histo(histo);

  return result;
}


/// \brief Density hue detector.
///
/// \param[in] histo the histogram image on which applying the detector.
///
/// \return the proportion of population that verify the hue detector.
///
/// This test works on density histogram and compare to the
/// equi-frequency histogram. If the normalized histogram show a peak,
/// it will differ from the reference density.
float hue0_descriptor(mln::image1d<unsigned> histo)
{
  mln::image1d<float>              histo_float;
  float                            sum;
  float                            result;

  sum         = sum_frequency_histo(histo);
  histo_float = mln::data::convert(float(), histo) / sum;
  result      = cmp_equi_frequency_histo(histo_float);

  return result*255;
}


/// \brief Density saturation detector.
///
/// \param[in] histo the histogram image on which applying the detector.
///
/// \return the proportion of population that verify the hue detector.
///
/// This test works on density histogram and compare to the
/// equi-frequency histogram. If the normalized histogram show a peak,
/// it will differ from the reference density.
float sat0_descriptor(mln::image1d<unsigned> histo)
{
  mln::image1d<float>              histo_float;
  float                            sum;
  float                            result;

  sum         = sum_frequency_histo(histo);
  histo_float = mln::data::convert(float(), histo) / sum;
  result      = cmp_equi_frequency_histo(histo_float);

  return result*255;
}


/// \brief Density value detector.
///
/// \param[in] histo the histogram image on which applying the detector.
///
/// \return the proportion of population that verify the hue detector.
///
/// This test works on density histogram and compare to the
/// equi-frequency histogram. If the normalized histogram show a peak,
/// it will differ from the reference density.
float val0_descriptor(mln::image1d<unsigned> histo)
{
  mln::image1d<float>              histo_float;
  float                            sum;
  float                            result;

  sum         = sum_frequency_histo(histo);
  histo_float = mln::data::convert(float(), histo) / sum;
  result      = cmp_equi_frequency_histo(histo_float);

  return result*255;
}


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
/// \param[in] peak the peak of the histogram.
/// \param[in] limit the threshold to compute the contribution.
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


/// \brief Millet value descriptor (cf. Millet.phd.2008)
///
/// \param[in] histo the histogram image on which applying the detector.
/// \param[in] threshold the limit where to compute the contribution.
///
/// \return the proportion of population that verify the hue detector.
///
/// This test aims at compute some deviation on the peak of the histogram of
/// the image. Large deviations mean lots of graduation in colors (such as
/// photos) and small ones mean something like cartoon.
float val1_descriptor(mln::image1d<unsigned> histo, const short threshold)
{
  typedef mln::fun::v2v::rgb_to_value_map<8>   t_rgb_to_value_map;

  float                            result;
  short                            peak;

  peak        = peak_histo(histo);
  result      = stddev2(histo, peak, threshold);

  return result;
}


/// \brief Error detector (jonathan idea).
///
/// \param[in] r_img_map the original red channel.
/// \param[in] g_img_map the original green channel.
/// \param[in] b_img_map the original blue channel.
/// \param[in] r_rdc_map the reduced (in color) red channel.
/// \param[in] g_rdc_map the reduced (in color) green channel.
/// \param[in] b_rdc_map the reduced (in color) blue channel.
///
/// \return the PNSNR (cf compression p278 Handbook Color).
///
/// \fixme the actual PNSNR is unbound, we need to fix its max to
/// 255. Two images which are the same produces an infinite PNSNR.
float err_descriptor(mln::image2d<mln::value::int_u8> r_img_map,
		     mln::image2d<mln::value::int_u8> g_img_map,
		     mln::image2d<mln::value::int_u8> b_img_map,
		     mln::image2d<mln::value::int_u8> r_rdc_map,
		     mln::image2d<mln::value::int_u8> g_rdc_map,
		     mln::image2d<mln::value::int_u8> b_rdc_map)


{
  typedef mln::accu::meta::stat::mean          t_mean;
  typedef mln::image2d<mln::value::int_u8>     t_map;
  typedef mln_trait_op_minus_(t_map,t_map)     t_minus;
  typedef mln_trait_op_times_(t_minus,t_minus) t_times;


  t_minus                          minus_red;
  t_minus                          minus_green;
  t_minus                          minus_blue;

  t_times                          times_red;
  t_times                          times_green;
  t_times                          times_blue;

  float                            error_red;
  float                            error_green;
  float                            error_blue;

  float                            error;

  minus_red      = (r_img_map - r_rdc_map);
  times_red      = minus_red * minus_red;

  minus_green    = (g_img_map - g_rdc_map);
  times_green    = minus_green * minus_green;

  minus_blue     = (b_img_map - b_rdc_map);
  times_blue     = minus_blue * minus_blue;

  error_red      = mln::data::compute(t_mean(), times_red);
  error_green    = mln::data::compute(t_mean(), times_green);
  error_blue     = mln::data::compute(t_mean(), times_blue);

  error          = (error_red + error_green + error_blue)/3.0;
  error          = mln::math::sqrt(error);
  error          = 20 * log(255/error);

// FIXME:
// SAME IMAGE PRODUCE PNSNR GOING THROW THE INFINITY.
// DIFFERENT IMAGE PRODUCE PNSNR GOING NEAR ZERO.
// WE SHOULD KEEP THE PNSNR BELOW 255 FOR COMPARAISON.

  return error;
}

/// Discriminant operators.
///
/// We have some operators that compute the threshold which separate
/// two sub-populations in a histogram.
/// /{

/// \brief Compute discrimination threshold.
/// \param[in] avg1 the mean of the population 1.
/// \param[in] var1 the variance of the population 1.
/// \param[in] avg2 the mean of the population 2.
/// \param[in] var2 the variance of the population 2.
///
/// \return the threshold to discriminate the two populations.
///
/// Linear discriminant analysis in 1d is done. When two population
/// have the same variance, the threshold is at (m1+m2)/2. When threre
/// are different variances, we propose the threshold at the position
/// (m1*sqrt(v1)+m2*sqrt(v2))/(sqrt(v1)+sqrt(v2)).
float threshold_histo(float avg1, float var1, float avg2, float var2)
{
  float sigma1    = mln::math::sqrt(var1);
  float sigma2    = mln::math::sqrt(var2);
  float threshold = (avg1*sigma1+avg2*sigma2)/(sigma1+sigma2);

  return threshold;
}

/// \brief Compute discrimination threshold.
/// \param[in] avg1 the mean of the population 1.
/// \param[in] var1 the variance of the population 1.
/// \param[in] avg2 the mean of the population 2.
/// \param[in] var2 the variance of the population 2.
///
/// \return the threshold to discriminate the two populations.
///
/// Linear discriminant analysis in 1d is done. When two population
/// have the same variance, the threshold is at (m1+m2)/2. When threre
/// are different variances, we propose the threshold at the position
/// (m1*var1+m2*var2)/(sqrt(v1)+sqrt(v2)).

float threshold3_histo(float avg1, float var1, float avg2, float var2)
{
  float threshold = (avg1*var1+avg2*var2)/(var1+var2);

  return threshold;
}


/// \brief Compute discrimination threshold.
/// \param[in] avg1 the mean of the population 1.
/// \param[in] var1 the variance of the population 1.
/// \param[in] avg2 the mean of the population 2.
/// \param[in] var2 the variance of the population 2.
///
/// \return the threshold to discriminate the two populations.
///
/// Gaussian discriminant analysis in 1d is done. Compute the
/// discrimanation and solve the parabolic equation.
float threshold2_histo(float avg1, float var1, float avg2, float var2)
{
  float a = var2 - var1;
  float b = -2 * (avg1 * var2 - avg2 * var1);
  float c = var2 * mln::math::sqr(avg1) - var1 * mln::math::sqr(avg2);
  float d = mln::math::sqr(b) - 4 * a * c;

  if (d < 0)
    std::cout << "delta negatif" << std::endl;

  float x1 = (-b+mln::math::sqrt(d))/(2*a);
  float x2 = (-b-mln::math::sqrt(d))/(2*a);

  std::cout << "a  = " << a << std::endl;
  std::cout << "b  = " << b << std::endl;
  std::cout << "c  = " << c << std::endl;
  std::cout << "d  = " << d << std::endl;
  std::cout << "x1 = " << x1 << std::endl;
  std::cout << "x2 = " << x2 << std::endl;

  return x2;
}

/// \brief Minimisation of the error.
///
/// \param[in] histo_grp1 the histogram of the first population.
/// \param[in] histo_grp2 the histogram of the second population.
///
/// \return the threshold.
///
/// Computes the error and find the minimum error threshold. It's just
/// a counting of element in four categories (0 - but detects 1, 1 -
/// but detects 1, 1 but detects 0, 0 but detects 0). The error is the
/// sum of missclassifications for the classes 0 and 1. As Otsu done,
/// we can iterate on the threshold and compute the error associated
/// to it. The bes threshold is the one that minimize the error.
short min_error(const mln::image1d<float> histo_grp1,
		const mln::image1d<float> histo_grp2,
		float *_c00, float *_c10, float *_c01, float *_c11)
{
  float c00[256];
  float c10[256];
  float c01[256];
  float c11[256];
  float err[256];

  for (short p = 0; p < 256; p++)
  {
    c00[p] = cnt_histo(histo_grp1|mln::box1d(mln::point1d(0),
					     mln::point1d(p)));

    c10[p] = cnt_histo(histo_grp1|mln::box1d(mln::point1d(p+1),
					     mln::point1d(255)));

    c01[p] = cnt_histo(histo_grp2|mln::box1d(mln::point1d(0),
					     mln::point1d(p)));

    c11[p] = cnt_histo(histo_grp2|mln::box1d(mln::point1d(p+1),
					     mln::point1d(255)));
  }

  short threshold = 0;
  float error     = c01[0] + c01[0] + c00[0] + c11[0];

  for(short p = 0; p < 256; p++)
  {
    err[p] = c10[p] + c01[p];

//     std::cout << " p   = " << p
// 	      << ";c00 = " << c00[p]
// 	      << ";c10 = " << c10[p]
// 	      << ";c01 = " << c01[p]
// 	      << ";c11 = " << c11[p]
// 	      << std::endl;
//    std::cout << "err[" << p <<  "] = " << err[p] << std::endl;

    if (error > err[p])
    {
      error     = err[p];
      threshold = p;
    }
  }

  *_c00 = c00[threshold];
  *_c10 = c10[threshold];
  *_c01 = c01[threshold];
  *_c11 = c11[threshold];

  return threshold;
}


/// \brief Fisher analysis.
///
/// \param[in] histo the histogram of the mixed population.
///
/// \return the threshold.
///
/// This routine is less performant than the others because it starts
/// with a mixed population, but we know exactly the two separated
/// population. It was just a test. Don't take it in production
/// environement, it doesn't help you.
short fisher_analysis(const mln::image1d<float> histo)
{
  typedef mln::value::int_u8 t_int_u8;

  // FIXE ME SIZE const short a =  mln_min(t_int_u8);
  //              float cnt1[a];

  float cnt1[256];
  float sum1[256];
  float sqr1[256];
  float avg1[256];
  float var1[256];

  float cnt2[256];
  float sum2[256];
  float sqr2[256];
  float avg2[256];
  float var2[256];

  float cnt0[256]; // count of points
  float sum0[256]; // sum of population
  float sqr0[256]; // sqr of population
  float avg0[256]; // average of the population
  float v_in[256]; // variance with-in class
  float v_bw[256]; // variance between class
  float var0[256]; // variance of the population
  short threshold;
  float pos;
  float min_var;

  // Assign the first elements
  cnt1[0]   = 0;
  sum1[0]   = 0;
  sqr1[0]   = 0;
  avg1[0]   = 0;
  var1[0]   = 0;

  // Compute the stats of the wall histogram
  cnt2[0]   = 0;
  sum2[0]   = 0;
  sqr2[0]   = 0;

  for(short p = 0; p < 256; p++)
  {
    pos     =  p;
    cnt2[0] += mln::opt::at(histo,p);
    sum2[0] += (pos * mln::opt::at(histo,p));
    sqr2[0] += (mln::math::sqr(pos) * mln::opt::at(histo,p));
  }

  avg2[0]   = (0 == cnt2[0])? 0 : sum2[0] / cnt2[0];
  var2[0]   = (0 == cnt2[0])? 0 : sqr2[0] / cnt2[0] - mln::math::sqr(avg2[0]);

  // watch the array limits
  for (short p = 1; p < 256; p++)
  {
    pos     = p;

    // Assign the statistics to the primary class
    cnt1[p] = cnt1[p-1] + mln::opt::at(histo, p);
    sum1[p] = sum1[p-1] + pos * mln::opt::at(histo, p);
    sqr1[p] = sqr1[p-1] + mln::math::sqr(pos) * mln::opt::at(histo, p);
    avg1[p] = (0 == cnt1[p])? 0 : (sum1[p] / cnt1[p]);
    var1[p] = (0 == cnt1[p])? 0 : ((sqr1[p] / cnt1[p])-mln::math::sqr(avg1[p]));

    // Assign the statistics to the second class
    cnt2[p] = cnt2[p-1] - mln::opt::at(histo, p);;
    sum2[p] = sum2[p-1] - p * mln::opt::at(histo, p);;
    sqr2[p] = sqr2[p-1] - mln::math::sqr(p) * mln::opt::at(histo, p);;
    avg2[p] = (0 == cnt2[p])? 0 : (sum2[p] / cnt2[p]);
    var2[p] = (0 == cnt2[p])? 0 : ((sqr2[p] / cnt2[p])-mln::math::sqr(avg2[p]));

    // Lets compute the invariants
    cnt0[p] = cnt1[p] + cnt2[p];
    sum0[p] = sum1[p] + sum2[p];
    sqr0[p] = sqr1[p] + sqr2[p];
    avg0[p] = (cnt1[p] * avg1[p] + cnt2[p] * avg2[p])/cnt0[p];
    v_in[p] = (cnt1[p] * var1[p] + cnt2[p] * var2[p])/cnt0[p];
    v_bw[p] = (cnt1[p] * mln::math::sqr(avg1[p]-avg0[p]) +
	       cnt2[p] * mln::math::sqr(avg2[p]-avg0[p]))/cnt0[p];
    var0[p] = v_in[p] + v_bw[p];
  }

  // Find the threshold that minimizes the intra-class variance
  min_var   = cnt2[0]*var2[0];
  threshold = 0;

  for(short p = 0; p < 256; p++)
  {
    // Compute the intra-class variance
    v_in[p] = cnt1[p]*var1[p] + cnt2[p]*var2[p];
//    std::cout << "var intra[" << p << "]= " << v_in[p] << std::endl;

    if (min_var > v_in[p])
    {
      min_var   = v_in[p];
      threshold = p;
    }
  }

  return threshold;
}
/// /}


/// Launching part.
///
/// In this part, we have the front end that walk over the image
/// databases and start to compute information that we pass to
/// previous routines.
/// \{

#define LVL0_DESCR 0  // Number of grey available
#define HUE0_DESCR 1  // Density histogram hue analysis
#define HUE1_DESCR 2  // Millet Hue analysis
#define SAT0_DESCR 3  // Density histogram saturation analysis
#define SAT1_DESCR 4  // Millet saturation analysis
#define VAL0_DESCR 5  // Density histogram grey level analysis
#define VAL1_DESCR 6  // Millet grey level analysis
#define GMP0_DESCR 7  // PNSNR with GIMP compression
#define GMP1_DESCR 8  // PNSNR with GIMP compression
#define GMP2_DESCR 9  // PNSNR with GIMP compression
#define MGK0_DESCR 9  // PNSNR with ImageMagick compression
#define MGK1_DESCR 10 // PNSNR with ImageMagick compression
#define MGK2_DESCR 11 // PNSNR with ImageMagick compression

#define MGK_DESCR(version) (MGK0_DESCR + version)
#define GMP_DESCR(version) (GMP0_DESCR + version)

#define NB_DESCR    12  // Number of descriptors
#define NB_DATABASE 2   // Number of image databases : AFP,ICDAR
#define NB_IMAGE    110 // Number of images
#define NB_VERSION  3   // Number of compression by GIMP or ImageMagick

/// \brief Make sure that the result is between 0 and 255.
///
/// \param[in] file_name the name of the analysis files.
/// \param[in] result the values for each image and each descriptors.
/// \param[in] size the number of image by each database.
///
/// Correct the descriptors if there is overflow values. Just used in
/// debug mode.
void init_descriptors(std::string file_name[],
		      float       result[][NB_DESCR],
		      int         size[])
{
  for (int i = 0; i < NB_IMAGE; i++)
  {
    file_name[i] = std::string("PGM");

    for (int d = 0; d < NB_DESCR; d++)
      result[i][d] = (i*d) % 256;
  }

  size[0] = 62;
  size[1] = 48;
}


/// \brief Dump gnuplot file for vizualisation of results.
///
/// \param[in] file_name the name of the analysis files.
/// \param[in] result the values for each image and each descriptors.
/// \param[in] size the number of image by each database.
///
/// This routine aimed at plotting the result with one graph. Each
/// database has got its color. We can see the values used by each
/// descriptors along the image of each database.
void dump_descriptors(const std::string file_name[],
		      const float       result[][NB_DESCR],
		      const int         size[])
{
  std::cout << "#!/usr/bin/gnuplot"                           << std::endl;
  std::cout << "set terminal x11 persist 1"                   << std::endl;
  std::cout << "plot '-' using 2 with point title 'ICDAR',\\" << std::endl;
  std::cout << "     '-' using 2 with point title 'AFP'"      << std::endl;

  int num = 0;

  for (int db = 0; db < NB_DATABASE; db++)
  {
    for (int i = 0; i < size[db]; i++)
    {
      std::cout << result[num][LVL0_DESCR] << " ";
      std::cout << result[num][HUE0_DESCR] << " ";
      std::cout << result[num][HUE1_DESCR] << " ";
      std::cout << result[num][SAT0_DESCR] << " ";
      std::cout << result[num][SAT1_DESCR] << " ";
      std::cout << result[num][VAL0_DESCR] << " ";
      std::cout << result[num][VAL1_DESCR] << " ";
      std::cout << result[num][GMP0_DESCR] << " ";
      std::cout << result[num][GMP1_DESCR] << " ";
      std::cout << result[num][GMP2_DESCR] << " ";
      std::cout << result[num][MGK0_DESCR] << " ";
      std::cout << result[num][MGK1_DESCR] << " ";
      std::cout << result[num][MGK2_DESCR] << " ";
      std::cout << " # " << file_name[num] << std::endl;
      num++;
    }

    std::cout << "e"   << std::endl;
  }
}


/// \brief Compute the descriptor histograms for each database.
///
/// \param[in] result the result values of each descriptors.
/// \param[in] size the number of images by database.
/// \param[out] histo the computed histograms.
///
/// This routine compute a histograms for each database and each descriptors.
/// So we can see the distribution of descriptor values over each database.
/// Result values are transform from float to int for histograms.
void compute_histo(const float			result[][NB_DESCR],
		   const int			size[],
		   mln::image1d<float>          histo[][NB_DATABASE])
{
  for (int i = 0; i < NB_DESCR; i++)
    for (int db = 0; db < NB_DATABASE; db++)
  {
    histo[i][db].init_(mln::box1d(mln::point1d(0),mln::point1d(255)));

    mln::data::fill(histo[i][db], mln::literal::zero);
  }

  short v;
  int   num = 0;

  for (int db = 0; db < NB_DATABASE; db++)
  {
    for (int i = 0; i < size[db]; i++)
    {
 	v = (short)mln::math::floor(result[num][VAL0_DESCR]+0.4999);
 	mln::opt::at(histo[VAL0_DESCR][db],v)++;

 	v = (short)mln::math::floor(result[num][LVL0_DESCR]+0.4999);
 	mln::opt::at(histo[LVL0_DESCR][db],v)++;

 	v = (short)mln::math::floor(result[num][HUE0_DESCR]+0.4999);
 	mln::opt::at(histo[HUE0_DESCR][db],v)++;

 	v = (short)mln::math::floor(result[num][HUE1_DESCR]+0.4999);
 	mln::opt::at(histo[HUE1_DESCR][db],v)++;

 	v = (short)mln::math::floor(result[num][SAT0_DESCR]+0.4999);
 	mln::opt::at(histo[SAT0_DESCR][db],v)++;

 	v = (short)mln::math::floor(result[num][SAT1_DESCR]+0.4999);
 	mln::opt::at(histo[SAT1_DESCR][db],v)++;

 	v = (short)mln::math::floor(result[num][VAL1_DESCR]+0.4999);
 	mln::opt::at(histo[VAL1_DESCR][db],v)++;

 	v = (short)mln::math::floor(result[num][GMP0_DESCR]+0.4999);
 	mln::opt::at(histo[GMP0_DESCR][db],v)++;

 	v = (short)mln::math::floor(result[num][GMP1_DESCR]+0.4999);
 	mln::opt::at(histo[GMP1_DESCR][db],v)++;

 	v = (short)mln::math::floor(result[num][GMP2_DESCR]+0.4999);
 	mln::opt::at(histo[GMP2_DESCR][db],v)++;

 	v = (short)mln::math::floor(result[num][MGK0_DESCR]+0.4999);
 	mln::opt::at(histo[MGK0_DESCR][db],v)++;

 	v = (short)mln::math::floor(result[num][MGK1_DESCR]+0.4999);
 	mln::opt::at(histo[MGK1_DESCR][db],v)++;

 	v = (short)mln::math::floor(result[num][MGK2_DESCR]+0.4999);
 	mln::opt::at(histo[MGK2_DESCR][db],v)++;

	num++;
    }
  }
}


/// Compute the threshold for discrimination between ICDAR and AFP.
///
/// \param[in] histo the histogram for each descriptor and for each database.
/// \param[out] threshold computed values to discriminate between ICDAR/AFP.
/// \param[out] c00 say CLASS 1 but CLASS 1.
/// \param[out] c10 say CLASS 2 but CLASS 1.
/// \param[out] c01 say CLASS 1 but CLASS 2.
/// \param[out] c11 say CLASS 2 but CLASS 2.
///
/// Compute the threshold by the minimum of error of
/// classification. Population are inverted in function of average,
/// technical solution to prevent min_error crashed. The bad thing is
/// now, the class 0 can be either ICDAR of AFP, depends on descriptors.
void compute_thresholds(const mln::image1d<float>    histo[][NB_DATABASE],
			short                        threshold[],
			float                        c00[],
			float                        c10[],
			float                        c01[],
			float                        c11[])
{
  for (int i = 0; i < NB_DESCR; i++)
  {
    float avg0 = avg_histo(histo[i][0]);
    float avg1 = avg_histo(histo[i][1]);

    if (avg0 < avg1)
    {
      threshold[i] = min_error(histo[i][0], histo[i][1],
			       &c00[i], &c10[i], &c01[i], &c11[i]);
    }
    else
    {
      threshold[i] = min_error(histo[i][1], histo[i][0],
			       &c00[i], &c10[i], &c01[i], &c11[i]);
    }

    std::cerr << "  i = "   << i
	      << "; c00 = " << c00[i]
	      << "; c10 = " << c10[i]
	      << "; c01 = " << c01[i]
	      << "; c11 = " << c11[i]
	      << "; threshold " << threshold[i]
	      << std::endl;

  }
}


/// \brief Walk over the database directories.
///
/// \param[out] file_name the array of the image name.
/// \param[out] result the descriptors values for each image.
/// \param[out] size the number of image for each database.
///
/// This is the front end part to access to the database directories. We use
/// boost library to do it. First we explore ICDAR database en then the AFP one.
/// For each valid image, we compute all the descriptors. Histograms are
/// computed at this level because we can do that thing one time for every
/// density descriptors. Every parameters are updated to retrurn the values.
void compute_descriptors(std::string file_name[],
			 float       result[][NB_DESCR],
			 int         size[])
{
  typedef boost::filesystem::path                   t_path;
  typedef boost::filesystem::directory_iterator     t_iter_path;
  typedef mln::image1d<unsigned>                    t_histo;
  typedef mln::value::rgb8                          t_rgb8;
  typedef mln::value::int_u8                        t_int_u8;
  typedef mln::image2d<t_int_u8>                    t_map;
  typedef mln::image2d<t_rgb8>                      t_input;
  typedef mln::fun::v2v::rgb_to_hue_map<8>          t_rgb_2_hue;
  typedef mln::fun::v2v::rgb_to_saturation_map<8>   t_rgb_2_sat;
  typedef mln::fun::v2v::rgb_to_value_map<8>        t_rgb_2_val;
  typedef mln::fun::v2v::component<t_rgb8,0>        t_rgb_2_red;
  typedef mln::fun::v2v::component<t_rgb8,1>        t_rgb_2_green;
  typedef mln::fun::v2v::component<t_rgb8,2>        t_rgb_2_blue;
  typedef mln::accu::meta::stat::histo1d            t_accu_histo;


  t_path img_path[2]    = { ICDAR_20P_INPUT_IMG_PATH, AFP_PPM_IMG_PATH};
  t_path mgk_path[3][2] = {{ICDAR_20P_MGK30_IMG_PATH, AFP_MGK30_IMG_PATH},
			   {ICDAR_20P_MGK20_IMG_PATH, AFP_MGK20_IMG_PATH},
			   {ICDAR_20P_MGK10_IMG_PATH, AFP_MGK10_IMG_PATH}};
  t_path gmp_path[3][2] = {{ICDAR_20P_GMP30_IMG_PATH, AFP_GMP30_IMG_PATH},
			   {ICDAR_20P_GMP20_IMG_PATH, AFP_GMP20_IMG_PATH},
			   {ICDAR_20P_GMP10_IMG_PATH, AFP_GMP10_IMG_PATH}};

  int    num            = 0;
  int	 cnt	        = 0;

  for (int db = 0; db < NB_DATABASE; db++)
  {
    if (boost::filesystem::exists(img_path[db]) &&
	boost::filesystem::is_directory(img_path[db]))
    {
      boost::filesystem::system_complete(img_path[db]);

      const t_iter_path end_iter;

      cnt = 0;

      for (t_iter_path dir_iter(img_path[db]); end_iter != dir_iter; ++dir_iter)
      {
	t_path  img_file  = dir_iter->path().leaf();
	t_path  dir_file  = dir_iter->path();
	t_input img_input;

	mln::io::ppm::load(img_input, dir_file.string().c_str());

	t_map   h_img_map = mln::data::transform(img_input, t_rgb_2_hue());
	t_map   s_img_map = mln::data::transform(img_input, t_rgb_2_sat());
	t_map   v_img_map = mln::data::transform(img_input, t_rgb_2_val());
	t_map   r_img_map = mln::data::transform(img_input, t_rgb_2_red());
	t_map   g_img_map = mln::data::transform(img_input, t_rgb_2_green());
	t_map   b_img_map = mln::data::transform(img_input, t_rgb_2_blue());
	t_histo h_img_hst = mln::data::compute(t_accu_histo(), h_img_map);
	t_histo s_img_hst = mln::data::compute(t_accu_histo(), s_img_map);
	t_histo v_img_hst = mln::data::compute(t_accu_histo(), v_img_map);
	t_histo r_img_hst = mln::data::compute(t_accu_histo(), r_img_map);
	t_histo g_img_hst = mln::data::compute(t_accu_histo(), g_img_map);
	t_histo b_img_hst = mln::data::compute(t_accu_histo(), b_img_map);

	std::cerr << dir_iter->path() << std::endl;

	file_name[num] = img_file.string();

	// descriptors
	result[num][LVL0_DESCR] = lvl0_descriptor(v_img_hst);
	result[num][HUE0_DESCR] = hue0_descriptor(h_img_hst);
	result[num][HUE1_DESCR] = hue1_descriptor(h_img_hst,20);
	result[num][SAT0_DESCR] = sat0_descriptor(s_img_hst);
	result[num][SAT1_DESCR] = sat1_descriptor(s_img_hst,50);
	result[num][VAL0_DESCR] = val0_descriptor(v_img_hst);
	//result[num][VAL1_DESCR] = val1_descriptor(v_img_hst, 15);
	result[num][VAL1_DESCR] = 0;

	// for gimp and magick
	for (int v = 0; v < NB_VERSION; v++)
	{
	  if (boost::filesystem::exists(mgk_path[v][db]) &&
	      boost::filesystem::exists(gmp_path[v][db]) &&
	      boost::filesystem::is_directory(mgk_path[v][db]) &&
	      boost::filesystem::is_directory(gmp_path[v][db]))
	  {
	    t_path  mgk_file  = mgk_path[v][db] / img_file;
	    t_path  gmp_file  = gmp_path[v][db] / img_file;
	    t_input gmp_input;

	    mln::io::ppm::load(gmp_input, gmp_file.string().c_str());

	    t_map   r_gmp_map = mln::data::transform(gmp_input,t_rgb_2_red());
	    t_map   g_gmp_map = mln::data::transform(gmp_input,t_rgb_2_green());
	    t_map   b_gmp_map = mln::data::transform(gmp_input,t_rgb_2_blue());

	    result[num][GMP_DESCR(v)]= err_descriptor(r_img_map,
						      g_img_map,
						      b_img_map,
						      r_gmp_map,
						      g_gmp_map,
						      b_gmp_map);

	    t_input mgk_input;

	    mln::io::ppm::load(mgk_input, mgk_file.string().c_str());

	    t_map   r_mgk_map = mln::data::transform(mgk_input,t_rgb_2_red());
	    t_map   g_mgk_map = mln::data::transform(mgk_input,t_rgb_2_green());
	    t_map   b_mgk_map = mln::data::transform(mgk_input,t_rgb_2_blue());

	    result[num][MGK_DESCR(v)]= err_descriptor(r_img_map,
						      g_img_map,
						      b_img_map,
						      r_mgk_map,
						      g_mgk_map,
						      b_mgk_map);
	  }
	}

	num++;
	cnt++;
      }
    }

    size[db] = cnt;
  }
}

/// \brief Just for debugging purpose and tests.
int main2()
{
  typedef mln::image1d<unsigned>                    t_histo;
  typedef mln::value::rgb8                          t_rgb8;
  typedef mln::value::int_u8                        t_int_u8;
  typedef mln::image2d<t_int_u8>                    t_map;
  typedef mln::image2d<t_rgb8>                      t_input;
  typedef mln::fun::v2v::rgb_to_hue_map<8>          t_rgb_2_hue;
  typedef mln::fun::v2v::rgb_to_saturation_map<8>   t_rgb_2_sat;
  typedef mln::fun::v2v::rgb_to_value_map<8>        t_rgb_2_val;
  typedef mln::fun::v2v::component<t_rgb8,0>        t_rgb_2_red;
  typedef mln::fun::v2v::component<t_rgb8,1>        t_rgb_2_green;
  typedef mln::fun::v2v::component<t_rgb8,2>        t_rgb_2_blue;
  typedef mln::accu::meta::stat::histo1d            t_accu_histo;

  t_input img_input;

  mln::io::ppm::load(img_input, ICDAR_20P_INPUT_IMG_PATH"/mp00032c_20p.ppm");
  //mln::io::ppm::load(img_input, AFP_PPM_IMG_PATH"/000_Del218430.ppm");



  t_map   h_img_map = mln::data::transform(img_input, t_rgb_2_hue());
  t_map   s_img_map = mln::data::transform(img_input, t_rgb_2_sat());
  t_map   v_img_map = mln::data::transform(img_input, t_rgb_2_val());
  t_map   r_img_map = mln::data::transform(img_input, t_rgb_2_red());
  t_map   g_img_map = mln::data::transform(img_input, t_rgb_2_green());
  t_map   b_img_map = mln::data::transform(img_input, t_rgb_2_blue());
  t_histo h_img_hst = mln::data::compute(t_accu_histo(), h_img_map);
  t_histo s_img_hst = mln::data::compute(t_accu_histo(), s_img_map);
  t_histo v_img_hst = mln::data::compute(t_accu_histo(), v_img_map);
  t_histo r_img_hst = mln::data::compute(t_accu_histo(), r_img_map);
  t_histo g_img_hst = mln::data::compute(t_accu_histo(), g_img_map);
  t_histo b_img_hst = mln::data::compute(t_accu_histo(), b_img_map);


  std::cout << "sat2 : " << sat0_descriptor(s_img_hst) << std::endl;

  return 0;
}


/// \brief Main entry.
///
/// This is the front end for every routines.
/// - compute descriptors.
/// - dump descriptors.
/// - compute histograms.
/// - compute thresholds.
/// - save results.
///
/// Global data used:
/// - file_name : all the initial images name, mixed every image database.
/// - result : all the result values for every descriptors and for every images.
/// - histo : the histograms ny database and by descriptors.
/// - size : the number of image by database.
/// - threshold : the computed histogram separation on each descriptors.
/// - cxx : the count of well/bad classified images function of threshold info.
int main()
{
  std::string			file_name[NB_IMAGE];
  float				result[NB_IMAGE][NB_DESCR];
  int				size[NB_DATABASE];
  mln::image1d<float>		histo[NB_DESCR][NB_DATABASE];
  short				threshold[NB_DESCR];
  float				c00[NB_DESCR];
  float				c10[NB_DESCR];
  float				c01[NB_DESCR];
  float				c11[NB_DESCR];

  std::cerr << "DESCRIPTORS" << std::endl;
  compute_descriptors(file_name,result,size);
//   std::cout << "DUMPING" << std::endl;
//   init_descriptors(file_name,result,size);
  dump_descriptors(file_name,result,size);
  std::cerr << "HISTO" << std::endl;
  compute_histo(result,size,histo);
  std::cerr << "THRESHOLD" << std::endl;
  compute_thresholds(histo,threshold,c00,c10,c01,c11);

  mln::io::plot::save_image_sh(histo[LVL0_DESCR][0], "lvl0_histo1.sh");
  mln::io::plot::save_image_sh(histo[HUE0_DESCR][0], "hue0_histo1.sh");
  mln::io::plot::save_image_sh(histo[HUE1_DESCR][0], "hue1_histo1.sh");
  mln::io::plot::save_image_sh(histo[SAT0_DESCR][0], "sat0_histo1.sh");
  mln::io::plot::save_image_sh(histo[SAT1_DESCR][0], "sat1_histo1.sh");
  mln::io::plot::save_image_sh(histo[VAL0_DESCR][0], "val0_histo1.sh");
  mln::io::plot::save_image_sh(histo[VAL1_DESCR][0], "val1_histo1.sh");
  mln::io::plot::save_image_sh(histo[GMP0_DESCR][0], "gmp0_histo1.sh");
  mln::io::plot::save_image_sh(histo[GMP1_DESCR][0], "gmp1_histo1.sh");
  mln::io::plot::save_image_sh(histo[GMP2_DESCR][0], "gmp2_histo1.sh");
  mln::io::plot::save_image_sh(histo[MGK0_DESCR][0], "mgk0_histo1.sh");
  mln::io::plot::save_image_sh(histo[MGK1_DESCR][0], "mgk1_histo1.sh");
  mln::io::plot::save_image_sh(histo[MGK2_DESCR][0], "mgk2_histo1.sh");

  mln::io::plot::save_image_sh(histo[LVL0_DESCR][1], "lvl0_histo2.sh");
  mln::io::plot::save_image_sh(histo[HUE0_DESCR][1], "hue0_histo2.sh");
  mln::io::plot::save_image_sh(histo[HUE1_DESCR][1], "hue1_histo2.sh");
  mln::io::plot::save_image_sh(histo[SAT0_DESCR][1], "sat0_histo2.sh");
  mln::io::plot::save_image_sh(histo[SAT1_DESCR][1], "sat1_histo2.sh");
  mln::io::plot::save_image_sh(histo[VAL0_DESCR][1], "val0_histo2.sh");
  mln::io::plot::save_image_sh(histo[VAL1_DESCR][1], "val1_histo2.sh");
  mln::io::plot::save_image_sh(histo[GMP0_DESCR][1], "gmp0_histo2.sh");
  mln::io::plot::save_image_sh(histo[GMP1_DESCR][1], "gmp1_histo2.sh");
  mln::io::plot::save_image_sh(histo[GMP2_DESCR][1], "gmp2_histo2.sh");
  mln::io::plot::save_image_sh(histo[MGK0_DESCR][1], "mgk0_histo2.sh");
  mln::io::plot::save_image_sh(histo[MGK1_DESCR][1], "mgk1_histo2.sh");
  mln::io::plot::save_image_sh(histo[MGK2_DESCR][1], "mgk2_histo2.sh");

  return 0;
}
/// \}

