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
/// This file is a prelude just before using the bench source. It puts
/// the light on the Jonathan's idea which wants to compare ICDAR and
/// AFP image database with difference between original image and
/// color reducted one. The key point is to say that when a color
/// image is degraded, the visual quality fall quickly in function of
/// the number of colors left. A contrario, when an image is black and
/// white, this kind of degradation doesn't change the original visual
/// quality. The PNSNR detector is build and learning threshold for
/// automatic classification is done (four algorithms are tested).

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

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/alias/point1d.hh>
#include <mln/core/alias/box1d.hh>

#include <mln/data/transform.hh>
#include <mln/data/compute.hh>
#include <mln/data/stretch.hh>
#include <mln/data/fill.hh>

#include <mln/fun/v2v/component.hh>

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

/// Work on histogram.
/// \brief Different routines are presented for computing mean, variance ...
/// \{

/// \brief Counting the number of pixels in the histogram.
/// \param[in] histo_ the image which represents the histogram.
///
/// \return the number of pixels in the histogram.
///
/// Such routine is a generic and is generally applied on a portion of
/// the real domain.
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

/// \brief Summing the values ponderated by their occurences.
/// \param[in] histo_ the image which represents the histogram.
///
/// \return the sum ponderated of values by their occurences.
///
/// This method is not a production routine, it's just a test to debug
/// histogram problem. When we compute something over the histogram,
/// we need to take care of the intermediate type that contains the
/// temporary result. If not the case, the type used is the one of the
/// value of the histogram and we can go to some overflow
/// problems. The trick is to store the value returned by the iterator
/// in some temporary computing type (look at pos in the code).
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

/// \brief Summing the values ponderated by their occurences.
/// \param[in] histo_ the image which represents the histogram.
///
/// \return the sum ponderated of values by their occurences.
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

/// \brief Computing the variance
/// \param[in] histo_ the image which represents the histogram.
///
/// \return the variance of the histogram.
///
/// This computing is safe because the quantity of information stored
/// in sum is low. This formulae doesn't show the overflow problem.
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

/// \brief Computing the variance
/// \param[in] histo_ the image which represents the histogram.
///
/// \return the variance of the histogram.
///
/// This is the debugging code to observe overflow problem.
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

/// \brief Computing the variance
/// \param[in] histo_ the image which represents the histogram.
///
/// \return the variance of the histogram.
///
/// My standard code for the variance.
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
/// \}


/// Classify.
/// \brief Classification routines that compute threshold.
/// \{

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

/// \brief Compute the other proportion of histogram.
///
/// \param[in] histo_ the histogram.
///
/// \return the ponderated sum of the sqrt values.
///
/// This is a test. Don't take this routine in production.
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
		const mln::image1d<float> histo_grp2)
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

    std::cout << " p   = " << p
	      << ";c00 = " << c00[p]
	      << ";c10 = " << c10[p]
	      << ";c01 = " << c01[p]
	      << ";c11 = " << c11[p]
	      << std::endl;
//    std::cout << "err[" << p <<  "] = " << err[p] << std::endl;

    if (error > err[p])
    {
      error     = err[p];
      threshold = p;
    }
  }

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
/// \}

/// \brief ERROR (MSE, PNSNR) compression p278 Handbook Color.
/// \param[in] original the original image.
/// \param[in] reduced the image with a reduced number of colors.
///
/// \return the PNSNR.
///
/// Compute the PNSNR. First compute the square difference in each
/// channel. Then compute the mean of those square differences and get
/// the mean over the three channels. Then, get the sqrt of that to
/// have a distance. The PNSNR is a logarithmic quantity of that distance.
float error_test(const std::string original,
		 const std::string reduced)

