#include <mln/core/image/image2d.hh>

#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/all.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/hsl.hh>

#include <mln/test/conversion.hh>

using namespace mln;
using value::rgb8;
using value::hsl_;
using value::int_u8;


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " input" << std::endl;
    return 1;
  }

  image2d<rgb8> input;
  io::ppm::load(input, argv[1]);

  float delta = test::conversion(input, hsl_<float, float, float>());

  std::cout << "delta conversion = " << delta << std::endl;

  return 0;
}
