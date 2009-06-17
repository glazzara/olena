/*!
 * \file   compute_histo_rgb.cc<2>
 * \author etiennefolio <ornthalas@gmail.com>
 */

#include <iostream>
#include <mln/literal/all.hh>
#include <mln/value/rgb.hh>
#include <mln/value/int_u8.hh>

#include "../../mln/histo/compute_histo_3d.hh"

int main()
{
  using namespace mln;
  using namespace value;
  typedef rgb<3> rgb3;

  // build test image
  image2d<rgb3> ima(8, 8);

  rgb3 red = literal::red;
  rgb3 green = literal::green;
  rgb3 black = literal::black;

  for (unsigned i = 1; i < 8; ++i)
    for (unsigned j = 0; j < 8; ++j)
    {
      point2d p(j, i);
      ima(p) = black;
    }

  for (unsigned i = 0; i < 8; ++i)
  {
    point2d p(i, 0);
    ima(p) = red;
  }

  point2d p(4, 5);
  ima(p) = green;

  // build histo
  image3d<unsigned> out = histo::compute_histo_3d(ima);

  // verify...
  mln_assertion(out(point3d(255, 0, 0)) == 8);
  mln_assertion(out(point3d(0, 255, 0)) == 1);
  mln_assertion(out(point3d(0, 0, 0)) == 55);
  mln_assertion(out(point3d(1, 0, 0)) == 0);
}
