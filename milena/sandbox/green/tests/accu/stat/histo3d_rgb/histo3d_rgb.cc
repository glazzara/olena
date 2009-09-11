/// TEST HISTO3D_RGB

#include <mln/img_path.hh>

#include <mln/io/plot/save_histo_sh.hh>
#include <mln/accu/stat/histo3d_rgb.hh>
#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/plot/save.hh>

#include <mln/data/compute.hh>
#include <mln/data/transform.hh>
#include <mln/data/fill.hh>

#include <mln/value/label_8.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/rgb.hh>
#include <mln/value/int_u.hh>

#include <mln/core/alias/neighb3d.hh>
#include <mln/core/alias/box3d.hh>
#include <mln/core/alias/point3d.hh>

#include <mln/core/routine/initialize.hh>
#include <mln/core/contract.hh>
#include <mln/core/grids.hh>

#include <mln/accu/math/sum.hh>
#include <mln/accu/math/count.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/accu/stat/variance.hh>
#include <mln/accu/stat/var.hh>

#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/morpho/elementary/closing.hh>

#include <mln/literal/zero.hh>
#include <mln/linear/convolve.hh>
#include <mln/linear/gaussian.hh>
#include <mln/labeling/regional_maxima.hh>
#include <mln/labeling/colorize.hh>
#include <mln/labeling/mean_values.hh>

#include <mln/make/w_window3d.hh>

#include <mln/math/sqr.hh>
#include <mln/math/pi.hh>
#include <mln/math/abs.hh>

#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/value.hh>


template <unsigned n>
void test_operator_equal()
{
  using namespace mln::accu::stat;

  typedef mln::value::rgb<n>           rgbn;
  mln::accu::stat::histo3d_rgb<rgbn>   histo1;
  mln::accu::stat::histo3d_rgb<rgbn>   histo2;
  rgbn                                 val(3,3,3);

  histo1.init();
  histo2.init();

  mln_assertion(histo1 == histo2);

  histo1.take(val);

  /// FIXME mln_assertion(histo1 != histo2); doesn't work!!
  mln_assertion(!(histo1 == histo2));

  histo2.take(val);

  mln_assertion(histo1 == histo2);

  std::cout << "(" << n << " bits) histo == histo       : ok" << std::endl;
}

template <unsigned n>
void test_instantiation_without_argument()
{
  typedef mln::value::rgb<n>                   rgbn;
  typedef mln_trait_value_comp(rgbn,0)         comp;
  const   mln::accu::stat::histo3d_rgb<rgbn>   histo;
  const   mln::image3d<unsigned>&              res = histo.to_result();
  const   mln::point3d&                        min =mln::point3d(mln_min(comp),
								 mln_min(comp),
								 mln_min(comp));
  const   mln::point3d&                        max =mln::point3d(mln_max(comp),
								 mln_max(comp),
								 mln_max(comp));
  const   mln::box3d&                          ref = mln::box3d(min, max);

  mln_assertion(ref == res.domain());
  mln_assertion(res.is_valid());

  std::cout << "(" << n << " bits) histo<T> histo       : ok" << std::endl;
}

template <unsigned n>
void test_initialization()
{
  typedef mln::value::int_u<n>         int_un;
  typedef mln::value::rgb<n>           rgbn;
  mln::accu::stat::histo3d_rgb<rgbn>   histo;
  mln::image3d<unsigned>               img_res = histo.to_result();
  mln::image3d<unsigned>               img_ref;

  mln::initialize(img_ref, img_res);
  mln::data::fill(img_ref, 0);
  histo.init();

  unsigned res = mln::data::compute(mln::accu::math::sum<int_un>(), img_res);
  unsigned ref = mln::data::compute(mln::accu::math::sum<int_un>(), img_ref);
  
  mln_assertion(ref == res);

  std::cout << "(" << n << " bits) histo.init()         : ok" << std::endl;
}

template <unsigned n>
void test_take_argument()
{
  typedef mln::value::int_u<n>         int_un;
  typedef mln::value::rgb<n>           rgbn;
  mln::accu::stat::histo3d_rgb<rgbn>   histo1;
  mln::accu::stat::histo3d_rgb<rgbn>   histo2;
  rgbn                                 val(3,3,3);

  histo1.init();
  histo2.init();
  histo1.take(val);

  const mln::image3d<unsigned>     img1 = histo1.to_result();
  const mln::image3d<unsigned>     img2 = histo2.to_result();

  const unsigned res = mln::data::compute(mln::accu::math::sum<int_un>(), img1);
  const unsigned ref = mln::data::compute(mln::accu::math::sum<int_un>(), img2);

  mln_assertion(ref == res-1);
  mln_assertion(1 == img1(mln::point3d(val.red(), val.green(), val.blue())));

  std::cout << "(" << n << " bits) histo.take(argument) : ok" << std::endl;
}