{

  typedef mln::value::rgb8                     t_rgb8;
  typedef mln::value::int_u8                   t_int_u8;
  typedef mln::fun::v2v::component<t_rgb8,0>   t_component_red;
  typedef mln::fun::v2v::component<t_rgb8,1>   t_component_green;
  typedef mln::fun::v2v::component<t_rgb8,2>   t_component_blue;
  typedef mln::accu::meta::stat::mean          t_mean;
  typedef mln::accu::meta::stat::histo1d       t_histo;
  typedef mln::image2d<t_int_u8>               t_img;
  typedef mln_trait_op_minus_(t_img,t_img)     t_minus;
  typedef mln_trait_op_times_(t_minus,t_minus) t_times;

  mln::image2d<mln::value::rgb8>   original_rgb8;
  mln::image2d<mln::value::rgb8>   reduced_rgb8;

  mln::image2d<mln::value::int_u8> original_red;
  mln::image2d<mln::value::int_u8> original_green;
  mln::image2d<mln::value::int_u8> original_blue;

  mln::image2d<mln::value::int_u8> reduced_red;
  mln::image2d<mln::value::int_u8> reduced_green;
  mln::image2d<mln::value::int_u8> reduced_blue;

//   mln::image2d<mln::value::int_u8> map_red;
//   mln::image2d<mln::value::int_u8> map_green;
//   mln::image2d<mln::value::int_u8> map_blue;

//   mln::image1d<unsigned>           histo_red;
//   mln::image1d<unsigned>           histo_green;
//   mln::image1d<unsigned>           histo_blue;

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


  mln::io::ppm::load(original_rgb8, original.c_str());
  mln::io::ppm::load(reduced_rgb8, reduced.c_str());

  original_red   = mln::data::transform(original_rgb8, t_component_red());
  original_green = mln::data::transform(original_rgb8, t_component_green());
  original_blue  = mln::data::transform(original_rgb8, t_component_blue());

  reduced_red    = mln::data::transform(reduced_rgb8, t_component_red());
  reduced_green  = mln::data::transform(reduced_rgb8, t_component_green());
  reduced_blue   = mln::data::transform(reduced_rgb8, t_component_blue());

  minus_red      = (original_red - reduced_red);
  times_red      = minus_red * minus_red;

  minus_green    = (original_green - reduced_green);
  times_green    = minus_green * minus_green;

  minus_blue     = (original_blue - reduced_blue);
  times_blue     = minus_blue * minus_blue;

  error_red      = mln::data::compute(t_mean(), times_red);
  error_green    = mln::data::compute(t_mean(), times_green);
  error_blue     = mln::data::compute(t_mean(), times_blue);

//   map_red	 = mln::data::stretch(t_int_u8(), times_red);
//   map_green	 = mln::data::stretch(t_int_u8(), times_blue);
//   map_blue	 = mln::data::stretch(t_int_u8(), times_green);

//   histo_red      = mln::data::compute(t_histo(), map_red);
//   histo_green    = mln::data::compute(t_histo(), map_green);
//   histo_blue     = mln::data::compute(t_histo(), map_blue);

//   mln::io::plot::save_image_sh(histo_red,  "histo_red.sh");
//   mln::io::plot::save_image_sh(histo_green,"histo_green.sh");
//   mln::io::plot::save_image_sh(histo_blue, "histo_blue.sh");

//   mln::io::pgm::save(map_red,  "red.pgm");
//   mln::io::pgm::save(map_green,"green.pgm");
//   mln::io::pgm::save(map_blue, "blue.pgm");

  error          = (error_red + error_green + error_blue)/3.0;
  error          = mln::math::sqrt(error);
  error          = 20 * log(255/error);

// Le PNSNR semble offrir plus d'espace pour la discrimination
// Si les images sont identiques ==> PNSNR = +inf
// Si les images sont très différentes ==> PNSNR = 0

  return error;
}


