#include <mln/core/image/image2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>

#include <mln/io/pgm/all.hh>
#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/all.hh>

#include <mln/data/convert.hh>
#include <mln/labeling/relabel.hh>

#include "get_line_images.hh"

void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << "lines.pgm nlines output.pgm" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << " Split lines into words." << std::endl
	    << "  input.pbm:   input 2D binary image (text is black; background is white)" << std::endl
	    << "  lines.pgm:   output image where line components are labeled (int_u8)" << std::endl
	    << "	       0 is the background label." << std::endl
	    << " output.pgm:   output image where line components are labeled (int_u8)" << std::endl
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

  image2d<int_u8> lines_;
  io::pgm::load(lines_, argv[2]);
  image2d<label_16> lines = data::convert(label_16(), lines_);

  label_16 nlines;
  labeling::relabel_inplace(lines, nlines);

  util::array<image2d<bool> > result = get_line_images(lines, nlines, 10);

  io::pbm::save(result[2], argv[3]);
}
