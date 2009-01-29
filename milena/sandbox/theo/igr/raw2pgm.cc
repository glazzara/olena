
#include <mln/core/image/image2d.hh>
#include <mln/make/image3d.hh>
#include <mln/debug/slices_2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/io/raw/load.hh>
#include <mln/io/pgm/save.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.raw output.pgm" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 3)
    usage(argv);

  image3d<int_u8> vol;
  io::raw::load(vol, argv[1]);

  int_u8 bg = 0;
  image2d<int_u8> ima = debug::slices_2d(vol, 1.f, bg);
  io::pgm::save(ima, argv[2]);
}
