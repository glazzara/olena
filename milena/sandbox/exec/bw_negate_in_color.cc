//
// Invert black and white in a color image.
//

#include <mln/essential/2d.hh>

namespace mln
{

  struct bw_negate_in_color
    : Function_v2v<bw_negate_in_color>
  {
    typedef value::rgb8 result;

    value::rgb8 operator()(const value::rgb8& v) const
    {
      if (v == literal::white)
	return literal::black;
      else if (v == literal::black)
	return literal::white;
      else
	return v;
    }

  };

}


int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc < 3)
  {
    std::cout << "Usage: " << argv[0] << " <input.ppm> <out.ppm>"
	      << std::endl;
    return 1;
  }

  image2d<value::rgb8> input;
  io::ppm::load(input, argv[1]);

  bw_negate_in_color f;
  io::ppm::save(data::transform(input, f), argv[2]);
}