/// \brief The main entry.
///
/// This is the front end for error classification. This is the big
/// loop where we walk other image data bases. Computation of
/// automatic thresholds are called from here.
int main()
{
  typedef boost::filesystem::path                   t_path;
  typedef boost::filesystem::directory_iterator     t_iter_path;

  mln::image1d<float> histo(256);
  mln::image1d<float> histo_grp[2]; // histo by group

  histo_grp[0].init_(mln::box1d(mln::point1d(0),mln::point1d(255)));
  histo_grp[1].init_(mln::box1d(mln::point1d(0),mln::point1d(255)));

  mln::data::fill(histo, mln::literal::zero);
  mln::data::fill(histo_grp[0], mln::literal::zero);
  mln::data::fill(histo_grp[1], mln::literal::zero);

  t_path original_path[] = {ICDAR_20P_INPUT_IMG_PATH,
			    AFP_PPM_IMG_PATH};

//   t_path reduced1_path[] = {ICDAR_20P_MGK30_IMG_PATH,
// 			    AFP_MGK30_IMG_PATH};

//   t_path reduced1_path[] = {ICDAR_20P_MGK20_IMG_PATH,
// 			    AFP_MGK20_IMG_PATH};

  t_path reduced1_path[] = {ICDAR_20P_MGK10_IMG_PATH,
			    AFP_MGK10_IMG_PATH};

//   t_path reduced2_path[] = {ICDAR_20P_GMP30_IMG_PATH,
// 			    AFP_GMP30_IMG_PATH};

//   t_path reduced2_path[] = {ICDAR_20P_GMP20_IMG_PATH,
// 			    AFP_GMP20_IMG_PATH};

  t_path reduced2_path[] = {ICDAR_20P_GMP10_IMG_PATH,
			    AFP_GMP10_IMG_PATH};


  std::cout << "#!/usr/bin/gnuplot"                       << std::endl;
  std::cout << "set terminal x11 persist 1"               << std::endl;
  std::cout << "ERROR"                                    << std::endl;
  std::cout << "plot '-' using 1 with point notitle,\\"   << std::endl;
  std::cout << "     '-' using 1 with point notitle"      << std::endl;

  for (int i = 0; i < 2; i++)
  {
    if (boost::filesystem::exists(original_path[i]) &&
	boost::filesystem::exists(reduced1_path[i]) &&
	boost::filesystem::exists(reduced2_path[i]) &&
	boost::filesystem::is_directory(original_path[i]) &&
	boost::filesystem::is_directory(reduced1_path[i]) &&
	boost::filesystem::is_directory(reduced2_path[i]))
    {
      boost::filesystem::system_complete(original_path[i]);
      const t_iter_path end_iter;
      float             error1 = 0.0;
      float             error2 = 0.0;
      t_path            leaf;
      t_path            reduced1_file;
      t_path            reduced2_file;

      for (t_iter_path dir_iter(original_path[i]);end_iter!=dir_iter;++dir_iter)
      {
	leaf          = dir_iter->path().leaf();
	reduced1_file = reduced1_path[i] / leaf;
	reduced2_file = reduced2_path[i] / leaf;

	error1 = error_test(dir_iter->path().string(), reduced1_file.string());
	error2 = error_test(dir_iter->path().string(), reduced2_file.string());

 	float a1 = 1;
 	short v1 = (short)mln::math::floor(error2+0.4999);
 	mln::opt::at(histo,v1) += a1;
 	mln::opt::at(histo_grp[i],v1) += a1;

// 	float a1 = error2 - mln::math::floor(error2);
// 	float a2 = mln::math::ceil(error2) - error2;
// 	short v1 = (short)mln::math::floor(error2);
// 	short v2 = (short)mln::math::ceil(error2);
// 	mln::opt::at(histo,v1) += a1;
// 	mln::opt::at(histo,v2) += a2;
// 	mln::opt::at(histo_grp[i],v1) += a1;
// 	mln::opt::at(histo_grp[i],v2) += a2;

	std::cout << error1  << " ";
	std::cout << error2  << " ";
	std::cout << "# "  << dir_iter->path().leaf() << std::endl;
      }
      std::cout << "e"   << std::endl;
    }
  }

  mln::io::plot::save_image_sh(histo, "histo.sh");
  mln::io::plot::save_image_sh(histo_grp[1], "histo_grp1.sh");
  mln::io::plot::save_image_sh(histo_grp[0], "histo_grp2.sh");

  float threshold  = fisher_analysis(histo);
  float threshold2 = threshold_histo(avg_histo(histo_grp[1]),
				     var_histo(histo_grp[1]),
				     avg_histo(histo_grp[0]),
				     var_histo(histo_grp[0]));
  float threshold3 = threshold2_histo(avg_histo(histo_grp[1]),
				     var_histo(histo_grp[1]),
				     avg_histo(histo_grp[0]),
				     var_histo(histo_grp[0]));
  float threshold4 = min_error(histo_grp[1],histo_grp[0]);

  std::cout << "threshold  = " << threshold   << std::endl;
  std::cout << "threshold2 = " << threshold2  << std::endl;
  std::cout << "threshold3 = " << threshold3  << std::endl;
  std::cout << "threshold4 = " << threshold4  << std::endl;
  std::cout << "avg_grp1   = " << avg_histo(histo_grp[1])  << std::endl;
  std::cout << "avg_grp2   = " << avg_histo(histo_grp[0])  << std::endl;

  // compute the classification matrix
  // for each sub population

  float c00 = cnt_histo(histo_grp[1] | mln::box1d(mln::point1d(0),
						  mln::point1d(threshold)));

  float c10 = cnt_histo(histo_grp[1] | mln::box1d(mln::point1d(threshold+1),
						  mln::point1d(255)));

  float c01 = cnt_histo(histo_grp[0] | mln::box1d(mln::point1d(0),
						  mln::point1d(threshold)));

  float c11 = cnt_histo(histo_grp[0] | mln::box1d(mln::point1d(threshold+1),
						  mln::point1d(255)));


  std::cout << "pop0 = " << cnt_histo(histo_grp[1]) << std::endl;
  std::cout << "pop1 = " << cnt_histo(histo_grp[0]) << std::endl;
  std::cout << std::endl;

  std::cout << "c00 = " << c00 << std::endl;
  std::cout << "c10 = " << c10 << std::endl;
  std::cout << "c01 = " << c01 << std::endl;
  std::cout << "c11 = " << c11 << std::endl;

  return 0;
}
