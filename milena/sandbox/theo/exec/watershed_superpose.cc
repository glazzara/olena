#include "filetype.hh"

#include <mln/morpho/watershed/superpose.hh>
#include <mln/data/fill.hh>
#include <mln/pw/all.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm watershed.pgm output.ppm" << std::endl
	    << "  Superpose the watershed line over the input image." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 4)
    usage(argv);

  trace::entering("main");

  image2d<int_u8> ima, ws;
  io::pgm::load(ima, argv[1]);
  io::pgm::load(ws,  argv[2]);

  io::ppm::save(morpho::watershed::superpose(ima, ws),
		argv[3]);

  trace::exiting("main");
}
