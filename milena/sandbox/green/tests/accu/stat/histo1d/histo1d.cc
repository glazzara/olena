/// TEST HISTO1D

#include <mln/img_path.hh>

#include <mln/io/plot/save_histo_sh.hh>
#include <mln/accu/stat/histo1d.hh>
#include <mln/fun/v2v/int_u16_to_int_u14.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/plot/save.hh>

#include <mln/data/compute.hh>
#include <mln/data/transform.hh>
#include <mln/data/fill.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/int_u.hh>
#include <mln/value/label_8.hh>
#include <mln/value/rgb8.hh>

#include <mln/core/alias/neighb1d.hh>
#include <mln/core/alias/box1d.hh>
#include <mln/core/alias/point1d.hh>

#include <mln/core/routine/initialize.hh>
#include <mln/core/contract.hh>
#include <mln/core/grids.hh>

#include <mln/accu/math/sum.hh>
#include <mln/accu/math/count.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/accu/stat/variance.hh>

#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/morpho/elementary/closing.hh>

#include <mln/linear/convolve.hh>
#include <mln/linear/gaussian.hh>
#include <mln/labeling/regional_maxima.hh>
#include <mln/labeling/colorize.hh>
#include <mln/labeling/mean_values.hh>

#include <mln/make/w_window1d.hh>

#include <mln/math/sqr.hh>
#include <mln/math/pi.hh>
#include <mln/math/abs.hh>

#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/value.hh>

//------------------------------------------------------------------------------
// TEST IN 8 BITS
//------------------------------------------------------------------------------


void test_8bits_operator_equal()
{
  using namespace mln::accu::stat;

  typedef mln::value::int_u8       int_u8;
  mln::accu::stat::histo1d<int_u8> histo1;
  mln::accu::stat::histo1d<int_u8> histo2;
  int_u8                           val = 3;

  histo1.init();
  histo2.init();

  mln_assertion(histo1 == histo2);

  histo1.take(val);

  mln_assertion(histo1 != histo2);

  histo2.take(val);

  mln_assertion(histo1 == histo2);

  std::cout << "(08 bits) histo == histo       : ok" << std::endl;
}

void test_8bits_instantiation_without_argument()
{
  typedef mln::value::int_u8               int_u8;
  const   mln::accu::stat::histo1d<int_u8> histo;
  const   mln::image1d<unsigned>&          res = histo.to_result();
  const   mln::point1d&                    min = mln::point1d(mln_min(int_u8));
  const   mln::point1d&                    max = mln::point1d(mln_max(int_u8));
  const   mln::box1d&                      ref = mln::box1d(min, max);

  mln_assertion(ref == res.domain());
  mln_assertion(res.is_valid());

  std::cout << "(08 bits) histo<T> histo       : ok" << std::endl;
}


void test_8bits_initialization()
{
  typedef mln::value::int_u8       int_u8;
  mln::accu::stat::histo1d<int_u8> histo;
  mln::image1d<unsigned>           img_res = histo.to_result();
  mln::image1d<unsigned>           img_ref;

  mln::initialize(img_ref, img_res);
  mln::data::fill(img_ref, 0);
  histo.init();

  unsigned res = mln::data::compute(mln::accu::math::sum<int_u8>(), img_res);
  unsigned ref = mln::data::compute(mln::accu::math::sum<int_u8>(), img_ref);
  
  mln_assertion(ref == res);

  std::cout << "(08 bits) histo.init()         : ok" << std::endl;
}


