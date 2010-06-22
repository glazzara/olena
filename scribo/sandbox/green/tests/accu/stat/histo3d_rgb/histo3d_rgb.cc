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
/// \brief This source manages unitary testing on rgb image3d histogram.
///
/// Tests are performed from 2 bits up to 8 bits quantification. The goal
/// is to go through each programmatic flow to verify the conformity of the
/// code. It sounds like a theoritic model of unitary testing for milena.
/// The last test enables statistic computations on that component.

#include <mln/accu/math/sum.hh>
#include <mln/accu/math/count.hh>
#include <mln/accu/stat/histo3d_rgb.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/accu/stat/variance.hh>
#include <mln/accu/stat/var.hh>

#include <mln/core/alias/box3d.hh>
#include <mln/core/alias/point3d.hh>
#include <mln/core/contract.hh>
#include <mln/core/grids.hh>
#include <mln/core/routine/initialize.hh>

#include <mln/data/compute.hh>
#include <mln/data/fill.hh>
#include <mln/data/transform.hh>

#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/img_path.hh>

#include <mln/io/ppm/load.hh>

#include <mln/literal/zero.hh>

#include <mln/math/sqr.hh>
#include <mln/math/abs.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/rgb.hh>

/// Specific histogram accumulators.
/// \{
/// \brief Define statistical accumulator for histogram.
///
/// Here, some histogram accumulators are defined, the sum of the frequencies,
/// the average, the variance.

double count_histo(const mln::image3d<unsigned>& img)
{
  mln_precondition(img.is_valid());

  double                             value  = 0.0;
  double                             result = 0.0;
  mln_piter_(mln::image3d<unsigned>) p(img.domain());

  for_all(p)
  {
    value   = img(p);
    result += value;
  }

  return result;
}

mln::algebra::vec<3,float> mean_histo(const mln::image3d<unsigned>& img)
{
  mln_precondition(img.is_valid());
  typedef mln::algebra::vec<3,float> vec3f;
  double                             value  = 0.0;
  double                             count  = 0.0;
  vec3f                              sum    = mln::literal::zero;
  vec3f                              result;

  mln_piter_(mln::image3d<unsigned>) p(img.domain());

  for_all(p)
  {
    value  = img(p);
    count += value;
    sum   += p.to_vec() * value;
  }

  result = sum / count;

  return result;
}


mln::algebra::mat<3,3,float> var_histo (const mln::image3d<unsigned>& img)
{
  mln_precondition(img.is_valid());
  typedef mln::algebra::vec<3,float>   vec3f;
  typedef mln::algebra::mat<3,3,float> mat3f;

  double                             value  = 0.0;
  double                             count  = count_histo(img);
  vec3f                              mean   = mean_histo(img);
  vec3f                              point;
  mat3f                              result = mln::literal::zero;
  mln_piter_(mln::image3d<unsigned>) p(img.domain());

  for_all(p)
  {
    value   = img(p);
    point   = p.to_vec() - mean;
    result += value * (point * point.t());
  }

  result /= count;

  return result;
}

/// \}

/// n-bits testing
/// \{
/// \brief This part of the code manages the n-bits unitary testing.
///
/// Many tests are performed such equalities between two histograms,
/// instantiation without argument, initialization, take interface and
/// integration.
///
/// \fixme the operator != isn't infer from the operator ==.


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

  mln_assertion(!(histo1 == histo2));
  // mln_assertion(histo1 != histo2);
  // FIXME: DOESN'T WORK !!


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

/// \fixme When the quantification grows, the computation error grows.

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
  const mat3f  var_res   = var_histo(img_res);

  // std::cout << "count_ref : " << count_ref << std::endl;
  // std::cout << "mean_ref  : " << mean_ref  << std::endl;
  // std::cout << "var_ref   : " << var_ref   << std::endl;

  // std::cout << "count_res : " << count_res << std::endl;
  // std::cout << "mean_res  : " << mean_res  << std::endl;
  // std::cout << "var_res   : " << var_res   << std::endl;


  mln_assertion(count_ref == count_res);

  /// FIXME: The error acceptance seems to high !!
  mln_assertion(3 > mln::math::abs(mean_ref[0] - mean_res[0]));
  mln_assertion(3 > mln::math::abs(mean_ref[1] - mean_res[1]));
  mln_assertion(3 > mln::math::abs(mean_ref[2] - mean_res[2]));

  mln_assertion(3 > mln::math::abs(var_ref(0,0) - var_res(0,0)));
  mln_assertion(3 > mln::math::abs(var_ref(0,1) - var_res(0,1)));
  mln_assertion(3 > mln::math::abs(var_ref(1,0) - var_res(1,0)));

  mln_assertion(3 > mln::math::abs(var_ref(1,1) - var_res(1,1)));
  mln_assertion(3 > mln::math::abs(var_ref(0,2) - var_res(0,2)));
  mln_assertion(3 > mln::math::abs(var_ref(2,0) - var_res(2,0)));

  mln_assertion(3 > mln::math::abs(var_ref(2,2) - var_res(2,2)));
  mln_assertion(3 > mln::math::abs(var_ref(2,1) - var_res(2,1)));
  mln_assertion(3 > mln::math::abs(var_ref(1,2) - var_res(1,2)));

  std::cout << "(" << n << " bits) test integration     : ok" << std::endl;
}

/// \}

/// \fixme Doesn't work with 1 bit, compilation problem.

int main()
{

  test_operator_equal<2>();
  test_instantiation_without_argument<2>();
  test_initialization<2>();
  test_take_argument<2>();
  test_take_other<2>();
  test_integration<2>();


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

  test_operator_equal<7>();
  test_instantiation_without_argument<7>();
  test_initialization<7>();
  test_take_argument<7>();
  test_take_other<7>();
  test_integration<7>();

  //
  // Do not execute it, unless you have the time :)
  //

  // test_operator_equal<8>();
  // test_instantiation_without_argument<8>();
  // test_initialization<8>();
  // test_take_argument<8>();
  // test_take_other<8>();
  // test_integration<8>();

  return 0;
}
