#include <mln/core/image/image2d.hh>

#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/all.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/hsl.hh>
#include <mln/value/int_u8.hh>

#include <mln/convert/from_to.hh>
#include <mln/data/convert.hh>
#include <mln/math/diff_abs.hh>

using namespace mln;
using value::rgb8;
using value::hsl_;
using value::int_u8;


inline
bool
is_green(hsl_<float, float, float> value)
{
  if (value.hue() > 75 && value.hue() < 170 &&
      value.sat() > 0.5 &&
      value.lum() > 0.3 && value.lum() < 0.7)
    return true;

  return false;
}

inline
int_u8
how_green(hsl_<float, float, float> value)
{
  float proba = 0.0;

  if (value.hue() > 60 && value.hue() < 180)
  {
    proba += 1.f - ((math::diff_abs(120.f, value.hue()) / 100.0) / 0.6);
    proba *= value.sat();
    proba *= 1.f - (math::diff_abs(0.5f, value.lum()));
  }

  if (proba > 1.f)
    proba = 1.f;
  return (int_u8) (proba * 255);
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
  image2d<int_u8> output;
  initialize(output, input);

  typedef image2d<hsl_<float, float, float> > H;
  H ima_hsl = data::convert(hsl_<float, float, float>(), input);
  mln_piter_(H) p(ima_hsl.domain());
  for_all(p)
  {
    output(p) = how_green(ima_hsl(p));
  }

  io::pgm::save(output, argv[2]);

  return 0;
}
