#include <mln/core/image/image2d.hh>

#include <mln/io/ppm/all.hh>
#include <mln/value/rgb8.hh>

#include <mln/upsampling/hq3x.hh>


using namespace mln;
using value::rgb8;


int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " input output" << std::endl;
    return 1;
  }

  image2d<rgb8> ima;
  io::ppm::load(ima, argv[1]);

  image2d<rgb8> hq3x_ima;
  hq3x_ima = upsampling::hq3x(ima);
  io::ppm::save(hq3x_ima, argv[2]);

  return 0;
}
