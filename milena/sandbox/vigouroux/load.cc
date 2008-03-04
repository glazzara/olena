#include <mln/core/image2d.hh>
#include <mln/value/int_u8.hh>
#include "color/my_hsi.hh"
#include "color/rgb_to_hsi.hh"
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

#include <mln/core/image2d.hh>
#include <mln/win/rectangle2d.hh>
#include <mln/make/pixel.hh>

#include <iostream>

template <typename I, typename O, typename W>
void test_fill(I& ima, O& out, W& win)
{
  mln::value::hsi_3x8 c_hsi;
  mln::value::rgb8 c2;
  mln_piter(I) p(out.domain());
  mln_qixter(I, W) q(ima, win, p);
  for_all(p)
    {
      for_all(q)
      {
	c_hsi = mln::fun::v2v::f_rgb_to_hsi_3x8(q.val());
	c2 = mln::fun::v2v::f_hsi_to_rgb_3x8(c_hsi);
      }
      out(p) = c2;
    }
}

int main()
{
  using namespace mln;
  using value::int_u8;
  image2d<mln::value::rgb8> lena;
  io::ppm::load(lena, "../../img/lena.ppm");
  image2d<mln::value::hsi_3x8>  inter(lena.domain());
  image2d<mln::value::rgb8>  out(lena.domain());
  const image2d<mln::value::rgb8> input = exact(lena);

  win::rectangle2d rect(3, 3);
  test_fill(input, out, rect);
  
  display::save_and_show (out, "display", 50);
}

