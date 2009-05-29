# include <cstdlib>
# include <cstdio>

# include <mln/value/int_u16.hh>
# include <mln/value/int_u8.hh>
# include <mln/data/stretch.hh>

# include <mln/io/pgm/load.hh>
# include <mln/io/ppm/save.hh>

# include <mln/core/var.hh>
# include <mln/core/image/image3d.hh>
# include <mln/core/alias/neighb3d.hh>

# include <mln/morpho/meyer_wst.hh>
# include <mln/morpho/closing_area.hh>
# include <mln/morpho/closing_volume.hh>



using namespace mln;
using value::int_u8;


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.raw lambda output.ppm" << std::endl;
  std::cerr << "  lambda >= 0" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::int_u16;

  if (argc != 3)
    usage(argv);

  trace::quiet = false;

  image3d<int_u16> ima16(256, 256, 256,  0);
  int size = 256 * 256 * 256 * 2;

  std::FILE* file = std::fopen(argv[1], "r");
  std::fread(ima16.buffer(), size, 1, file);

//   typedef image3d<int_u16> I;
//   mln_piter_(I) p(ima16.domain());
//   for_all(p)
//     if (ima16(p) != 0)
//       std::cout << ima16(p) << std::endl;

//   abort();

  int_u16 min_, max_;
  estim::min_max(ima16, min_, max_);
  std::cout << min_ << ' ' << max_ << std::endl;

  abort();

  image3d<int_u8> ima(ima16.domain());
  data::stretch(ima16, ima);
  ima16.destroy();

  int lambda = atoi(argv[2]);
  if (lambda < 0)
    usage(argv);

  image3d<int_u8> clo(ima.domain());
  morpho::closing_volume(ima, c6(), lambda, clo);

  unsigned nbasins;
  image3d<unsigned> wst = morpho::meyer_wst(clo, c6(), nbasins);
  std::cout << "nbasins = " << nbasins << std::endl;
}
