#include "my_yiq.hh"
#include "rgb_to_yiq.hh"

#include <cmath>

#include <mln/core/image/image2d.hh>
#include <mln/value/rgb.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/math/round.hh>
#include <mln/data/transform.hh>

#include "get_red.hh"

int main()
{
//    typedef mln::value::yiq_d Col;
   using namespace mln::fun::v2v;

   mln::value::yiq_d col;
   mln::image2d<mln::value::rgb8> lena;
   mln::io::ppm::load(lena, "../../../img/lena.ppm");

  mln::image2d<mln::value::yiq_d> lena_yiq = mln::data::transform(lena,
						    mln::fun::v2v::f_rgb_to_yiq_d());

  mln::image2d<mln::value::rgb8> lena_rgb = mln::data::transform(lena_yiq,
						  f_yiq_3x8);
}