void test_8bits_take_argument()
{
  typedef mln::value::int_u8       int_u8;
  mln::accu::stat::histo1d<int_u8> histo1;
  mln::accu::stat::histo1d<int_u8> histo2;
  int_u8                           val = 3;

  histo1.init();
  histo2.init();
  histo1.take(val);

  const mln::image1d<unsigned>     img1 = histo1.to_result();
  const mln::image1d<unsigned>     img2 = histo2.to_result();

  const unsigned res = mln::data::compute(mln::accu::math::sum<int_u8>(), img1);
  const unsigned ref = mln::data::compute(mln::accu::math::sum<int_u8>(), img2);

  mln_assertion(ref == res-1);
  mln_assertion(1 == img1(mln::point1d(val)));

  std::cout << "(08 bits) histo.take(argument) : ok" << std::endl;
}


void test_8bits_take_other()
{
  typedef mln::value::int_u8       int_u8;
  mln::accu::stat::histo1d<int_u8> histo1;
  mln::accu::stat::histo1d<int_u8> histo2;
  mln::accu::stat::histo1d<int_u8> histo3;
  int_u8                           val = 3;

  histo1.init();
  histo2.init();
  histo3.init();

  histo1.take(val);
  histo1.take(val);
  histo3.take(val);
  histo2.take(val);
  histo2.take(histo3);

  mln_assertion(histo1 == histo2);

  std::cout << "(08 bits) histo.take(other)    : ok" << std::endl;
}

double count_histo(mln::image1d<unsigned> img)
{
  mln_precondition(img.is_valid());
  
  double                             result = 0.0;
  mln_piter_(mln::image1d<unsigned>) p(img.domain());

  for_all(p)
    result += img(p);

  return result;
}

double mean_histo(mln::image1d<unsigned> img)
{
  mln_precondition(img.is_valid());
  
  double                             count  = 0.0;
  double                             sum    = 0.0;
  double                             result = 0.0;
  mln_piter_(mln::image1d<unsigned>) p(img.domain());

  for_all(p)
  {
    count += img(p);
    sum   += p[0] * img(p);
  }

  result = sum / count;

  return result;
}


double var_histo(mln::image1d<unsigned> img)
{
  mln_precondition(img.is_valid());
  
  double                             count  = 0.0;
  double                             sum    = 0.0;
  double                             sum2   = 0.0;
  double                             result = 0.0;
  mln_piter_(mln::image1d<unsigned>) p(img.domain());

  for_all(p)
  {
    count += img(p);
    sum   += p[0] * img(p);
    sum2  += p[0] * p[0] * img(p);
  }

  result = sum2 / count - (sum / count) * (sum / count);

  return result;
}

double var_histo2(mln::image1d<unsigned> img)
{
  mln_precondition(img.is_valid());
  
  double                             count  = count_histo(img);
  double                             mean   = mean_histo(img);
  double                             result = 0.0;
  mln_piter_(mln::image1d<unsigned>) p(img.domain());

  for_all(p)
  {
    result += img(p) * (p[0] - mean) * (p[0] - mean);
  }

  result /= count;

  return result;
}

void test_8bits_integration()
{
  typedef mln::value::int_u8                int_u8;
  typedef mln::accu::math::count<double>    count;
  typedef mln::accu::math::sum<double>      sum;
  typedef mln::accu::stat::mean<double>     mean;
  typedef mln::accu::stat::variance<double> variance;

  mln::image2d<int_u8>       img_ref;
  mln::image1d<unsigned>     img_res;

  mln::io::pgm::load(img_ref, OLENA_IMG_PATH"/lena.pgm");

  const double count_ref = mln::data::compute(count(),    img_ref);
  const double mean_ref  = mln::data::compute(mean(),     img_ref);
  const double var_ref   = mln::data::compute(variance(), img_ref);

  img_res = mln::data::compute(mln::accu::meta::stat::histo1d(), img_ref);
  //  img_res = mln::data::compute(mln::accu::stat::histo1d<int_u8>(), img_ref);

  const double count_res = count_histo(img_res);
  const double mean_res  = mean_histo(img_res);
  const double var_res   = var_histo(img_res);
  
  mln_assertion(count_ref == count_res);
  mln_assertion( mean_ref == mean_res );
  mln_assertion(0.0001 > abs(var_ref - var_res));

  std::cout << "(08 bits) test integration     : ok" << std::endl;
}

