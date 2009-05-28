#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/make/image3d.hh>
#include <mln/debug/slices_2d.hh>

#include <mln/value/int_u16.hh>
#include <mln/io/dump/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/literal/colors.hh>


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dump dim output.pbm" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u16;

  if (argc != 4)
    usage(argv);

  unsigned dim = atoi(argv[2]);
  if (dim != 2 && dim != 3)
  {
    std::cout << "dimensions invalid" << std::endl;
    return 1;
  }

  if (dim == 2)
  {
    image2d<bool> ima;
    io::dump::load(ima, argv[1]);
    io::pbm::save(ima, argv[3]);
  }
  else
  {
    image3d<bool> vol;
    io::dump::load(vol, argv[1]);

    image2d<bool> ima = debug::slices_2d(vol, 1.f, false);
    io::pbm::save(ima, argv[3]);
  }
}
