#include <mln/core/image/image2d.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/value/int_u8.hh>
#include <mln/morpho/erosion.hh>
#include <mln/fun/meta/red.hh>
#include <mln/value/rgb8.hh>
#include <mln/core/image/dmorph/fun_image.hh>

int main (int argc, const char * argv[])
{
  using namespace mln;
  using namespace value;

  typedef image2d<int_u8> image2dint;

  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " in.pgm [other_files.pgm]" << std::endl;
    return 1;
  }

  for (int i = 1; i < argc; ++i)
    {
      border::thickness = 2;

      image2d<rgb8> ima;
      io::ppm::load(ima, argv[i]);

      mln::fun_image<meta::red <rgb8>, image2d<rgb8> > red(ima);

      std::string name(argv[i]);
      name.erase(name.length() - 4);

      win::rectangle2d rect(5, 5);
      morpho::erosion(red, rect);

      io::ppm::save(ima, name.append("_red_eroded.pgm"));

    }
  return 0;
}