template <unsigned n>
void test_take_other()
{
  typedef mln::value::rgb<n>           rgbn;
  mln::accu::stat::histo3d_rgb<rgbn>   histo1;
  mln::accu::stat::histo3d_rgb<rgbn>   histo2;
  mln::accu::stat::histo3d_rgb<rgbn>   histo3;
  rgbn                                 val(3,3,3);

  histo1.init();
  histo2.init();
  histo3.init();

  histo1.take(val);
  histo1.take(val);
  histo3.take(val);
  histo2.take(val);
  histo2.take(histo3);

  mln_assertion(histo1 == histo2);

  std::cout << "(" << n << " bits) histo.take(other)    : ok" << std::endl;
}

double count_histo(const mln::image3d<unsigned>& img)
{
  mln_precondition(img.is_valid());
  
  double                             result = 0.0;
  mln_piter_(mln::image3d<unsigned>) p(img.domain());

  for_all(p)
    result += img(p);

  return result;
}

mln::algebra::vec<3,float> conv(const mln::algebra::vec<3,float>& vec)
{
  mln::algebra::vec<3,float> result;

  result[0] = vec[2];
  result[1] = vec[0];
  result[2] = vec[1];

  return result;
}

mln::algebra::vec<3,float> mean_histo(const mln::image3d<unsigned>& img)
{
  mln_precondition(img.is_valid());
  typedef mln::algebra::vec<3,float> vec3f;
  double                             count  = 0.0;
  vec3f                              sum    = mln::literal::zero;
  vec3f                              result;

  mln_piter_(mln::image3d<unsigned>) p(img.domain());

  for_all(p)
  {
    /*
    std::cout << "(1) " << p << std::endl;
    std::cout << "(2) " << (vec3f)p << std::endl;
    std::cout << "(3) " << conv((vec3f)p) << std::endl;
    */
    count += img(p);
    sum   += conv((vec3f)p) * img(p);
    // sum   += p.to_vec() * img(p);
  }

  result = sum / count;

  return result;
}


double var_histo(const mln::image3d<unsigned>& img)
{
  mln_precondition(img.is_valid());
  typedef mln::algebra::vec<3,float>   vec3f;
  typedef mln::algebra::mat<3,3,float> mat3f;
  double                               count  = 0.0;
  double                               sum    = 0.0;
  double                               sum2   = 0.0;
  double                               result = 0.0;
  mln_piter_(mln::image3d<unsigned>) p(img.domain());

  for_all(p)
  {
    count += img(p);
    sum   += p[0] * img(p);
    sum2  += p[0] * p[0] * img(p);
  }

  result = sum2 / count - (sum / count) * (sum / count);

  return result;
}

mln::algebra::mat<3,3,float> var_histo2(const mln::image3d<unsigned>& img)
{
  mln_precondition(img.is_valid());
  typedef mln::algebra::vec<3,float>   vec3f;
  typedef mln::algebra::mat<3,3,float> mat3f;
  
  double                             count  = count_histo(img);
  vec3f                              mean   = mean_histo(img);
  vec3f                              point;
  mat3f                              result = mln::literal::zero;
  mln_piter_(mln::image3d<unsigned>) p(img.domain());

  for_all(p)
  {
    // point = p.to_vec() - mean;
    point  = conv((vec3f)p) - mean;
    result += img(p) * (point * point.t());
  }

  result /= count;

  return result;
}

