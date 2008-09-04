#include <mln/core/image/image2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <sandbox/jardonnet/subsampling/gaussian_subsampling.hh>


int main(int argc, char*)
{
  using namespace mln;
  using value::int_u8;
  
  image2d<int_u8> img;

  io::pgm::load(img, "../../../img/lena.pgm");

  image2d<int_u8> output = subsampling::gaussian_subsampling(img, 0.1, make::dpoint2d(1,1), argc);
  
  io::pgm::save(output, "gsub.pgm");
}
