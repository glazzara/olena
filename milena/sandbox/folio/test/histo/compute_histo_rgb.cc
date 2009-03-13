/*!
 * \file   compute_histo_rgb.cc<2>
 * \author etiennefolio <ornthalas@gmail.com>
 */

#include <iostream>
#include <mln/debug/println.hh>
#include <mln/literal/all.hh>

#include "../../mln/histo/compute_histo_rgb.hh"

int main()
{
  using namespace mln;

  // build test image
  image2d<value::rgb8> ima(10, 10);

  value::rgb8 red = literal::red;
  value::rgb8 green = literal::green;
  value::rgb8 black = literal::black;

  for (unsigned i = 0; i < 10; ++i)
    for (unsigned j = 5; j < 10; ++j)
    {
      point2d p(j, i);
      ima(p) = black;
    }

  for (unsigned i = 0; i < 10; ++i)
    for (unsigned j = 0; j < 5; ++j)
    {
      point2d p(j, i);
      ima(p) = red;
    }

  point2d p(8, 2);
  ima(p) = green;

  std::cout << "input :" << std::endl;
  debug::println(ima);

  // let's run !
  image3d<value::int_u8> out = histo::compute_histo_rgb<value::int_u8>(ima);

  // output ?
  std::cout << "out(0, 0, 0) = " << out(point3d(0, 0, 0)) << std::endl;
  std::cout << "out(255, 0, 0) = " << out(point3d(255, 0, 0)) << std::endl;
  std::cout << "out(0, 255, 0) = " << out(point3d(0, 255, 0)) << std::endl;

  return 0;
}