template <unsigned n>
void test_integration()
{
  typedef mln::value::rgb8                        rgb8;
  typedef mln::value::rgb<n>                      rgbn;
  typedef mln::algebra::vec<3,float>              vec3f;
  typedef mln::algebra::mat<3,3,float>            mat3f;
  typedef mln::accu::math::count<rgbn>            count;
  typedef mln::accu::math::sum<rgbn,vec3f>        sum;
  typedef mln::accu::stat::mean<rgbn,vec3f,vec3f> mean;
  typedef mln::accu::stat::var<vec3f>             var;

  mln::image2d<rgb8>         img_fst;
  mln::image2d<rgbn>         img_ref;
  mln::image3d<unsigned>     img_res;

  mln::io::ppm::load(img_fst, OLENA_IMG_PATH"/lena.ppm");
  img_ref = mln::data::transform(img_fst, mln::fun::v2v::rgb8_to_rgbn<n>());

  const double count_ref = mln::data::compute(count(), img_ref);
  const vec3f  sum_ref   = mln::data::compute(sum(),   img_ref);
  const vec3f  mean_ref  = mln::data::compute(mean(),  img_ref);
  const mat3f  var_ref   = mln::data::compute(var(),   img_ref);
  
  img_res = mln::data::compute(mln::accu::stat::histo3d_rgb<rgbn>(), img_ref);


  const double count_res = count_histo(img_res);
  const vec3f  mean_res  = mean_histo(img_res);
  const mat3f  var_res   = var_histo2(img_res);
 
  std::cout << "count_ref : " << count_ref << std::endl;
  std::cout << "mean_ref  : " << mean_ref  << std::endl;
  std::cout << "var_ref   : " << var_ref   << std::endl;

  std::cout << "count_res : " << count_res << std::endl;
  std::cout << "mean_res  : " << mean_res  << std::endl;
  std::cout << "var_res   : " << var_res   << std::endl;

  
  mln_assertion(count_ref == count_res);
 
  mln_assertion(0.0001 > abs(mean_ref[0] - mean_res[0]));
  mln_assertion(0.0001 > abs(mean_ref[1] - mean_res[1]));
  mln_assertion(0.0001 > abs(mean_ref[2] - mean_res[2]));

  mln_assertion(0.0001 > abs(var_ref(0,0) - var_res(0,0)));
  mln_assertion(0.0001 > abs(var_ref(0,1) - var_res(0,1)));
  mln_assertion(0.0001 > abs(var_ref(1,0) - var_res(1,0)));

  mln_assertion(0.0001 > abs(var_ref(1,1) - var_res(1,1)));
  mln_assertion(0.0001 > abs(var_ref(0,2) - var_res(0,2)));
  mln_assertion(0.0001 > abs(var_ref(2,0) - var_res(2,0)));


  mln_assertion(0.0001 > abs(var_ref(2,2) - var_res(2,2)));
  mln_assertion(0.0001 > abs(var_ref(2,1) - var_res(2,1)));
  mln_assertion(0.0001 > abs(var_ref(1,2) - var_res(1,2)));
  
  std::cout << "(" << n << " bits) test integration     : ok" << std::endl;
}

int main()
{
  //
  // Doesn't work with 1 bit, compilation problem
  //

  /*
  test_operator_equal<1>();
  test_instantiation_without_argument<1>();
  test_initialization<1>();
  test_take_argument<1>();
  test_take_other<1>();
  test_integration<1>();
  */

  test_operator_equal<2>();
  test_instantiation_without_argument<2>();
  test_initialization<2>();
  test_take_argument<2>();
  test_take_other<2>();
  test_integration<2>();

  /*
  test_operator_equal<3>();
  test_instantiation_without_argument<3>();
  test_initialization<3>();
  test_take_argument<3>();
  test_take_other<3>();
  test_integration<3>();
  
  

  test_operator_equal<4>();
  test_instantiation_without_argument<4>();
  test_initialization<4>();
  test_take_argument<4>();
  test_take_other<4>();
  test_integration<4>();
  
  
  test_operator_equal<5>();
  test_instantiation_without_argument<5>();
  test_initialization<5>();
  test_take_argument<5>();
  test_take_other<5>();
  test_integration<5>();
  
  test_operator_equal<6>();
  test_instantiation_without_argument<6>();
  test_initialization<6>();
  test_take_argument<6>();
  test_take_other<6>();
  test_integration<6>();
  */
  /*
  test_operator_equal<7>();
  test_instantiation_without_argument<7>();
  test_initialization<7>();
  test_take_argument<7>();
  test_take_other<7>();
  test_integration<7>();
  */
    
  //
  // Do not execute it, unless you have the time :)
  //

  /*
  test_operator_equal<8>();
  test_instantiation_without_argument<8>();
  test_initialization<8>();
  test_take_argument<8>();
  test_take_other<8>();
  test_integration<8>();
  */
 

  // p2p/fold+transform_domain for hcv

  return 0;
}
