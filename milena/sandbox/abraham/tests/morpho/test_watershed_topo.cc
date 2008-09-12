#include <mln/core/image2d.hh>
#include <mln/core/window2d.hh>
#include <mln/core/neighb2d.hh>
#include <mln/core/p_set.hh>

#include <mln/value/int_u8.hh>
#include <mln/level/compare.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/morpho/basic_najman.hh>
#include <string>
#include <iostream>

int main (int argc, const char* argv [])
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> image2dint;

  image2dint input;

  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " in.pgm out.pgm" << std::endl;
    return 1;
  }

  io::pgm::load(input, argv[1]);

  morpho::basic_najman< image2d<int_u8>, neighb2d> n(input, c4());

  n.gotopo();

  io::pgm::save(n.pima, argv[2]);

  return 0;
}
