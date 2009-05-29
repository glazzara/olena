#include "my_cmy.hh"
#include "rgb_to_cmy.hh"

#include <cmath>

#include <mln/core/image/image2d.hh>
#include <mln/value/rgb.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/math/round.hh>
#include <mln/data/transform.hh>

int main()
{
  using namespace mln;

  image2d<value::rgb8> lena;
  io::ppm::load(lena, "../../../img/lena.ppm");

  image2d< value::cmy_<float, float, float> > lena_hsi 
     = data::transform(lena,
                        fun::v2v::f_rgb_to_cmy_f());

  image2d<value::rgb8> lena_rgb = data::transform(lena_hsi,
						   fun::v2v::f_cmy_to_rgb_3x8);
}

