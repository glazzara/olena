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
/// \brief This source manages unitary testing on the kmean implementation.
///
/// Tests are performed from 2 bits up to 8 bits quantification. The goal
/// is to go through each programmatic flow to verify the conformity of the
/// code. It sounds like a theoritic model of unitary testing for milena.
/// The last test enables statistic computations on that component.

#include <iostream>

#include <mln/literal/colors.hh>

#include <mln/algebra/vec.hh>
#include <mln/algebra/mat.hh>

#include <mln/clustering/k_mean.hh>

#include <mln/core/macros.hh>
#include <mln/core/contract.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/transform.hh>

#include <mln/img_path.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/trait/value/print.hh>
#include <mln/trait/image/print.hh>

#include <mln/pw/value.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#define SIZE_IMAGE    512
#define SIZE_SAMPLE1  (512*512)
#define SIZE_SAMPLE2  4
#define NB_CENTER     2
#define DIM_POINT     3
#define TYPE_POINT    double
#define MAT_POINT1    mln::algebra::mat<SIZE_SAMPLE1, DIM_POINT, TYPE_POINT>
#define MAT_POINT2    mln::algebra::mat<SIZE_SAMPLE2, DIM_POINT, TYPE_POINT>
#define MAT_CENTER    mln::algebra::mat<NB_CENTER, DIM_POINT, TYPE_POINT>
#define MAT_DISTANCE1 mln::algebra::mat<SIZE_SAMPLE1, NB_CENTER, TYPE_POINT>
#define MAT_DISTANCE2 mln::algebra::mat<SIZE_SAMPLE2, NB_CENTER, TYPE_POINT>
#define MAT_GROUP1    mln::algebra::mat<SIZE_SAMPLE1, NB_CENTER, TYPE_POINT>
#define MAT_GROUP2    mln::algebra::mat<SIZE_SAMPLE2, NB_CENTER, TYPE_POINT>
#define VEC_VAR       mln::algebra::vec<NB_CENTER, TYPE_POINT>

/// Tools.
/// \{
/// \brief Define some tools which are used in unitary testing.
///
/// This kind of tools help to fill an image with only 4 tons or
/// computing a distance between two colors. Just keep an eye, nothing
/// really difficult in this code.

struct rgb8_to_4colors : mln::Function_v2v<rgb8_to_4colors>
{
  typedef mln::value::rgb8 result;

  mln::value::rgb8 operator()(const mln::value::rgb8& color) const
  {
    mln::value::rgb8 result;
    unsigned         hash = (color.red() + color.green() + color.blue()) % 4;

    switch (hash)
    {
      case 0: result = mln::literal::lime;   break;
      case 1: result = mln::literal::brown;  break;
      case 2: result = mln::literal::teal;   break;
      case 3: result = mln::literal::purple; break;
    }

    return result;
  }
};


void print_color(const mln::value::rgb8& color)
{
  std::cout << "{r=" << color.red() << ", ";
  std::cout << "g="  << color.green() << ", ";
  std::cout << "b="  << color.blue() << "}" << std::endl;
}


void fill_image_with_4colors(mln::image2d<mln::value::rgb8>& img)
{
  const mln::value::rgb8 lime   = mln::literal::lime;
  const mln::value::rgb8 brown  = mln::literal::brown;
  const mln::value::rgb8 teal   = mln::literal::teal;
  const mln::value::rgb8 purple = mln::literal::purple;

  img = mln::data::transform(img, rgb8_to_4colors());

  //print_color(lime);
  //print_color(brown);
  //print_color(teal);
  //print_color(purple);
}


double dist(mln::value::rgb8 color1, mln::value::rgb8 color2)
{
  double red   = color1.red()   - color2.red();
  double green = color1.green() - color2.green();
  double blue  = color1.blue()  - color2.blue();
  double result= red * red + green * green + blue * blue;

  return result;
}

/// \}


/// External mutators.
/// \{
/// \brief Replace the kmean data structure values.
///
/// This is a hack that provides low level routines to access key data
/// structure like point, center, distance and group.

void set_point(MAT_POINT2&             point,
	       const unsigned          index,
	       const mln::value::rgb8& color)
{
  point(index,0) = color.red();
  point(index,1) = color.green();
  point(index,2) = color.blue();
}

void set_center(MAT_CENTER&             center,
		const unsigned          index,
		const mln::value::rgb8& color)
{
  center(index,0) = color.red();
  center(index,1) = color.green();
  center(index,2) = color.blue();
}