double gaussian_distribution(double x, double mean, double sigma)
{
  double num    = exp(-0.5*mln::math::sqr((x - mean)/sigma));
  double denom  = sigma*mln::math::sqrt(2*mln::math::pi);
  double result = num/denom;

  return result;
}

/// To calibrate with gnuplot (see gaussian.sh)
/// square(x) = x*x
/// gaussian(x,mean,sigma)=exp(-0.5*square((x-mean)/sigma))/(sigma*sqrt(2*pi))
/// plot [-20:20] gaussian(x,0,6.0)

void gaussian_filter(double ws[], unsigned size, double sigma)
{
  int h = size/2;

  for (int i = -h; i <= h; ++i)
  {
    ws[i+h] = gaussian_distribution(i, 0.0, sigma);  
  }

  double sum = 0.0;

  for (unsigned i = 0; i < size; ++i)
    sum += ws[i];

  for (unsigned i = 0; i < size; ++i)
    ws[i] /= sum;
}

/// The aim of this function is to rebuild an label image2d from the segmenting
/// image of the histogram (label) and the original image (input).
/// label_image2d / for each grey tone, associate its label.

mln::image2d<mln::value::label_8>
build_8bits(const mln::image2d<mln::value::int_u8>&  input,
	    const mln::image1d<mln::value::label_8>& label)
{
  mln::trace::entering("build_8bits");
  mln_precondition(label.is_valid());
  mln_precondition(input.is_valid());

  mln::image2d<mln::value::label_8>             output;  

  mln::initialize(output, input);

  mln_piter_(mln::image2d<mln::value::int_u8>)  pi(input.domain());
  mln_piter_(mln::image2d<mln::value::label_8>) po(output.domain());

  for_all_2(pi, po)
  {
    mln::value::int_u8 val = input(pi);
    unsigned           grp = label(mln::point1d(val));

    output(po) = grp;
  }

  mln::trace::exiting("build_8bits");
  return output;
}


void test_8bits_classifying()
{
  typedef mln::value::int_u8                int_u8;
  typedef mln::value::label_8               label_8;
  typedef mln::value::rgb8                  rgb8;
  typedef mln::accu::stat::mean<double>     mean;

  mln::image2d<int_u8>        img_ref;
  mln::image2d<int_u8>        img_out;
  mln::image2d<rgb8>          img_rgb;
  mln::image1d<unsigned>      img_res;
  mln::image1d<double>        img_smooth;
  mln::image1d<label_8>       labels;
  label_8                     nlabels;

  //-----------------------------------------------------
  // Loading the scribo image and computing its histogram
  //-----------------------------------------------------

  std::cout << "(08 bits) LOADING HISTOGRAM" << std::endl;

  //  mln::io::pgm::load(img_ref, OLENA_IMG_PATH"/lena.pgm");
  mln::io::pgm::load(img_ref, SCRIBO_IMG_PATH"/mp00082c_50p_8bits.pgm");
  img_res = mln::data::compute(mln::accu::stat::histo1d<int_u8>(), img_ref);
  mln::io::plot::save_histo_sh(img_res, "histo0_8bits.sh");


  //-----------------------------------------------------
  // Smoothing the histogram with a gaussian filter
  //-----------------------------------------------------

  std::cout << "(08 bits) SMOOTHING HISTOGRAM" << std::endl;

  double ws[41];
  gaussian_filter(ws, 41, 6.0);
  img_smooth = mln::linear::convolve(img_res, mln::make::w_window1d(ws));
  mln::io::plot::save_histo_sh(img_smooth, "histo1_8bits.sh");


  //-----------------------------------------------------
  // Segmenting the histogram with the watershed method
  //-----------------------------------------------------

  std::cout << "SEGMENTING HISTOGRAM" << std::endl;

  /*
  labels = mln::labeling::regional_maxima(img_smooth, mln::c2(), nlabels);  
  std::cout  << "N labels : " << nlabels << std::endl;
  mln::io::plot::save_histo_sh(labels, "histo2_8bits.sh");
  */

  
  labels = mln::morpho::watershed::flooding(img_smooth, mln::c2(), nlabels);
  std::cout  << "N labels : " << nlabels << std::endl;
  mln::io::plot::save_histo_sh(labels, "histo2_8bits.sh");
  

  //-----------------------------------------------------
  // Rebuilding the image with the mean of each region
  //-----------------------------------------------------

  std::cout << "(08 bits) BUILDING OUTPUT" << std::endl;

  mln::image2d<label_8>img_label = build_8bits(img_ref, labels);

  std::cout << "(08 bits) COLORING OUTPUT" << std::endl;

  img_out = mln::labeling::mean_values(img_ref, img_label, nlabels);
  img_rgb = mln::labeling::colorize(rgb8(), img_label);

  mln::io::pgm::save(img_out, "out_8bits.pgm");
  mln::io::ppm::save(img_rgb, "color_8bits.pgm");

  //labels = mln::morpho::elementary::dilation(labels, mln::c2());  
  //mln::io::plot::save_histo_sh(labels, "histo3.sh");
  //mln::io::plot::save(labels, "labelized.data");
}

