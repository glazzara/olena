#include <mln/core/concept/function.hh>
#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/dump/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/data/transform.hh>
#include <mln/data/stretch.hh>
#include <mln/arith/minus.hh>

namespace mln
{

  struct color_diff : Function_v2v<color_diff>
  {
    typedef value::rgb8 result;

    value::rgb8 operator()(const int& v) const
    {
      int v_d2 = std::abs(v) * 0.0001;

      if (v < 0)
	return value::rgb8(128 + v_d2, v_d2, v_d2);
      else if (v > 0)
	return value::rgb8(v_d2, 128 + v_d2, v_d2);
      else
	return value::rgb8(255, 255, 255);
    }
  };

}


int main(int argc, char *argv[])
{
  using namespace mln;


  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " in.dump out.ppm" << std::endl;
    return 1;
  }

  image2d<double> input;

  io::dump::load(input, argv[1]);

  color_diff f;
  image2d<value::rgb8> result = data::transform(input, f);

  io::ppm::save(result, argv[2]);
}
