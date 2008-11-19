#include "my_yuv.hh"
#include "rgb_to_yuv.hh"

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
//    typedef mln::value::yuv_d Col;
   using namespace mln::fun::v2v;

   mln::value::yuv_d col;
   mln::image2d<mln::value::rgb8> lena;
   mln::io::ppm::load(lena, "../../../img/lena.ppm");

  mln::image2d<mln::value::yuv_d> lena_yuv = mln::level::transform(lena,
						    mln::fun::v2v::f_rgb_to_yuv_d());

  mln::image2d<mln::value::rgb8> lena_rgb = mln::level::transform(lena_yuv,
						  f_yuv_3x8);
}
