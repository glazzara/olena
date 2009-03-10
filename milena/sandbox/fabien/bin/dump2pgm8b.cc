#include <mln/core/image/image2d.hh>
#include <mln/make/image3d.hh>
#include <mln/debug/slices_2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/io/dump/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/literal/colors.hh>


int usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dump output.pgm" << std::endl;
  return 1;
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::rgb8;
  using value::int_u8;

  if (argc != 3)
    return usage(argv);

  image3d<int_u8> vol;
  io::dump::load(vol, argv[1]);

  rgb8 bg = literal::black;
  image2d<int_u8> ima = debug::slices_2d(vol, 1.f, 0);
  io::pgm::save(ima, argv[2]);

  return 0;
}
