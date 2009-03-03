
#include <mln/core/image/image2d.hh>
#include <mln/make/image3d.hh>
#include <mln/debug/slices_2d.hh>

#include <mln/value/rgb8.hh>
#include <mln/io/dump/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/literal/colors.hh>


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dump output.ppm" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::rgb8;

  if (argc != 3)
    usage(argv);

  image3d<rgb8> vol;
  io::dump::load(vol, argv[1]);

  rgb8 bg = literal::black;
  image2d<rgb8> ima = debug::slices_2d(vol, 1.f, bg);
  io::ppm::save(ima, argv[2]);
}
