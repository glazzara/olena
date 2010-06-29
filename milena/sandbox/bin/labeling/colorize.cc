#include <mln/core/concept/function.hh>
#include <mln/core/image/image2d.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/label_8.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/load.hh>

#include <mln/labeling/colorize.hh>


int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc < 3)
    std::cout << "Usage : " << argv[0]
	      << " input_label_8.pgm out.ppm" << std::endl;

  image2d<value::label_8> input;
  io::pgm::load(input, argv[1]);

  io::ppm::save(labeling::colorize(value::rgb8(), input), argv[2]);
}