//------------------------------------------------------------------------------
// TEST IN 15 BITS
//------------------------------------------------------------------------------

void test_14bits_operator_equal()
{
  using namespace mln::accu::stat;

  typedef mln::value::int_u<14>     int_u14;
  mln::accu::stat::histo1d<int_u14> histo1;
  mln::accu::stat::histo1d<int_u14> histo2;
  int_u14                           val = 3;

  histo1.init();
  histo2.init();

  mln_assertion(histo1 == histo2);

  histo1.take(val);

  /// FIXME mln_assertion(histo1 != histo2); doesn't work!!
  mln_assertion(!(histo1 == histo2));

  histo2.take(val);

  mln_assertion(histo1 == histo2);

  std::cout << "(14 bits) histo == histo       : ok" << std::endl;
}

void test_14bits_instantiation_without_argument()
{
  typedef mln::value::int_u<14>             int_u14;
  const   mln::accu::stat::histo1d<int_u14> histo;
  const   mln::image1d<unsigned>&           res = histo.to_result();
  const   mln::point1d&                     min =mln::point1d(mln_min(int_u14));
  const   mln::point1d&                     max =mln::point1d(mln_max(int_u14));
  const   mln::box1d&                       ref = mln::box1d(min, max);

  mln_assertion(ref == res.domain());
  mln_assertion(res.is_valid());

  std::cout << "(14 bits) histo<T> histo       : ok" << std::endl;
}


void test_14bits_initialization()
{
  typedef mln::value::int_u<14>     int_u14;
  mln::accu::stat::histo1d<int_u14> histo;
  mln::image1d<unsigned>            img_res = histo.to_result();
  mln::image1d<unsigned>            img_ref;

  mln::initialize(img_ref, img_res);
  mln::data::fill(img_ref, 0);
  histo.init();

  unsigned res = mln::data::compute(mln::accu::math::sum<int_u14>(), img_res);
  unsigned ref = mln::data::compute(mln::accu::math::sum<int_u14>(), img_ref);
  
  mln_assertion(ref == res);

  std::cout << "(14 bits) histo.init()         : ok" << std::endl;
}


