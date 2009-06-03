#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>

#include "color_internal_gradient.hh"

#include <iostream>

void usage(char** argv)
{
  std::cout << "Usage: " << argv[0] << " input.ppm [gradient.pgm]"
	    << std::endl;
  abort();
}


int main(int argc, char** argv)
{
  if (argc < 2)
    usage(argv);

  const char* finput = argv[1];
  const char* foutput = (argc == 3) ? argv[2] : "gradient.pgm";

  using namespace mln;
  using value::int_u8;
  using value::rgb8;

  typedef image2d<rgb8> I;
  typedef image2d<int_u8> O;

  I input;
  io::ppm::load(input, finput);

  O output = color_internal_gradient(input);
  io::pgm::save(output, foutput);
}

