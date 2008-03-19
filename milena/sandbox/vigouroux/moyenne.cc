#include "color/my_hsi.hh"
#include "color/rgb_to_hsi.hh"
#include <mln/display/save_and_show.hh>
#include <mln/value/rgb.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/math/round.hh>
#include <mln/level/transform.hh>

#include <mln/core/image2d.hh>
#include <cmath>

#include <iostream>

template <typename I, typename O>
float rms (const I& ima, O& out)
{
  mln::value::rgb8 c1;
  mln::value::rgb8 c2;
  float distred = 0;
  float distgreen = 0;
  float distblue = 0;
  float sum = 0;
  float nb = 0;
  float moy = 0;

  mln_piter(I) p(out.domain());
  for_all(p)
  {
    c1 = ima(p);
    c2 = out(p);
    distred = c2.red() - c1.red();
    distgreen = c2.green() - c1.green();
    distblue = c2.blue() - c1.blue();

    ++nb;
    sum += distred * distred + distblue * distblue + distgreen * distgreen;
  }
  moy = std::sqrt(sum / nb);
  return (moy);
}

int main()
{
  using namespace mln;
  using value::int_u8;
  image2d<value::rgb8> lena;
  io::ppm::load(lena, "../../img/lena.ppm");

  image2d<value::hsi_3x8> lena_hsi = level::transform(lena,
					       fun::v2v::f_rgb_to_hsi_3x8);

  image2d<value::rgb8>  lena_rgb = level::transform(lena_hsi,
					       fun::v2v::f_hsi_to_rgb_3x8);


  float err = rms(lena, lena_rgb);

  std::cout << "err: " << err << std::endl;

  display::save_and_show (lena_rgb, "display", 50);
  return (0);
}

