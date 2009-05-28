#include "filetype.hh"

#include <mln/histo/compute.hh>
#include <mln/debug/histo.hh>

#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/all.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm [mask.pbm] output.txt" << std::endl
	    << "  Compute an histogram." << std::endl
	    << "  Input images are 2D." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 3 && argc != 4)
    usage(argv);

  trace::entering("main");

  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);

  histo::array<int_u8> h;
  if (argc == 3)
    h = histo::compute(input);
  else
    {
      image2d<bool> mask;
      io::pbm::load(mask, argv[2]);
      h = histo::compute(input | pw::value(mask));
    }
  debug::histo(h, argv[argc - 1]);

  trace::exiting("main");
}
