#include <mln/core/image2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <sandbox/jardonnet/subsampling/subsampling_2d.hh>


int main(int argc, char*)
{
  using namespace mln;
  using value::int_u8;
  
  image2d<int_u8> img;

  io::pgm::load(img, "../../../img/lena.pgm");

  std::cout << geom::nrows(img) << geom::ncols(img)  << std::endl;
  image2d<int_u8> output = subsampling::subsampling_2d(img, make::dpoint2d(1,1), argc);
  
  io::pgm::save(output, "out1.pgm");
}
