#include <sandbox/icdar/2009/hsc/clean_input.hh>
#include <sandbox/icdar/2009/hsc/input_to_ws.hh>
#include <sandbox/icdar/2009/hsc/ws_to_wsl.hh>
#include <sandbox/icdar/2009/hsc/io/icdar/save.hh>

#include <mln/world/binary_2d/subsample.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label.hh>
#include <mln/value/rgb8.hh>
#include <mln/debug/colorize.hh>
#include <mln/labeling/relabel.hh>

#include <mln/level/transform.hh>
#include <mln/fun/v2v/enc.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/core/image/violent_cast_image.hh>






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


  // First clean.
  input = clean_input(input);


  // Sub-sample.
  image2d<int_u8> small = world::binary_2d::subsample(input, 4);

  typedef value::int_u<12> L;

  L n_lines;
  image2d<L>
    // Get text regions.
    ws    = input_to_ws(input, small, n_lines),
    // Merge regions.
    wsl   = ws_to_wslines(input, small, ws, n_lines);

  typedef value::label<12> L_;
  L_ n_lines_;
  image2d<L_> wsl_ = labeling::relabel(violent_cast_image_<L_>(wsl),
				       n_lines_);

  io::icdar::save(wsl_, argv[2]);

  if (argc == 4)
    {
      io::ppm::save(debug::colorize(value::rgb8(),
				    wsl_,
				    n_lines_),
		    argv[3]);
    }

  trace::exiting("main");
}
