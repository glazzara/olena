#include <mln/essential/2d.hh>
#include <mln/value/hsl.hh>
#include <mln/core/image/violent_cast_image.hh>
#include <mln/extract/all.hh>
#include <mln/value/label_16.hh>
#include <mln/value/int_u8.hh>
#include <mln/morpho/closing_area.hh>
#include <mln/io/dump/save.hh>

mln::value::int_u8 foo(unsigned u)
{
  return u == 0 ?
    0 : // wshed line
    1 + (u - 1) % 255; // basin
}


namespace mln
{

  struct hsl2rgb : Function_v2v<hsl2rgb>
  {
    typedef value::rgb8 result;

    value::rgb8 operator()(const value::hsl_f& v) const
    {
       value::rgb8 res((v.hue() / 360.0f) * 255.0f,
			 v.sat() * 255.0f,
			 v.lum());
       return res;
    }
  };

}



int main(int, char *argv[])
{
  using namespace mln;

  using value::rgb8;
  using value::hsl_f;
  using value::int_u8;
  using value::label_16;

  typedef value::hsl_<short, short, short> hsl_s;
  typedef image2d<rgb8> I;
  typedef image2d<hsl_f> J;

  I input_;
  io::ppm::load(input_, argv[1]);
  J input = level::convert(hsl_f(), input_);

  std::cout << "input_rgb(p) = " << input_(point2d(310,0)) << " - input_hsl(p) = " << input(point2d(310,0))
	    << " hsl2rgb(p) = " << hsl2rgb()(input(point2d(310,0)))
	    << std::endl;
  std::cout << "input_rgb(p) = " << input_(point2d(311,0)) << " - input_hsl(p) = " << input(point2d(311,0))
	    << " hsl2rgb(p) = " << hsl2rgb()(input(point2d(311,0)))
	    << std::endl;

  /// Compute the gradient on each component of the HSL image.
  /// Then keep the max values in the three images.
  image2d<float> grad_hue;
  initialize(grad_hue, input);
  data::fill(grad_hue, extract::hue(input));
  image2d<float> grad_sat;
  initialize(grad_sat, input);
  data::fill(grad_sat, extract::sat(input));
  image2d<float> grad_lum;
  initialize(grad_lum, input);
  data::fill(grad_lum, extract::lum(input));
  grad_hue = morpho::gradient(grad_hue, win_c4p());
  grad_sat = morpho::gradient(grad_sat, win_c4p());
  grad_lum = morpho::gradient(grad_lum, win_c4p());

  image2d<value::int_u8> grad;
  initialize(grad, grad_hue);
  mln_piter_(image2d<value::int_u8>) p(grad.domain());
  for_all(p)
    grad(p) = math::max(grad_hue(p), math::max(grad_sat(p), grad_lum(p)));
  io::pgm::save(grad, "hsl2rgb_grad.pgm");

  image2d<int_u8> clo = morpho::closing_area(grad, c4(), 10);
  io::pgm::save(clo, "hsl2rgb_clo_a100.pgm");

  label_16 nbasins;
  image2d<label_16> wshed = morpho::meyer_wst(clo, c4(), nbasins);
  io::dump::save(wshed, "hsl2rgb_wshed.dump");
  std::cout << "nbasins = " << nbasins << std::endl;
  io::pgm::save(level::transform(wshed, convert::to_fun(foo)),
		"hsl2rgb_wshed.pgm");

  I out = level::transform(input, hsl2rgb());
  io::ppm::save(out, "hsl2rgb_out.ppm");

}