void test_14bits_take_argument()
{
  typedef mln::value::int_u<14>     int_u14;
  mln::accu::stat::histo1d<int_u14> histo1;
  mln::accu::stat::histo1d<int_u14> histo2;
  int_u14                           val = 3;

  histo1.init();
  histo2.init();
  histo1.take(val);

  const mln::image1d<unsigned>     img1 = histo1.to_result();
  const mln::image1d<unsigned>     img2 = histo2.to_result();

  const unsigned res = mln::data::compute(mln::accu::math::sum<int_u14>(),img1);
  const unsigned ref = mln::data::compute(mln::accu::math::sum<int_u14>(),img2);

  mln_assertion(ref == res-1);
  mln_assertion(1 == img1(mln::point1d(val)));

  std::cout << "(14 bits) histo.take(argument) : ok" << std::endl;
}


void test_14bits_take_other()
{
  typedef mln::value::int_u<14>     int_u14;
  mln::accu::stat::histo1d<int_u14> histo1;
  mln::accu::stat::histo1d<int_u14> histo2;
  mln::accu::stat::histo1d<int_u14> histo3;
  int_u14                           val = 3;

  histo1.init();
  histo2.init();
  histo3.init();

  histo1.take(val);
  histo1.take(val);
  histo3.take(val);
  histo2.take(val);
  histo2.take(histo3);

  mln_assertion(histo1 == histo2);

  std::cout << "(14 bits) histo.take(other)    : ok" << std::endl;
}

void test_14bits_integration()
{
  typedef mln::value::int_u16               int_u16;
  typedef mln::value::int_u<14>             int_u14;
  typedef mln::accu::math::count<double>    count;
  typedef mln::accu::math::sum<double>      sum;
  typedef mln::accu::stat::mean<double>     mean;
  typedef mln::accu::stat::variance<double> variance;

  mln::image2d<int_u16>      img_fst;
  mln::image2d<int_u14>      img_ref;
  mln::image1d<unsigned>     img_res;

  mln::io::pgm::load(img_fst, OLENA_IMG_PATH"/lena_16.pgm");
  img_ref = mln::data::transform(img_fst, mln::fun::v2v::int_u16_to_int_u14());

  const double count_ref = mln::data::compute(count(),    img_ref);
  const double mean_ref  = mln::data::compute(mean(),     img_ref);
  const double var_ref   = mln::data::compute(variance(), img_ref);

  img_res = mln::data::compute(mln::accu::stat::histo1d<int_u14>(), img_ref);

  const double count_res = count_histo(img_res);
  const double mean_res  = mean_histo(img_res);
  const double var_res   = var_histo2(img_res);

  //std::cout << "var_res : " << var_res << std::endl;
  //std::cout << "var_ref : " << var_ref << std::endl;
  
  mln_assertion(count_ref == count_res);
  mln_assertion( mean_ref == mean_res );
  mln_assertion(0.0001 > abs(var_ref - var_res));

  std::cout << "(14 bits) test integration     : ok" << std::endl;
}

/// The aim of this function is to rebuild an label image2d from the segmenting
/// image of the histogram (label) and the original image (input).
/// label_image2d / for each grey tone, associate its label.

mln::image2d<mln::value::label_8>
build_14bits(const mln::image2d<mln::value::int_u<14> >&  input,
	     const mln::image1d<mln::value::label_8>&     label)
{
  mln::trace::entering("build_14bits");
  mln_precondition(label.is_valid());
  mln_precondition(input.is_valid());

  mln::image2d<mln::value::label_8>             output;  

  mln::initialize(output, input);

  mln_piter_(mln::image2d<mln::value::int_u<14> >) pi(input.domain());
  mln_piter_(mln::image2d<mln::value::label_8>) po(output.domain());

  for_all_2(pi, po)
  {
    mln::value::int_u<14> val = input(pi);
    unsigned              grp = label(mln::point1d(val));

    output(po) = grp;
  }

  mln::trace::exiting("build_14bits");
  return output;
}