void set_distance(MAT_DISTANCE2& distance,
		  const unsigned index,
		  const double   d1,
		  const double   d2)
{
  distance(index,0) = d1;
  distance(index,1) = d2;
}

void set_group(MAT_GROUP2& group,
	       const unsigned index,
	       const unsigned min)
{
  group(index, min)   = 1.0;
  group(index, 1-min) = 0.0;
}

/// \}

/// Fake states.
/// \{
/// \brief Help to build from scratch temporary states for the kmean algorithm.
///
/// This hack allow us to build temporary results used in the previous step to
/// make us very the behaviour of the current step. There is a fake state for
/// every key data structure (point, group, center and distance).

void fake_init_point(MAT_POINT2&             point,
		     const mln::value::rgb8& point1,
		     const mln::value::rgb8& point2,
		     const mln::value::rgb8& point3,
		     const mln::value::rgb8& point4)
{
  set_point(point, 0, point1);
  set_point(point, 1, point2);
  set_point(point, 2, point3);
  set_point(point, 3, point4);
}


void fake_update_group(MAT_GROUP2&     group,
		       const unsigned& point1_min,
		       const unsigned& point2_min,
		       const unsigned& point3_min,
		       const unsigned& point4_min)
{
  set_group(group, 0, point1_min);
  set_group(group, 1, point2_min);
  set_group(group, 2, point3_min);
  set_group(group, 3, point4_min);
}


void fake_init_center(MAT_CENTER&            center,
		      const mln::value::rgb8 center1,
		      const mln::value::rgb8 center2)
{

  set_center(center, 0, center1);
  set_center(center, 1, center2);
}




void fake_update_distance(MAT_DISTANCE2&          distance,
			  const mln::value::rgb8& point1,
			  const mln::value::rgb8& point2,
			  const mln::value::rgb8& point3,
			  const mln::value::rgb8& point4,
			  const mln::value::rgb8& center1,
			  const mln::value::rgb8& center2)
{
  set_distance(distance, 0, dist(point1, center1), dist(point1, center2));
  set_distance(distance, 1, dist(point2, center1), dist(point2, center2));
  set_distance(distance, 2, dist(point3, center1), dist(point3, center2));
  set_distance(distance, 3, dist(point4, center1), dist(point4, center2));

  /*
  for (int i = 0; i < SIZE_SAMPLE2; ++i)
    for (int j = 0; j < NB_CENTER; ++j)
      std::cout << "d(" << i << "," << j << ") = " << distance(i,j) <<std::endl;
  */
}

/// \}

/// Equivalence.
/// \{
/// \brief Test equivalence between point and image, center and color.
///
/// Two kinds of equivalence are defined here. The first one tests the
/// equality between the data cloud and the initial image, while the
/// second one tests the equality between a center and a color.

bool is_equivalent(const mln::image2d<mln::value::rgb8>& img,
		   const MAT_POINT1& point)
{
  mln_piter_(mln::image2d<mln::value::rgb8>) pi(img.domain());
  bool                                       result = true;
  unsigned                                   index  = -1;

  for_all(pi)
  {
    bool     test  = true;

    ++index;

    test = test && (point(index,0) == img(pi).red());
    test = test && (point(index,1) == img(pi).green());
    test = test && (point(index,2) == img(pi).blue());

    if (!test)
    {
      std::cout << pi;
      std::cout << "{r=" << img(pi).red()   << ", ";
      std::cout << "g="  << img(pi).green() << ", ";
      std::cout << "b="  << img(pi).blue()  << "}";
      std::cout << index;
      std::cout << "[r=" << point(index,0)  << ", ";
      std::cout << "g="  << point(index,1)  << ", ";
      std::cout << "b="  << point(index,2)  << "]" << std::endl;

      mln_assertion(test);
    }

    result &= test;
  }

  return result;
}

bool is_equal(const mln::value::rgb8& ref,
	      const MAT_CENTER& center,
	      const unsigned i)
{
  bool result = true;

  result = result && (center(i, 0) - ref.red()   < 1.0);
  result = result && (center(i, 1) - ref.green() < 1.0);
  result = result && (center(i, 2) - ref.blue()  < 1.0);

  return result;
}

/// \}


/// kmean unitary testing
/// \{
/// \brief This part of the code manages the unitary testing.
///
/// Many tests are performed and new kind of unitary testing
/// appears. In fact, we must simulate the previous steps of the line
/// currently testing and so making some fake steps. Bad and deep
/// hacking in data structure are required. We test the instantiation
/// of the kmean object, its initialization (points and centers), its
/// core routines (update_center, update_group, update_distance,
/// update_variance) and the final loop.

