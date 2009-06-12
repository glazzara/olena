#include <mln/labeling/compute.hh>
#include <mln/util/array.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/literal/colors.hh>
#include <mln/data/convert.hh>

#include <mln/value/label_8.hh>
#include <mln/estim/min_max.hh>
#include <mln/core/image/vmorph/violent_cast_image.hh>
#include <mln/core/var.hh>

#include <scribo/draw/bounding_boxes.hh>


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm output.ppm" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << "  input.pgm:  labeled lines" << std::endl
	    << "  output.ppm: with red boxes" << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::label_8;
  using value::rgb8;

  if (argc != 3)
    usage(argv);

  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);

  int_u8 dummy, n_boxes;
  estim::min_max(input, dummy, n_boxes);

  label_8 n = unsigned(n_boxes);

  mln_VAR(input_, violent_cast_image_<label_8>(input));
  util::array<box2d> boxes = labeling::compute(accu::meta::shape::bbox(),
					       input_,
					       n);
 
  image2d<rgb8> out = data::convert(rgb8(), input);
  scribo::draw::bounding_boxes(out, boxes, literal::red);
  io::ppm::save(out, argv[2]);
}
