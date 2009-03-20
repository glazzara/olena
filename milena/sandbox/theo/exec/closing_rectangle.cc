#include "filetype.hh"

#include <mln/morpho/closing/structural.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm height width output.pgm" << std::endl
	    << "  Height and width are odd positive integers." << std::endl
	    << "  Rectangle closing." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  trace::entering("main");

  if (argc != 5)
    usage(argv);

  int height = atoi(argv[2]);
  if (height < 0 || height % 2 == 0)
    usage(argv);

  int width  = atoi(argv[3]);
  if (width < 0 || width % 2 == 0)
    usage(argv);

  image2d<int_u8> ima, clo;
  io::pgm::load(ima, argv[1]);

  clo = morpho::closing::structural(ima, win::rectangle2d(height, width));
  io::pgm::save(clo, argv[4]);

  trace::exiting("main");
}
