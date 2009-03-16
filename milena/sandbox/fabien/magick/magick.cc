#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>

#include "load.hh"

int main(int argc, char* argv[])
{
  using namespace mln;
  using value::rgb8;

  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " filename.ext" << std::endl;
    return 1;
  }

  image2d<rgb8> lena;

  io::magick::load(lena, argv[1]);

  return 0;
}
