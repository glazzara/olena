#include <sandbox/icdar/2009/hsc/input_to_lines.hh>
#include <sandbox/icdar/2009/hsc/io/icdar/save.hh>

#include <mln/value/rgb8.hh>
#include <mln/labeling/colorize.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm output.dat [output.ppm]" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << "  input.pbm:  input 2D binary image (text is black; background is white)" << std::endl
	    << "  output.dat: line buffer (int; 0 is bg)." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 3 && argc != 4)
    usage(argv);

  trace::entering("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  typedef value::label<12> L;
  L n_lines;
  image2d<L> output = input_to_lines(input, n_lines, 0.7); // with 70%

  io::icdar::save(output, argv[2]);

  if (argc == 4)
    {
      io::ppm::save(labeling::colorize(value::rgb8(),
				    output,
				    n_lines),
		    argv[3]);
    }

  trace::exiting("main");
}
