#include <mln/core/image/image2d.hh>
#include <mln/make/image3d.hh>
#include <mln/debug/slices_2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>

#include <mln/io/dump/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/level/stretch.hh>


int usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dump dim output.pgm" << std::endl;
  return 1;
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::int_u12;
  using value::rgb8;

  if (argc != 4)
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
    image2d<int_u12> ima2d;
    io::dump::load(ima2d, argv[1]);
    image2d<int_u8> ima_pgm = level::stretch(int_u8(), ima2d);
    io::pgm::save(ima_pgm, argv[3]);
  }
  else
  {
    image3d<int_u12> ima3d;
    io::dump::load(ima3d, argv[1]);
    image3d<int_u8> ima_pgm = level::stretch(int_u8(), ima3d);

    image2d<int_u8> ima_result = debug::slices_2d(ima_pgm, 1.f, 0);
    io::pgm::save(ima_result, argv[3]);
  }

  return 0;
}
