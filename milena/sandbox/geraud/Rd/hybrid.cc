#include <iostream>

#include <mln/core/image2d.hh>
#include <mln/core/neighb2d.hh>
#include <mln/value/int_u8.hh>

#include <mln/debug/println.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include "hybrid.hh"


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " f.pgm g.pgm c output.pgm" << std::endl
            << "reconstruction by dilation (hybrid version; may 2007)" << std::endl
            << "f = marker (to be dilated)" << std::endl
            << "g = mask (constraint >= f)" << std::endl
            << "c: 4 or 8" << std::endl;
  exit(1);
}


int main(int argc, char* argv[])
{
  if (argc != 5)
    usage(argv);

  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> I;

  int c = atoi(argv[3]);
  if (c != 4 and c != 8)
    usage(argv);

  I f = io::pgm::load<int_u8>(argv[1]);
  I g = io::pgm::load<int_u8>(argv[2]);
  
  if (not (f <= g))
    {
      std::cerr << "pb" << std::endl;
      return 1;
    }

  io::pgm::save(morpho::Rd::hybrid(f, g,
				   (c == 4 ? c4() : c8())),
		argv[4]);
}
