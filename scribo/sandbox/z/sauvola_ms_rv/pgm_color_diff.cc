#include <mln/core/concept/function.hh>
#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/data/transform.hh>
#include <mln/arith/minus.hh>

namespace mln
{

  struct color_diff : Function_v2v<color_diff>
  {
    typedef value::rgb8 result;

    color_diff(int threshold) : threshold_(threshold) {}

    value::rgb8 operator()(const int& v) const
    {
      int v_d2 = std::abs(v / 2);

      if (v >= -threshold_ && v <= threshold_)
	return value::rgb8(255, 255, 255);
      else if (v > threshold_)
	return value::rgb8(v_d2, 128 + v_d2, v_d2);
      else
	return value::rgb8(128 + v_d2, v_d2, v_d2);
    }


    int threshold_;
  };

}


int main(int argc, char *argv[])
{
  using namespace mln;


  if (argc != 5)
  {
    std::cout << "Usage: " << argv[0] << " in.pgm ref.pgm threshold out.ppm" << std::endl;
    return 1;
  }

  image2d<value::int_u8> input, ref;

  io::pgm::load(input, argv[1]);
  io::pgm::load(ref, argv[2]);

  image2d<int> diff = input - ref;

  color_diff f(atoi(argv[3]));
  image2d<value::rgb8> result = data::transform(diff, f);

  io::ppm::save(result, argv[4]);
}
