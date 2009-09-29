#include <mln/core/image/image2d.hh>
#include <mln/core/image/tiled2d.hh>

#include <mln/io/ppm/load.hh>
#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>

#include <mln/debug/quiet.hh>

#include "bench.hh"


using namespace mln;
using value::rgb8;


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " input" << std::endl;
    return 1;
  }

  tiled2d<rgb8> tiled_ima;

  io::ppm::load(tiled_ima, argv[1]);

  bench_disk_perf(tiled_ima);

  return 0;
}
