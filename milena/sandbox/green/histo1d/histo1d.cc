/// TEST HISTO1D

#include "save_sh.hh"
#include "histo1d.hh"

#include <mln/io/pgm/load.hh>
#include <mln/io/plot/save.hh>

#include <mln/data/compute.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>

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
  mln_assertion(  var_ref == var_res  );

  std::cout << "test integration     : ok" << std::endl;
}


void test_classifying()
{
  typedef mln::value::int_u8  int_u8;
  typedef mln::value::label_8 label_8;

  mln::image2d<int_u8>        img_ref;
  mln::image1d<unsigned>      img_res;
  mln::image1d<label_8>       labels;
  label_8                     n_shape;

  mln::io::pgm::load(img_ref, "/usr/local/share/olena/images/lena.pgm");
  
  img_res = mln::data::compute(mln::accu::stat::histo1d<int_u8>(), img_ref);

  mln::io::plot::save_sh(img_res, "histo.sh");

  img_res = mln::morpho::elementary::closing(img_res, mln::c2());
  img_res = mln::morpho::elementary::closing(img_res, mln::c2());
  img_res = mln::morpho::elementary::closing(img_res, mln::c2());

  mln::io::plot::save_sh(img_res, "histo1.sh");
  /*
  labels = mln::morpho::watershed::flooding(img_res, mln::c2(), n_shape);

  mln::io::plot::save_sh(labels, "histo2.sh");

  labels = mln::morpho::elementary::dilation(labels, mln::c2());
  
  mln::io::plot::save_sh(labels, "histo3.sh");

  //mln::io::plot::save(labels, "labelized.data");
  */
}

int main()
{
  using namespace mln;
  using namespace std;

  test_instantiation_without_argument();
  test_initialization();
  test_take_argument();
  test_take_other();
  test_operator_equal();
  test_integration();

  //test_classifying();

  return 0;
}
