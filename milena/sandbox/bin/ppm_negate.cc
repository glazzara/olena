#include <mln/core/concept/function.hh>
#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/ppm/all.hh>
#include <mln/arith/revert.hh>
#include <mln/data/transform.hh>

namespace mln
{

  struct rgb_negate : Function_v2v<rgb_negate>
  {
    typedef value::rgb8 result;

    result operator()(const value::rgb8& v) const
    {
      value::rgb8 tmp(255 - v.red(), 255 - v.green(), 255 - v.blue());
      return tmp;
    }

  };

} // end of namespace mln



int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc < 3)
  {
    std::cout << "Usage: " << argv[0] << " input.ppm output.ppm"
	      << std::endl;
    return 1;
  }

  image2d<value::rgb8> input;
  io::ppm::load(input, argv[1]);

  io::ppm::save(data::transform(input, rgb_negate()), argv[2]);
}

