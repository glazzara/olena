#include "my_yuv.hh"
#include "rgb_to_yuv.hh"

#include <cmath>

#include <mln/core/image2d.hh>
#include <mln/value/rgb.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/math/round.hh>
#include <mln/level/transform.hh>

int main()
{
  using namespace mln;

  image2d<value::rgb8> lena;
  io::ppm::load(lena, "../../../img/lena.ppm");

  image2d< value::yuv_<double, double, double> > lena_hsi 
     = level::transform(lena,
                        fun::v2v::f_rgb_to_yuv_f());

  image2d<value::rgb8> lena_rgb = level::transform(lena_hsi,
						   fun::v2v::f_yuv_to_rgb_3x8);
}

