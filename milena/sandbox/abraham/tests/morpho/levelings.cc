#include <mln/core/image/image2d.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/value/int_u8.hh>
#include <mln/morpho/vector_median.hh>
#include <mln/morpho/vmt.hh>
#include <mln/morpho/autarkical_leveling.hh>
#include <mln/fun/meta/red.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/mixin.hh>
#include <mln/core/image/dmorph/fun_image.hh>
#include <mln/core/image/vmorph/violent_cast_image.hh>

int main (int argc, const char * argv[])
{
  using namespace mln;
  using namespace value;

  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " in.pgm [other_files.pgm]" << std::endl;
    return 1;
  }

  for (int i = 1; i < argc; ++i)
    {
      border::thickness = 2;

      image2d<rgb8> ima;
      io::ppm::load(ima, argv[i]);

      std::string name(argv[i]);
      name.erase(name.length() - 4);

      win::rectangle2d smallrect(13, 13);
      win::rectangle2d bigrect(25, 25);

      image2d<rgb8> ima_vm = morpho::vector_median(ima, bigrect);

      io::ppm::save(ima_vm, name.append("_vector_medianed.ppm"));

      name = argv[i];
      name.erase(name.length() - 4);


      io::ppm::save(morpho::autarkical_leveling(ima, ima_vm, smallrect), name.append("_autarkical_leveled.ppm"));

    }
  return 0;
}
