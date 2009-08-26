// UNARY TESTS ON K_MEAN

#include "k_mean.hh"

#include <iostream>


#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include <mln/algebra/vec.hh>
#include <mln/algebra/mat.hh>

#include <mln/core/image/image2d.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/trait/value/print.hh>
#include <mln/trait/image/print.hh>

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
  std::cout << "main()" << std::endl;
  mln::trace::quiet = false;
  std::cout << "main::trace" << std::endl;
  test_instantiation();

  return 0;
}
