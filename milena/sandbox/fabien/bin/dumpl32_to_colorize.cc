#include <mln/core/image/image2d.hh>
#include <mln/make/image3d.hh>
#include <mln/debug/slices_2d.hh>
#include <mln/labeling/colorize.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_32.hh>
#include <mln/value/rgb8.hh>
#include <mln/core/concept/literal.hh>

#include <mln/io/dump/load.hh>
#include <mln/io/ppm/save.hh>


int usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dump dim nbasins output.ppm" << std::endl;
  return 1;
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::int_u12;
  using value::label_32;
  using value::rgb8;

  if (argc != 5)
    return usage(argv);

  unsigned dim = atoi(argv[2]);
  unsigned nbasins = atoi(argv[3]);
  if (dim != 2 && dim != 3)
  {
    std::cout << "<dimensions> invalid" << std::endl;
    return 1;
  }

  if (dim == 2)
  {
    image2d<label_32> ima2d;
    io::dump::load(ima2d, argv[1]);
    image2d<rgb8> ima_rgb = labeling::colorize(rgb8(), ima2d, nbasins);
    io::ppm::save(ima_rgb, argv[4]);
  }
  else
  {
    image3d<label_32> ima3d;
    io::dump::load(ima3d, argv[1]);
    image3d<rgb8> ima_rgb = labeling::colorize(rgb8(), ima3d, nbasins);

    image2d<rgb8> ima_result = debug::slices_2d(ima_rgb, 1.f, literal::black);
    io::ppm::save(ima_result, argv[4]);
  }

  return 0;
}
