#include <mln/core/image/tiled2d.hh>

#include <mln/io/ppm/load.hh>
#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>

#include <mln/debug/quiet.hh>


using namespace mln;
using value::rgb8;


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " input output" << std::endl;
    return 1;
  }

  tiled2d<rgb8> tiled_ima;

  io::ppm::load(tiled_ima, argv[1]);

  std::cout << "bbox: " << tiled_ima.bbox() << std::endl;
  /*mln_piter_(tiled2d<rgb8>) p(tiled_ima.domain());
  for_all(p)
  {
    if (p.col() % 64 == 0)
    {
      tiled_ima(p) = literal::purple;
    }
  }*/

  return 0;
}
