#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/magick/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/save.hh>

int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;

  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " filename.ext" << std::endl;
    return 1;
  }

  image2d<bool> lena;

  io::magick::load(lena, argv[1]);
  io::pbm::save(lena, "result.pbm");

  return 0;
}
