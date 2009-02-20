
#include <mln/core/image/image2d.hh>
#include <mln/make/image3d.hh>
#include <mln/debug/slices_2d.hh>

#include <mln/value/int_u16.hh>
#include <mln/io/dump/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/literal/colors.hh>


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dump output.pgm" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u16;

  if (argc != 3)
    usage(argv);

  image3d<int_u16> vol;
  io::dump::load(vol, argv[1]);

  int_u16 bg = 0;
  image2d<int_u16> ima = debug::slices_2d(vol, 1.f, bg);
  io::pgm::save(ima, argv[2]);
}
