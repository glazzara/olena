#include <iostream>
#include <sstream>
#include <string>
#include <mln/debug/println.hh>
#include <mln/data/fill.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/level/convert.hh>
#include <mln/level/stretch.hh>

#include <mln/core/alias/window2d.hh>

#include "dilation-n.hh"
#include "erosion-n.hh"

int main(int argc, char** argv)
{
  using namespace mln;

  // check arguments
  if (argc != 5)
  {
    std::cerr << "Usage:" << std::endl
	      << "  ./a.out <in.pbm> <out.pbm> <k> <lambda>" << std::endl;
    exit(1);
  }

  // build test image
  std::cout << "  => loading " << argv[1] << "..." << std::endl;
  image2d<bool> in;
  io::pbm::load(in, argv[1]);

  value::int_u8 k = 1;
  image2d<value::int_u8> ima = level::convert(k, in);
  ima = level::stretch(k, ima);

  std::cout << "  => create window..." << std::endl;
  window2d it;
  it.insert(0, 0);
  for (unsigned i = 1; i <= atoi(argv[3]); ++i)
  {
    it.insert(0, -i);
    it.insert(0, i);
  }

  std::cout << "  => applying dilation..." << std::endl;
  image2d<value::int_u8> out;
  out = morpho::erosion_n(ima, it, atoi(argv[4]));

  std::cout << "  => saving " << argv[2] << "..." << std::endl;
  io::pgm::save(out, argv[2]);
}
