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
/// \brief This source manages unitary testing on image1d histogram component.
///
/// Tests are performed in 8 bits and also in 14 bits quantification. The goal
/// is to go through each programmatic flow to verify the conformity of the
/// code. It sounds like a theoritic model of unitary testing for milena.
/// The last test enables statistic computations on that component.

#include <mln/accu/math/count.hh>
#include <mln/accu/math/sum.hh>
#include <mln/accu/stat/histo1d.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/accu/stat/variance.hh>

#include <mln/core/alias/box1d.hh>
#include <mln/core/alias/point1d.hh>
#include <mln/core/contract.hh>
#include <mln/core/grids.hh>
#include <mln/core/routine/initialize.hh>

#include <mln/data/compute.hh>
#include <mln/data/fill.hh>
#include <mln/data/transform.hh>

#include <mln/fun/v2v/int_u16_to_int_u14.hh>

#include <mln/img_path.hh>

#include <mln/io/pgm/load.hh>

#include <mln/math/sqr.hh>
#include <mln/math/abs.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>


/// Specific histogram accumulators.
/// \{
/// \brief Define statistical accumulator for histogram.
///
/// Here, some histogram accumulators are defined, the sum of the frequencies,
/// the average, the variance.

double count_histo(const mln::image1d<unsigned> img)
{
  mln_precondition(img.is_valid());

  double                             result = 0.0;
  double                             value  = 0.0;
  mln_piter_(mln::image1d<unsigned>) p(img.domain());

  for_all(p)
  {
    value  = img(p);
    result += value;
  }

  return result;
}

double mean_histo(mln::image1d<unsigned> img)
{
  mln_precondition(img.is_valid());

  double                             value  = 0.0;
  double                             site   = 0.0;
  double                             count  = 0.0;
  double                             sum    = 0.0;
  double                             result = 0.0;
  mln_piter_(mln::image1d<unsigned>) p(img.domain());

  for_all(p)
  {
    value =  img(p);
    site  =  p.ind();
    count += value;
    sum   += site * value;
  }

  result = sum / count;

  return result;
}


/// \fixme there is a conversion type problem with the interface of iterator p.

double var_histo(const mln::image1d<unsigned> img)
{
  mln_precondition(img.is_valid());

  double                             value  = 0.0;
  double                             site   = 0.0;
  // FIXME: such variables value and site are necessary to obtain the
  // well computation of the variance.

  double                             count  = 0.0;
  double                             sum    = 0.0;
  double                             sum2   = 0.0;
  double                             result = 0.0;
  mln_piter_(mln::image1d<unsigned>) p(img.domain());

  for_all(p)
  {
    value  = img(p);
    site   = p.ind();
    count += value;
    sum   += site * value;
    sum2  += mln::math::sqr(site) * value;
  }

  result = sum2 / count - (sum / count) * (sum / count);

  return result;
}


/// \fixme there is a conversion type problem with the interface of iterator p.

double var_histo2(const mln::image1d<unsigned> img)
{
  mln_precondition(img.is_valid());

  double                             value  = 0.0;
  double                             site   = 0.0;
  // FIXME: such variables value and site are necessary to obtain the
  // well computation of the variance.

  double                             count  = count_histo(img);
  double                             mean   = mean_histo(img);
  double                             result = 0.0;
  mln_piter_(mln::image1d<unsigned>) p(img.domain());

  for_all(p)
  {
    value  = img(p);
    site   = p.ind();
    result += value * mln::math::sqr(site - mean);
  }

  result /= count;

  return result;
}

/// \}


/// 8-bits testing
/// \{
/// \brief This part of the code manages the 8-bits unitary testing.
///
/// Many tests are performed such equalities between two histograms,
/// instantiation without argument, initialization, take interface and
/// integration.
///
/// \fixme the operator != isn't infer from the operator ==.

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

  mln_assertion(!(histo1 == histo2));
  // mln_assertion(histo1 != histo2);
  // FIXME: DOESN'T WORK !!

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

  const double count_res = count_histo(img_res);
  const double mean_res  = mean_histo(img_res);
  const double var_res   = var_histo(img_res);

  mln_assertion(count_ref == count_res);
  mln_assertion( mean_ref == mean_res );

  // std::cout << "var_ref : " << var_ref << std::endl;
  // std::cout << "var_res : " << var_res << std::endl;

  mln_assertion(0.0001 > mln::math::abs(var_ref - var_res));

  std::cout << "(08 bits) test integration     : ok" << std::endl;
}

/// \}

/// 14-bits testing
/// \{
/// \brief This part of the code manages the 14-bits unitary testing.
///
/// Many tests are performed such equalities between two histograms,
/// instantiation without argument, initialization, take interface and
/// integration.
///
/// \fixme the operator != isn't infer from the operator ==.

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

  /// FIXME: mln_assertion(histo1 != histo2); doesn't work!!
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

  mln::io::pgm::load(img_fst, OLENA_IMG_PATH"/lena_16_imageMagick.pgm");
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

/// \}

int main()
{
  test_8bits_operator_equal();
  test_8bits_instantiation_without_argument();
  test_8bits_initialization();
  test_8bits_take_argument();
  test_8bits_take_other();
  test_8bits_operator_equal();
  test_8bits_integration();

  test_14bits_instantiation_without_argument();
  test_14bits_initialization();
  test_14bits_take_argument();
  test_14bits_take_other();
  test_14bits_operator_equal();
  test_14bits_integration();

  return 0;
}
