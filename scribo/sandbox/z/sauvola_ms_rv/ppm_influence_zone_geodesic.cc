#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/rgb8.hh>
#include <mln/transform/influence_zone_geodesic.hh>
#include <mln/io/ppm/all.hh>


int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 3)
  {
    std::cout << argv[0] << " input.ppm output.ppm" << std::endl;
    return 1;
  }

  image2d<value::rgb8> input;
  io::ppm::load(input, argv[1]);

  image2d<value::rgb8> iz = transform::influence_zone_geodesic(input, c8());

  io::ppm::save(iz, argv[2]);
}
