// BENCH TEST CF MILLET 2008

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


//============================================================================//
// MILLET HUE DESCRIPTOR
//
// This test is used for discrimination between black and white pictures and
// color ones. Some colored Black and white pictures have their energy near
// the peak.
//============================================================================//

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


//============================================================================//
// MILLET SATURATION DESCRIPTOR
//
// This test is used for discrimination between black and white pictures and
// color ones. Black and white pictures have their energy in the low saturation
// band.
//============================================================================//

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
  color       = label_hue(peak);}


//============================================================================//
// MILLET DESCRIPTOR
//
// This test aims at compute the number of grey levels. Photographies tends to
// use all the levels or many of them.
//============================================================================//

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


float lvl0_descriptor(mln::image1d<unsigned> histo)
{
  float                            result;

  // FIXME 255
  result      = 255-count_null_frequency_histo(histo);

  return result;
}

//============================================================================//
// DENSITY DESCRIPTOR
//
//
//============================================================================//

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

//============================================================================//
// MILLET DESCRIPTOR
//
// This test aims at compute some deviation on the peak of the histogram of
// the image. Large deviations mean lots of graduation in colors (such as
// photos) and small ones mean something like cartoon.
//============================================================================//


// calcul de contribution
float r(short p, unsigned histo_p, short x, unsigned histo_x)
{
  float result = mln::math::sqr(((float)histo_x / histo_p) * (x-p));

  return result;
}

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

float var0_descriptor(mln::image1d<unsigned> histo, const short threshold)
{
  typedef mln::fun::v2v::rgb_to_value_map<8>   t_rgb_to_value_map;

  float                            result;
  short                            peak;

  peak        = peak_histo(histo);
  result      = stddev2(histo, peak, threshold);

  return result;
}


//============================================================================//
// ERROR DESCRIPTOR
//============================================================================//


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

// Le PNSNR semble offrir plus d'espace pour la discrimination
// Si les images sont identiques ==> PNSNR = +inf
// Si les images sont très différentes ==> PNSNR = 0
  // FIXME METTRE UN MAX A 255

  return error;
}



//============================================================================//
//         CLASSIFICATION DE FISHER EN 2 CLASSES SUR UN HISTO 1D
//============================================================================//

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

//============================================================================//
//          CLASSIFIEUR
//============================================================================//


// Linear discriminant analysis in 1d
// With same variance, threshold = (m1+m2)/2
// With different variance, (m1*sqrt(v1)+m2*sqrt(v2))/(sqrt(v1)+sqrt(v2))
float threshold_histo(float avg1, float var1, float avg2, float var2)
{
  float sigma1    = mln::math::sqrt(var1);
  float sigma2    = mln::math::sqrt(var2);
  float threshold = (avg1*sigma1+avg2*sigma2)/(sigma1+sigma2);

  return threshold;
}

float threshold3_histo(float avg1, float var1, float avg2, float var2)
{
  float threshold = (avg1*var1+avg2*var2)/(var1+var2);

  return threshold;
}


// if gaussian without the same variance
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

// return the threshold
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




//============================================================================//
//                         MAIN
//============================================================================//





#define LVL0_DESCR 0
#define HUE0_DESCR 1
#define HUE1_DESCR 2
#define SAT0_DESCR 3
#define SAT1_DESCR 4
#define VAL0_DESCR 5
#define VAL1_DESCR 6
#define GMP0_DESCR 7
#define GMP1_DESCR 8
#define GMP2_DESCR 9
#define MGK0_DESCR 9
#define MGK1_DESCR 10
#define MGK2_DESCR 11

#define MGK_DESCR(version) (MGK0_DESCR + version)
#define GMP_DESCR(version) (GMP0_DESCR + version)

#define NB_DESCR    12
#define NB_DATABASE 2
#define NB_IMAGE    110
#define NB_VERSION  3


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
 	v = (short)mln::math::floor(result[num][VAR0_DESCR]+0.4999);
 	mln::opt::at(histo[VAR0_DESCR][db],v)++;

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

 	v = (short)mln::math::floor(result[num][VAL0_DESCR]+0.4999);
 	mln::opt::at(histo[VAL0_DESCR][db],v)++;

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
	//result[num][VAL1_DESCR] = var0_descriptor(v_img_hst, 15);
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

