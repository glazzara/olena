#include <mln/core/image/image2d.hh>
#include <mln/io/tiff/load.hh>
#include <mln/io/pbm/save.hh>
// #include <mln/level/convert.hh>


int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc < 3)
  {
    std::cout << "Usage: " << argv[0] << " <input.tiff> <output.pbm>" << std::endl;
    return 1;
  }

  image2d<bool> ima;
  io::tiff::load(ima, argv[1]);
  io::pbm::save(ima, argv[2]);

//  io::ppm::save(ima, argv[2]);
//  io::pbm::save(level::convert(bool(), ima), argv[2]);
}