void test_instantiation()
{
  mln::clustering::k_mean<SIZE_SAMPLE2,NB_CENTER, DIM_POINT, TYPE_POINT> kmean;

  // test the compilation

  std::cout << "test instantiation   : ok" << std::endl;
}


void test_init_point()
{
  typedef mln::value::rgb8  rgb8;
  mln::image2d<rgb8>        img_ref;

  mln::clustering::k_mean<SIZE_SAMPLE1,NB_CENTER, DIM_POINT, TYPE_POINT> kmean;

  mln::io::ppm::load(img_ref, "/usr/local/share/olena/images/lena.ppm");
  //mln::io::ppm::save(img_ref, "verif.ppm");

  fill_image_with_4colors(img_ref);
  kmean.init_point(img_ref);

  mln_assertion(true == is_equivalent(img_ref, kmean.get_point()));

  std::cout << "Test init point      : ok" << std::endl;
}


void test_init_center()
{
  mln::clustering::k_mean<SIZE_SAMPLE2, NB_CENTER, DIM_POINT, TYPE_POINT> kmean;

  const mln::value::rgb8 lime   = mln::literal::lime;
  const mln::value::rgb8 brown  = mln::literal::brown;
  const mln::value::rgb8 teal   = mln::literal::teal;
  const mln::value::rgb8 purple = mln::literal::purple;

  fake_init_point(kmean.get_point(), lime, brown, teal, purple);
  kmean.init_center();

  mln_assertion(is_equal(lime,   kmean.get_center(), 0)  ||
		is_equal(brown,  kmean.get_center(), 0)  ||
		is_equal(teal,   kmean.get_center(), 0)  ||
		is_equal(purple, kmean.get_center(), 0));

  mln_assertion(is_equal(lime,   kmean.get_center(), 1)  ||
		is_equal(brown,  kmean.get_center(), 1)  ||
		is_equal(teal,   kmean.get_center(), 1)  ||
		is_equal(purple, kmean.get_center(), 1));

  std::cout << "Test init center     : ok" << std::endl;
}


void test_update_distance()
{
  mln::clustering::k_mean<SIZE_SAMPLE2, NB_CENTER, DIM_POINT, TYPE_POINT> kmean;

  const mln::value::rgb8 lime   = mln::literal::lime;
  const mln::value::rgb8 brown  = mln::literal::brown;
  const mln::value::rgb8 teal   = mln::literal::teal;
  const mln::value::rgb8 purple = mln::literal::purple;
  const mln::value::rgb8 c1     = lime;
  const mln::value::rgb8 c2     = purple;
  const MAT_DISTANCE2&   dist_v = kmean.get_distance();

  fake_init_point(kmean.get_point(), lime, brown, teal, purple);
  fake_init_center(kmean.get_center(), c1, c2);
  kmean.update_distance();

  mln_assertion(dist(lime,   c1) == dist_v(0,0));
  mln_assertion(dist(lime,   c2) == dist_v(0,1));
  mln_assertion(dist(brown,  c1) == dist_v(1,0));
  mln_assertion(dist(brown,  c2) == dist_v(1,1));
  mln_assertion(dist(teal,   c1) == dist_v(2,0));
  mln_assertion(dist(teal,   c2) == dist_v(2,1));
  mln_assertion(dist(purple, c1) == dist_v(3,0));
  mln_assertion(dist(purple, c2) == dist_v(3,1));

  std::cout << "Test update distance : ok" << std::endl;
}


void test_update_group()
{
  mln::clustering::k_mean<SIZE_SAMPLE2, NB_CENTER, DIM_POINT, TYPE_POINT> kmean;

  const mln::value::rgb8 lime      = mln::literal::lime;
  const mln::value::rgb8 brown     = mln::literal::brown;
  const mln::value::rgb8 teal      = mln::literal::teal;
  const mln::value::rgb8 purple    = mln::literal::purple;
  const mln::value::rgb8 c1        = lime;
  const mln::value::rgb8 c2        = purple;
  const unsigned         point1_min= 0; // lime   near lime   (c1)
  const unsigned         point2_min= 1; // brown  near purple (c2)
  const unsigned         point3_min= 1; // teal   near purple (c2)
  const unsigned         point4_min= 1; // purple near purple (c2)
  const MAT_GROUP2&      group     = kmean.get_group();

  fake_init_point(kmean.get_point(), lime, brown, teal, purple);
  fake_init_center(kmean.get_center(), c1, c2);
  fake_update_distance(kmean.get_distance(), lime, brown, teal, purple, c1, c2);
  kmean.update_group();

  mln_assertion(0.0 == group(0, 1 - point1_min));
  mln_assertion(1.0 == group(0, point1_min));
  mln_assertion(0.0 == group(1, 1 - point2_min));
  mln_assertion(1.0 == group(1, point2_min));
  mln_assertion(0.0 == group(2, 1 - point3_min));
  mln_assertion(1.0 == group(2, point3_min));
  mln_assertion(0.0 == group(3, 1 - point4_min));
  mln_assertion(1.0 == group(3, point4_min));

  std::cout << "Test update group    : ok" << std::endl;
}


