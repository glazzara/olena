#include "filetype.hh"

#include <mln/world/binary_2d/subsample.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm n output.pgm" << std::endl
	    << "  Subsampling." << std::endl
	    << "  n >= 2." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 4)
    usage(argv);


  trace::entering("main");

  int n = std::atoi(argv[2]);
  if (n < 2)
    {
      std::cerr << "bad n!" << std::endl;
      usage(argv);
    }


  image2d<bool> ima;
  io::pbm::load(ima, argv[1]);
  io::pgm::save(world::binary_2d::subsample(ima,
					    n),
		argv[3]);

  trace::exiting("main");
}
