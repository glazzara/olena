#include <mln/value/hsi.hh>
#include "color/rgb_to_hsi.hh"

#include <cmath>

#include <mln/core/image2d.hh>
#include <mln/value/rgb.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/math/round.hh>
#include <mln/level/transform.hh>

#include "function.hh"
#include "yuv/my_yuv.hh"
#include "yuv/fun.hh"
#include "yuv/rgb_to_yuv.hh"

int main()
{
  using namespace mln::fun::v2v;

   mln::value::hsi_d col;
   mln::image2d<mln::value::rgb8> lena;
   mln::io::ppm::load(lena, "../../img/lena.ppm");

  mln::image2d<mln::value::yuv_f> lena_hsi = mln::level::transform(lena,
								   mln::fun::v2v::f_rgb_to_yuv_f());

  mln::image2d<mln::value::rgb8> lena_rgb = mln::level::transform(lena_hsi,
						  f_get_red_<mln::value::rgb8>());
}
