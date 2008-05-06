#include "rgbto.hh"
#include "../color/rgb_to_hsi.hh"
#include "rgbto.hh"
#include "torgb.hh"

#include <cmath>

#include <mln/core/image2d.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/math/round.hh>
#include <mln/level/transform.hh>


int main(void)
{
//   mln::value::cmy_<float, float, float> cmy(0, 0, 0);
//   mln::value::rgb<8> rgb;

//   rgb = mln::fun::v2v::f_rgb_to_< mln::value::cmy_<float, float, float> >(cmy);
  using namespace mln;

  image2d<value::rgb8> lena;
  io::ppm::load(lena, "../../../img/lena.ppm");

  image2d<value::hsi_f> lena_hsi =
    level::transform(lena, fun::v2v::f_rgb_to_<value::hsi_f> ());

  image2d<value::rgb8> lena_rgb = 
    level::transform(lena_hsi, fun::v2v::f_to_rgb_< value::rgb<8> > ());
}