void test_update_center()
{
  mln::clustering::k_mean<SIZE_SAMPLE2, NB_CENTER, DIM_POINT, TYPE_POINT> kmean;

  const mln::value::rgb8 lime      = mln::literal::lime;
  const mln::value::rgb8 brown     = mln::literal::brown;
  const mln::value::rgb8 teal      = mln::literal::teal;
  const mln::value::rgb8 purple    = mln::literal::purple;
  const mln::value::rgb8 c1        = lime;
  const mln::value::rgb8 c2        = purple;
  const unsigned         pt1_min   = 0; // lime   near lime   (c1)
  const unsigned         pt2_min   = 1; // brown  near purple (c2)
  const unsigned         pt3_min   = 1; // teal   near purple (c2)
  const unsigned         pt4_min   = 1; // purple near purple (c2)
  const mln::value::rgb8 mean_c1   = lime;
  const mln::value::rgb8 mean_c2   = (brown+teal+purple)/3;

  fake_init_point(kmean.get_point(), lime, brown, teal, purple);
  fake_init_center(kmean.get_center(), c1, c2);
  fake_update_distance(kmean.get_distance(), lime, brown, teal, purple, c1, c2);
  fake_update_group(kmean.get_group(), pt1_min, pt2_min, pt3_min, pt4_min);
  kmean.update_center();

  mln_assertion(is_equal(mean_c1, kmean.get_center(), 0));
  mln_assertion(is_equal(mean_c2, kmean.get_center(), 1));

  std::cout << "Test update center   : ok" << std::endl;
}


void test_update_variance()
{
  mln::clustering::k_mean<SIZE_SAMPLE2, NB_CENTER, DIM_POINT, TYPE_POINT> kmean;

  const mln::value::rgb8 lime      = mln::literal::lime;
  const mln::value::rgb8 brown     = mln::literal::brown;
  const mln::value::rgb8 teal      = mln::literal::teal;
  const mln::value::rgb8 purple    = mln::literal::purple;
  const mln::value::rgb8 c1        = lime;
  const mln::value::rgb8 c2        = purple;
  const unsigned         pt1_min   = 0; // lime   near lime   (c1)
  const unsigned         pt2_min   = 1; // brown  near purple (c2)
  const unsigned         pt3_min   = 1; // teal   near purple (c2)
  const unsigned         pt4_min   = 1; // purple near purple (c2)
  const double           v1        = 0;
  const double           v2        = dist(purple, brown) + dist(purple, teal);
  const mln::value::rgb8 mean_c2   = (brown+teal+purple)/3;
  const VEC_VAR&         var       = kmean.get_variance();

  fake_init_point(kmean.get_point(), lime, brown, teal, purple);
  fake_init_center(kmean.get_center(), c1, c2);
  fake_update_distance(kmean.get_distance(), lime, brown, teal, purple, c1, c2);
  fake_update_group(kmean.get_group(), pt1_min, pt2_min, pt3_min, pt4_min);
  kmean.update_variance();

  mln_assertion(v1 == var[0]);
  mln_assertion(v2 == var[1]);

  std::cout << "Test update variance : ok" << std::endl;
}


/// \fixme this procedure tests actually nothing.
void test_loop()
{
  typedef mln::value::rgb8  rgb8;
  mln::image2d<rgb8>        img_ref;

  mln::clustering::k_mean<SIZE_SAMPLE1, NB_CENTER, DIM_POINT, TYPE_POINT> kmean;

  mln::io::ppm::load(img_ref, "/usr/local/share/olena/images/lena.ppm");
  //mln::io::ppm::save(img_ref, "verif.ppm");

  fill_image_with_4colors(img_ref);
  kmean.init_point(img_ref);

  kmean.loop(img_ref);

  // \FIXME: Which assertion must we define ?
  //  std::cout << "Test loop        : ok" << std::endl;
}

/// \}

int main()
{
  test_instantiation();
  test_init_point();
  test_init_center();
  test_update_distance();
  test_update_group();
  test_update_center();
  test_update_variance();
  test_loop();

  return 0;
}
