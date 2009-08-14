/// TEST HISTO1D

#include "save_sh.hh"
#include "histo1d.hh"

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/plot/save.hh>

#include <mln/data/compute.hh>
#include <mln/data/fill.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/value/rgb8.hh>

#include <mln/core/alias/neighb1d.hh>
#include <mln/core/alias/box1d.hh>
#include <mln/core/alias/point1d.hh>

#include <mln/core/routine/initialize.hh>
#include <mln/core/contract.hh>

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




void test_operator_equal()
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

  /// FIXME mln_assertion(histo1 != histo2); doesn't work!!
  mln_assertion(!(histo1 == histo2));

  histo2.take(val);

  mln_assertion(histo1 == histo2);

  std::cout << "histo == histo       : ok" << std::endl;
}

void test_instantiation_without_argument()
{
  typedef mln::value::int_u8               int_u8;
  const   mln::accu::stat::histo1d<int_u8> histo;
  const   mln::image1d<unsigned>&          res = histo.to_result();
  const   mln::point1d&                    min = mln::point1d(mln_min(int_u8));
  const   mln::point1d&                    max = mln::point1d(mln_max(int_u8));
  const   mln::box1d&                      ref = mln::box1d(min, max);

  mln_assertion(ref == res.domain());
  mln_assertion(res.is_valid());

  std::cout << "histo<T> histo       : ok" << std::endl;
}


void test_initialization()
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

  std::cout << "histo.init()         : ok" << std::endl;
}


void test_take_argument()
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

  std::cout << "histo.take(argument) : ok" << std::endl;
}


void test_take_other()
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

  std::cout << "histo.take(other)    : ok" << std::endl;
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

void test_integration()
{
  typedef mln::value::int_u8                int_u8;
  typedef mln::accu::math::count<double>    count;
  typedef mln::accu::math::sum<double>      sum;
  typedef mln::accu::stat::mean<double>     mean;
  typedef mln::accu::stat::variance<double> variance;

  mln::image2d<int_u8>       img_ref;
  mln::image1d<unsigned>     img_res;

  mln::io::pgm::load(img_ref, "/usr/local/share/olena/images/lena.pgm");

  const double count_ref = mln::data::compute(count(),    img_ref);
  const double mean_ref  = mln::data::compute(mean(),     img_ref);
  const double var_ref   = mln::data::compute(variance(), img_ref);

  img_res = mln::data::compute(mln::accu::stat::histo1d<int_u8>(), img_ref);

  const double count_res = count_histo(img_res);
  const double mean_res  = mean_histo(img_res);
  const double var_res   = var_histo(img_res);
  
  mln_assertion(count_ref == count_res);
  mln_assertion( mean_ref == mean_res );
  mln_assertion(0.0001 > abs(var_ref - var_res));

  std::cout << "test integration     : ok" << std::endl;
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
build(const mln::image2d<mln::value::int_u8>&  input,
      const mln::image1d<mln::value::label_8>& label)
{
  mln::trace::entering("build");
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

  mln::trace::exiting("build");
  return output;
}


void test_classifying()
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

  std::cout << "LOADING HISTOGRAM" << std::endl;

  //  mln::io::pgm::load(img_ref, "/usr/local/share/olena/images/lena.pgm");
  mln::io::pgm::load(img_ref, "mp00082c_50p.pgm");
  img_res = mln::data::compute(mln::accu::stat::histo1d<int_u8>(), img_ref);
  mln::io::plot::save_sh(img_res, "histo0.sh");


  //-----------------------------------------------------
  // Smoothing the histogram with a gaussian filter
  //-----------------------------------------------------

  std::cout << "SMOOTHING HISTOGRAM" << std::endl;

  double ws[41];
  gaussian_filter(ws, 41, 6.0);
  img_smooth = mln::linear::convolve(img_res, mln::make::w_window1d(ws));
  mln::io::plot::save_sh(img_smooth, "histo1.sh");


  //-----------------------------------------------------
  // Segmenting the histogram with the watershed method
  //-----------------------------------------------------

  std::cout << "SEGMENTING HISTOGRAM" << std::endl;

  /*
  labels = mln::labeling::regional_maxima(img_smooth, mln::c2(), nlabels);  
  std::cout  << "N labels : " << nlabels << std::endl;
  mln::io::plot::save_sh(labels, "histo2.sh");
  */

  
  labels = mln::morpho::watershed::flooding(img_smooth, mln::c2(), nlabels);
  std::cout  << "N labels : " << nlabels << std::endl;
  mln::io::plot::save_sh(labels, "histo2.sh");
  

  //-----------------------------------------------------
  // Rebuilding the image with the mean of each region
  //-----------------------------------------------------

  std::cout << "BUILDING OUTPUT" << std::endl;

  mln::image2d<label_8>img_label = build(img_ref, labels);

  std::cout << "COLORING OUTPUT" << std::endl;

  img_out = mln::labeling::mean_values(img_ref, img_label, nlabels);
  img_rgb = mln::labeling::colorize(rgb8(), img_label);

  mln::io::pgm::save(img_out, "out.pgm");
  mln::io::ppm::save(img_rgb, "color.pgm");

  //labels = mln::morpho::elementary::dilation(labels, mln::c2());  
  //mln::io::plot::save_sh(labels, "histo3.sh");
  //mln::io::plot::save(labels, "labelized.data");
}

int main()
{
  using namespace mln;
  using namespace std;

  /*
  test_instantiation_without_argument();
  test_initialization();
  test_take_argument();
  test_take_other();
  test_operator_equal();
  test_integration();
  */

  test_classifying();

  return 0;
}
