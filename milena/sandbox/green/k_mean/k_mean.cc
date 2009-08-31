// UNARY TESTS ON K_MEAN

#include "k_mean.hh"

#include <iostream>

#include <mln/pw/value.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include <mln/literal/colors.hh>

#include <mln/algebra/vec.hh>
#include <mln/algebra/mat.hh>

#include <mln/core/macros.hh>
#include <mln/core/contract.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/data/transform.hh>

#include <mln/trait/value/print.hh>
#include <mln/trait/image/print.hh>

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


void test_instantiation()
{
  mln::clustering::k_mean<SIZE_SAMPLE2,NB_CENTER, DIM_POINT, TYPE_POINT> kmean;

  // test the compilation

  std::cout << "test instantiation   : ok" << std::endl;
}

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

void set_point(MAT_POINT2&             point,
	       const unsigned          index, 
	       const mln::value::rgb8& color)
{
  point(index,0) = color.red();
  point(index,1) = color.green();
  point(index,2) = color.blue();
}

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

void set_center(MAT_CENTER&             center,
		const unsigned          index,
		const mln::value::rgb8& color)
{
  center(index,0) = color.red();
  center(index,1) = color.green();
  center(index,2) = color.blue();
}

void fake_init_center(MAT_CENTER&            center,
		      const mln::value::rgb8 center1,
		      const mln::value::rgb8 center2)
{

  set_center(center, 0, center1);
  set_center(center, 1, center2);
}


double dist(mln::value::rgb8 color1, mln::value::rgb8 color2)
{
  double red   = color1.red()   - color2.red();
  double green = color1.green() - color2.green();
  double blue  = color1.blue()  - color2.blue();
  double result= red * red + green * green + blue * blue;

  return result;
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

void set_distance(MAT_DISTANCE2& distance,
		  const unsigned index,
		  const double   d1,
		  const double   d2)
{
  distance(index,0) = d1;
  distance(index,1) = d2;
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

void set_group(MAT_GROUP2& group,
	       const unsigned index,
	       const unsigned min)
{
  group(index, min)   = 1.0;
  group(index, 1-min) = 0.0;
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

void test_update_var()
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

  std::cout << "Test update variance: ok" << std::endl;
}


int main()
{
  test_instantiation();
  test_init_point();
  test_init_center();
  test_update_distance();
  test_update_group();
  test_update_center();
  test_update_var();
  
  //  mln::trace::quiet = false;

  test_update_center();

  return 0;
}
