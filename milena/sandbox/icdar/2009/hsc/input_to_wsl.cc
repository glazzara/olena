#include <sandbox/icdar/2009/hsc/input_to_ws.hh>
#include <sandbox/icdar/2009/hsc/ws_to_wsl.hh>

#include <mln/world/binary_2d/subsample.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/labeling/colorize.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>






void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm output.pgm [output.ppm]" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << "  input.pbm:  input 2D binary image (text is black; background is white)" << std::endl
	    << "  output.pgm: output image where line components are labeled (int_u8)" << std::endl
	    << "              0 is the background label." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;

  if (argc != 3 && argc != 4)
    usage(argv);


  trace::entering("main");


  image2d<bool> input;
  io::pbm::load(input, argv[1]);


  int_u8 n_lines;

  image2d<int_u8>
    small = world::binary_2d::subsample(input, 4),
    ws    = input_to_ws(input, small, n_lines),
    wsl   = ws_to_wslines(input, small, ws, n_lines);


  io::pgm::save(wsl, argv[2]);

  if (argc == 4)
    {
      io::ppm::save(labeling::colorize(value::rgb8(),
				    wsl,
				    n_lines),
		    argv[3]);
    }


  trace::exiting("main");
}
