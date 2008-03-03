#include <mln/core/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/display/save_and_show.hh>
#include <mln/value/rgb.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/math/round.hh>
#include <mln/level/transform.hh>

#include <mln/core/w_window2d_float.hh>
#include <mln/border/thickness.hh>
#include <mln/linear/convolve.hh>

# include <mln/display/save.hh>
# include <mln/display/show.hh>

int main()
{
  using namespace mln;
  using value::int_u8;
  image2d<mln::value::rgb8> lena;
  io::ppm::load(lena, "../../img/lena.ppm");
//   image2d<mln::value::hsi_3x8>  inter(lena.domain());
  image2d<mln::value::rgb8>  out(lena.domain());
  const image2d<mln::value::rgb8> input = exact(lena);
  
//   mln_piter(image2d<mln::value::rgb8>) p(input.domain());
//   for_all(p)
//   {
//     inter(p) = fun::v2v::f_rgb_to_hsi_3x8(p);
    
//   }

  display::save_and_show (lena, "display", 1);
}

