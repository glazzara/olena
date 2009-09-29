#include <mln/core/image/image2d.hh>

#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/all.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/hsl.hh>
#include <mln/value/int_u8.hh>

#include <mln/convert/from_to.hh>
#include <mln/data/convert.hh>
#include <mln/math/diff_abs.hh>
#include <mln/math/min.hh>

using namespace mln;
using value::rgb8;
using value::hsl_;
using value::int_u8;

#define GREEN 120.f
#define BLUE  240.f


inline
float
hue_truth(hsl_<float, float, float> value, float hue, float range)
{
  float truth = 0.0;

  if (value.hue() > (hue - range) && value.hue() < (hue + range))
  {
    truth += 1.f - ((math::diff_abs(hue, value.hue()) / 100.0) / (range / 100));
  }

  if (truth > 1.f)
    truth = 1.f;
  if (truth < 0.f)
    truth = 0.f;
  return truth;
}

inline
float
sat_truth(hsl_<float, float, float> value)
{
  return value.sat();
}

inline
float
lum_truth(hsl_<float, float, float> value)
{
  return 1.f - 2 * (math::diff_abs(0.5f, value.lum()));
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
  image2d<int_u8> hue_ima;
  initialize(hue_ima, input);
  image2d<int_u8> sl_p_ima;
  initialize(sl_p_ima, input);
  image2d<int_u8> sl_m_ima;
  initialize(sl_m_ima, input);

  typedef image2d<hsl_<float, float, float> > H;
  H ima_hsl = data::convert(hsl_<float, float, float>(), input);
  mln_piter_(H) p(ima_hsl.domain());
  for_all(p)
  {
    float truth = 0.0;
    truth = hue_truth(ima_hsl(p), GREEN, 60.f) * sat_truth(ima_hsl(p)) * lum_truth(ima_hsl(p));
    output(p) = (int_u8) (truth * 255);

    hue_ima(p) = (int_u8) (hue_truth(ima_hsl(p), GREEN, 60.f) * 255);
    sl_p_ima(p) = (int_u8) (sat_truth(ima_hsl(p)) * lum_truth(ima_hsl(p)) * 255);
    sl_m_ima(p) = (int_u8) (math::min(sat_truth(ima_hsl(p)), lum_truth(ima_hsl(p))) * 255);
  }

  io::pgm::save(output, argv[2]);
  io::pgm::save(hue_ima, "hue.pgm");
  io::pgm::save(sl_p_ima, "sl_p.pgm");
  io::pgm::save(sl_m_ima, "sl_m.pgm");

  return 0;
}
