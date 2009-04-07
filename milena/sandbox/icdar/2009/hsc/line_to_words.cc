#include <mln/value/int_u8.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>

#include <sandbox/icdar/2009/hsc/line_to_words.hh>




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " line.pbm output.pgm" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << "  line.pbm:  a 2D image of a line (text is black; background is white)" << std::endl
	    << "  output.pgm: output image where words are labeled (int_u8)" << std::endl
	    << "              0 is the background label." << std::endl;
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

  int_u8 n_words;
  image2d<int_u8> output = line_to_words(input, n_words);

  io::pgm::save(output, argv[2]);


  trace::exiting("main");
}
