#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/morpho/attribute/height.hh>
#include <mln/morpho/closing/leveling.hh>

#include <mln/arith/minus.hh>

#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm h output.pgm" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 4)
    usage(argv);

  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);

  int i = std::atoi(argv[2]);
  int_u8 h = i;

  morpho::attribute::height< image2d<int_u8> > acc;
  image2d<int_u8> clo = morpho::closing::leveling(input, c4(),
						  acc, h);
  io::pgm::save(clo, "tmp.pgm");
  arith::minus_inplace(clo, input);

  io::pgm::save(clo, argv[3]);
}
