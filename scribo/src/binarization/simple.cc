#include <scribo/binarization/simple.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/save.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm output.pbm" << std::endl
	    << "  Simple binarization in SCRIBO." << std::endl;
  std::abort();
}



int main(int argc, char *argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 3)
    usage(argv);

  trace::entering("main");

  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);


  io::pbm::save(scribo::binarization::simple(input),
		argv[2]);


  trace::exiting("main");
}
