/*!
 * \file   compute_histo_rgb_from_ppm.cc
 * \author etiennefolio <ornthalas@gmail.com>
 */

#include <iostream>
#include <mln/debug/println.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/level/stretch.hh>

#include "../../mln/histo/compute_histo_rgb.hh"

int main()
{
  using namespace mln;

  // build test image
  image2d<value::rgb8> ima;
  io::ppm::load(ima, "../../../../img/lena.ppm");

  // let's run !
  image3d<value::int_u8> out = histo::compute_histo_rgb<value::int_u8>(ima);

  // output ?
  // FIXME: need projection to visualize.

  return 0;
}
