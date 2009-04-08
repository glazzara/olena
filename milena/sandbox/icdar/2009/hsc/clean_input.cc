#include <sandbox/icdar/2009/hsc/clean_input.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>






void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm output.pbm" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << "  input.pbm:  input 2D binary image (text is black; background is white)" << std::endl
	    << "  output.pbm: cleaned input." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 3)
    usage(argv);


  trace::entering("main");


  image2d<bool> input;

  io::pbm::load(input, argv[1]);
  io::pbm::save(clean_input(input),
		argv[2]);

  trace::exiting("main");
}
