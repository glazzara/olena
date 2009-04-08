#include <mln/value/int_u16.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/debug/colorize.hh>

#include <sandbox/icdar/2009/hsc/clean_input.hh>
#include <sandbox/icdar/2009/hsc/line_to_words.hh>
#include <sandbox/icdar/2009/hsc/io/icdar/save.hh>




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm output.dat [output.ppm]" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << "  input.pbm:  a raw binary 2D image (either a line or a page)" << std::endl
	    << "  output.dat: output image where words are labeled (int_u8)" << std::endl
	    << "              0 is the background label." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u16;

  if (argc != 3 && argc != 4)
    usage(argv);


  trace::entering("main");


  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  input = clean_input(input);


  int_u16 n_words;
  image2d<int_u16> output = line_to_words(input, n_words);

  io::icdar::save(output, argv[2]);

  if (argc == 4)
    {
      io::ppm::save(debug::colorize(value::rgb8(),
				    output,
				    n_words),
		    argv[3]);
    }

  trace::exiting("main");
}
