#include "my_cmy.hh"
#include "rgb_to_cmy.hh"

#include <cmath>

#include <mln/core/image/image2d.hh>
#include <mln/value/rgb.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/math/round.hh>
#include <mln/level/transform.hh>

#include "fun.hh"

int main()
{
//    typedef mln::value::cmy_d Col;
   using namespace mln::fun::v2v;

   mln::value::cmy_d col;
   mln::image2d<mln::value::rgb8> lena;
   mln::io::ppm::load(lena, "../../../img/lena.ppm");

  mln::image2d<mln::value::cmy_d> lena_cmy = mln::level::transform(lena,
						    mln::fun::v2v::f_rgb_to_cmy_d());

  mln::image2d<mln::value::rgb8> lena_rgb = mln::level::transform(lena_cmy,
						  f_cmy_3x8);
}
