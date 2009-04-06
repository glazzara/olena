#include "filetype.hh"

#include <mln/fun/vv2v/diff_abs.hh>
#include <mln/level/transform.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input1.pgm input2.pgm output.pgm" << std::endl
	    << "  Point-wise diff abs." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 4)
    usage(argv);


  trace::entering("main");

  image2d<int_u8> input1, input2;
  io::pgm::load(input1, argv[1]);
  io::pgm::load(input2, argv[2]);

  io::pgm::save(level::transform(input1,
				 input2,
				 fun::vv2v::diff_abs<int_u8>()),
		argv[3]);

  trace::exiting("main");
}
