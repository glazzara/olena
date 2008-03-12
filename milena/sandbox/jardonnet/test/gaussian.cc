#include <mln/core/image2d.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/linear/gaussian.hh>

#include <mln/algebra/vec.hh>

int main(int, char*)
{
  using namespace mln;

  image2d< value::rgb8 > img;
  io::ppm::load(img, "../../../img/lena.ppm");
  image2d< value::rgb8 > out(img.domain());

  linear::gaussian(img, .5, out);

  io::ppm::save(out, "./+gau.ppm");
}
