#include <mln/core/image/image2d.hh>

#include <mln/io/ppm/all.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/hsl.hh>

#include <mln/convert/from_to.hh>
#include <mln/data/convert.hh>

using namespace mln;
using value::rgb8;
using value::hsl_;


bool
is_green(hsl_<float, float, float> value)
{
  if (value.hue() > 75 && value.hue() < 170 &&
      value.sat() > 0.5 &&
      value.lum() > 0.3 && value.lum() < 0.7)
    return true;

  return false;
}

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " input output" << std::endl;
    return 1;
  }

  image2d<rgb8> input;
  io::ppm::load(input, argv[1]);

  typedef image2d<hsl_<float, float, float> > H;
  H ima_hsl = data::convert(hsl_<float, float, float>(), input);
  mln_piter_(H) p(ima_hsl.domain());
  for_all(p)
  {
    if (!is_green(ima_hsl(p)))
    {
      ima_hsl(p).sat() = 0.3;
      ima_hsl(p).lum() = 0.3;
    }
  }

  image2d<rgb8> output = data::convert(rgb8(), ima_hsl);
  io::ppm::save(output, argv[2]);

  return 0;
}
