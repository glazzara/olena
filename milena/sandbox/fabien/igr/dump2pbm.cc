
#include <mln/core/image/image2d.hh>
#include <mln/make/image3d.hh>
#include <mln/debug/slices_2d.hh>

#include <mln/value/int_u16.hh>
#include <mln/io/dump/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/literal/colors.hh>


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dump output.pbm" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u16;

  if (argc != 3)
    usage(argv);

  image3d<bool> vol;
  io::dump::load(vol, argv[1]);

  int_u16 bg = 0;
  image2d<bool> ima = debug::slices_2d(vol, 1.f, bg);
  io::pbm::save(ima, argv[2]);
}
