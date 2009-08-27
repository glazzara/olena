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

  print_color(lime);
  print_color(brown);
  print_color(teal);
  print_color(purple);
}

#define SIZE_IMAGE  512
#define SIZE_SAMPLE (512*512)
#define NB_CENTER   2
#define DIM_POINT   3
#define TYPE_POINT  double
#define MAT_POINT   mln::algebra::mat<SIZE_SAMPLE, DIM_POINT, TYPE_POINT>
#define MAT_CENTER  mln::algebra::mat<NB_CENTER, DIM_POINT, TYPE_POINT>

bool is_equivalent(const mln::image2d<mln::value::rgb8>& img,
		   const MAT_POINT& point)
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

  mln::clustering::k_mean<SIZE_SAMPLE,NB_CENTER, DIM_POINT, TYPE_POINT> kmean;

  mln::io::ppm::load(img_ref, "/usr/local/share/olena/images/lena.ppm");
  //mln::io::ppm::save(img_ref, "verif.ppm");

  fill_image_with_4colors(img_ref);
  kmean.init_point(img_ref);
  
  mln_assertion(true == is_equivalent(img_ref, kmean.get_point()));

  std::cout << "Test init point : ok" << std::endl;
}

bool is_equal(const mln::value::rgb8& ref, const MAT_CENTER& center, unsigned i)
{
  bool result = true;

  result = result && (ref.red() == center(i, 0));
  result = result && (ref.green() == center(i, 1));
  result = result && (ref.blue() == center(i, 2));

  return result;
}

bool is_center_initialized(const MAT_CENTER& center)
{
  typedef mln::value::rgb8  rgb8;
  const rgb8                lime   = mln::literal::lime;
  const rgb8                brown  = mln::literal::brown;
  const rgb8                teal   = mln::literal::teal;
  const rgb8                purple = mln::literal::purple;
  bool                      result = false;

  for (unsigned i = 0; i < NB_CENTER; ++i)
  {
    result = result || is_equal(lime, center, i);
    result = result || is_equal(brown, center, i);
    result = result || is_equal(teal, center, i);
    result = result || is_equal(purple, center, i);
  }

  return result;
}

void test_init_center()
{
  typedef mln::value::rgb8  rgb8;
  mln::image2d<rgb8>        img_ref;

  mln::clustering::k_mean<SIZE_SAMPLE,NB_CENTER, DIM_POINT, TYPE_POINT> kmean;

  mln::io::ppm::load(img_ref, "/usr/local/share/olena/images/lena.ppm");

  fill_image_with_4colors(img_ref);
  kmean.init_point(img_ref);
  kmean.init_center();

  mln_assertion(true == is_center_initialized(kmean.get_center()));
  std::cout << "Test init center : ok" << std::endl;
}

void test_instantiation()
{

  std::cout << "test_instantiation" << std::endl;
  
  mln::trace::entering("safe");
  //  typedef mln::value::int_u8 int_u8;
  typedef mln::value::rgb8 rgb8;
  const   unsigned           SIZE = 512*512;

  //mln::image2d<int_u8>        img_ref;
  mln::image2d<rgb8>        img_ref;

  mln::io::ppm::load(img_ref, "/usr/local/share/olena/images/lena.ppm");

  mln::trace::exiting("safe");
  mln::trace::entering("clustering");
  mln::clustering::k_mean<SIZE,2,3,double> kmean;

  std::cout << img_ref.domain() << std::endl;
  kmean.init_point(img_ref);
  kmean.init_center();
  kmean.update_distance();
  kmean.update_group();
  kmean.update_center();
  mln::trace::exiting("clustering");
  /*
  int_u8 val;
  */
  /*
  mln::trait::value::print<mln::value::int_u8>(std::cout);
  mln::trait::image::print<mln::image2d<int_u8> >();

  mln::image2d<int_u8>        img_out;


  //mln::io::pgm::load(img_ref, "mp00082c_50p.pgm");
  mln::trace::exiting("image");
  

  mln::trace::entering("test");

  mln::trace::exiting("test");
  //  k_mean.distance();
  */
}

void test_col()
{
}

void test_row()
{
}

int main()
{
  //test_instantiation();
  test_init_point();
  test_init_center();

  return 0;
}
