#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/labeling/background.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/value/label_16.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/all.hh>
#include <mln/labeling/compute.hh>
#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/data/fill.hh>
#include "label_maj.hh"
#include "clean_lines.hh"

void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm lines.pgm output.pgm" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << " Rebuild splitted components from a label image." << std::endl
	    << "  input.pbm:   input 2D binary image (text is black; background is white)" << std::endl
	    << "  lines.pgm:   output image where line components are labeled (int_u8)" << std::endl
	    << "	       0 is the background label." << std::endl
	    << " output.pgm    output image where line components are labeled (int_u8)" << std::endl
	    << "	       0 is the background label." << std::endl;
  std::abort();
}


int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace mln::value;

  if (argc < 4)
    usage(argv);

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  image2d<int_u8> lines;
  io::pgm::load(lines, argv[2]);

  // Save result.
  io::pgm::save(clean_lines(input, lines, 0.7f), argv[3]);
}