void test_14bits_classifying()
{
  typedef mln::value::int_u16               int_u16;
  typedef mln::value::int_u<14>             int_u14;
  typedef mln::value::label_8               label_8;
  typedef mln::value::rgb8                  rgb8;
  typedef mln::accu::stat::mean<double>     mean;

  mln::image2d<int_u16>       img_fst;
  mln::image2d<int_u14>       img_ref;
  mln::image2d<int_u14>       img_out;
  mln::image2d<rgb8>          img_rgb;
  mln::image1d<unsigned>      img_res;
  mln::image1d<double>        img_smooth;
  mln::image1d<label_8>       labels;
  label_8                     nlabels;

  //-----------------------------------------------------
  // Loading the scribo image and computing its histogram
  //-----------------------------------------------------

  std::cout << "(14 bits) LOADING HISTOGRAM" << std::endl;

  //mln::io::pgm::load(img_fst, OLENA_IMG_PATH"/lena_16.pgm");
  mln::io::pgm::load(img_fst, SCRIBO_IMG_PATH"/mp00082c_50p_16bits.pgm");
  img_ref = mln::data::transform(img_fst, mln::fun::v2v::int_u16_to_int_u14());
  img_res = mln::data::compute(mln::accu::stat::histo1d<int_u14>(), img_ref);
  mln::io::plot::save_histo_sh(img_res, "histo0_14bits.sh");


  //-----------------------------------------------------
  // Smoothing the histogram with a gaussian filter
  //-----------------------------------------------------

  std::cout << "(14 bits) SMOOTHING HISTOGRAM" << std::endl;

  double ws[401];
  gaussian_filter(ws, 401, 50.0);
  img_smooth = mln::linear::convolve(img_res, mln::make::w_window1d(ws));
  mln::io::plot::save_histo_sh(img_smooth, "histo1_14bits.sh");


  //-----------------------------------------------------
  // Segmenting the histogram with the watershed method
  //-----------------------------------------------------

  std::cout << "(14 bits) SEGMENTING HISTOGRAM" << std::endl;

  /*
  labels = mln::labeling::regional_maxima(img_smooth, mln::c2(), nlabels);  
  std::cout  << "N labels : " << nlabels << std::endl;
  mln::io::plot::save_histo_sh(labels, "histo2.sh");
  */

  
  labels = mln::morpho::watershed::flooding(img_smooth, mln::c2(), nlabels);
  std::cout  << "N labels : " << nlabels << std::endl;
  mln::io::plot::save_histo_sh(labels, "histo2_14bits.sh");
  

  //-----------------------------------------------------
  // Rebuilding the image with the mean of each region
  //-----------------------------------------------------

  std::cout << "(14 bits) BUILDING OUTPUT" << std::endl;

  mln::image2d<label_8>img_label = build_14bits(img_ref, labels);

  std::cout << "(14 bits) COLORING OUTPUT" << std::endl;

  img_out = mln::labeling::mean_values(img_ref, img_label, nlabels);
  img_rgb = mln::labeling::colorize(rgb8(), img_label);

  mln::io::pgm::save(img_out, "out_14bits.pgm");
  mln::io::ppm::save(img_rgb, "color_14bits.pgm");

  //labels = mln::morpho::elementary::dilation(labels, mln::c2());  
  //mln::io::plot::save_histo_sh(labels, "histo3.sh");
  //mln::io::plot::save(labels, "labelized.data");
}

int main()
{
  test_8bits_operator_equal();
  /*
  test_8bits_instantiation_without_argument();
  test_8bits_initialization();
  test_8bits_take_argument();
  test_8bits_take_other();
  test_8bits_operator_equal();
  */
  //test_8bits_integration();

  //test_8bits_classifying();

  // PROBLEME AVEC LES COORDONNEES PAR DEFAUT QUI SONT EN SIGNED SHORT
  // SEE mln/core/def/coord.hh
  /*
  test_14bits_instantiation_without_argument();
  test_14bits_initialization();
  test_14bits_take_argument();
  test_14bits_take_other();
  test_14bits_operator_equal();
  test_14bits_integration();
  */
  //test_14bits_classifying();
  
  return 0;
}
