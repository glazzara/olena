#include <mln/value/hsi.hh>
#include "color/rgb_to_hsi.hh"

#include <cmath>

#include <mln/core/image/image2d.hh>
#include "rgb.hh"

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/math/round.hh>
#include <mln/level/transform.hh>



template <typename I1, typename I2>
float rms(const mln::Image<I1>& ima1_, const mln::Image<I2>& ima2_)
{
  const I1& ima1 = exact(ima1_);
  const I2& ima2 = exact(ima2_);

  mln_precondition(ima1.is_valid() && ima2.is_valid());

  double sum = 0, nb = 0;

  mln_piter(I1) p(ima1.domain());
  for_all(p)
  {
    mln_value(I1) c1 = ima1(p);
    mln_value(I2) c2 = ima2(p);
    double
      distred = c2.red() - c1.red(),
      distgreen = c2.green() - c1.green(),
      distblue = c2.blue() - c1.blue();

    ++nb;
    sum += distred * distred + distblue * distblue + distgreen * distgreen;
  }

  if (nb == 0)
    return 0;

  return std::sqrt(sum / nb);
}


int main()
{
  using namespace mln;

  image2d<value::rgb8> lena;
  io::ppm::load(lena, "../../img/lena.ppm");

  image2d<value::hsi_f> lena_hsi = level::transform(lena,
						    fun::v2v::f_rgb_to_hsi_f);

  image2d<value::rgb8> lena_rgb = level::transform(lena_hsi,
						   fun::v2v::f_hsi_to_rgb_3x8);

  double err = rms(lena, lena_rgb);
  std::cout << "err: " << err << std::endl;
}

