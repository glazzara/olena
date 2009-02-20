#include <mln/essential/2d.hh>

#include <mln/core/image/violent_cast_image.hh>

#include <mln/extract/all.hh>

#include <mln/value/label_16.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/hsl.hh>

#include <mln/morpho/closing_area.hh>
#include <mln/morpho/closing_volume.hh>

#include <mln/io/dump/save.hh>
#include <mln/io/ppm/all.hh>

#include <mln/labeling/mean_values.hh>

#include <mln/fun/l2l/wrap.hh>


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

int main(int argc, char *argv[])
{
  using namespace mln;

  using value::rgb8;
  using value::hsl_f;
  using value::int_u8;
  using value::label_16;

  typedef value::hsl_<short, short, short> hsl_s;
  typedef image2d<rgb8> I;
  typedef image2d<hsl_f> J;

  if (argc < 3)
  {
    std::cout << "Usage: " << argv[0] << " <ima.ppm> <closure_lambda>"
	      << std::endl;
  }

  I input_;
  io::ppm::load(input_, argv[1]);
  J input = level::convert(hsl_f(), input_);
  io::ppm::save(level::transform(input, hsl2rgb()), "input-hsl.ppm");

  {
    I tmp = level::transform(input, hsl2rgb());

    image2d<int_u8> grad_hue;
    initialize(grad_hue, input);
    data::fill(grad_hue, extract::blue(tmp));
    image2d<int_u8> grad_sat;
    initialize(grad_sat, input);
    data::fill(grad_sat, extract::green(tmp));
    image2d<int_u8> grad_lum;
    initialize(grad_lum, input);
    data::fill(grad_lum, extract::blue(tmp));


    grad_hue = morpho::gradient(grad_hue, win_c4p());
    io::pgm::save(grad_hue, "hsl2rgb_grad_hue.pgm");

    grad_sat = morpho::gradient(grad_sat, win_c4p());
    io::pgm::save(grad_sat, "hsl2rgb_grad_sat.pgm");

    grad_lum = morpho::gradient(grad_lum, win_c4p());
    io::pgm::save(grad_lum, "hsl2rgb_grad_lum.pgm");

    image2d<value::int_u8> grad;
    initialize(grad, grad_hue);
    mln_piter_(image2d<value::int_u8>) p(grad.domain());
    for_all(p)
      grad(p) = math::max(grad_hue(p), math::max(grad_sat(p), grad_lum(p)));
    io::pgm::save(grad, "hsl2rgb_grad_rgb.pgm");

  }

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
//  io::pgm::save(grad_hue, "hsl2rgb_grad_hue.pgm");

  grad_sat = morpho::gradient(grad_sat, win_c4p());
//  io::pgm::save(grad_sat, "hsl2rgb_grad_sat.pgm");

  grad_lum = morpho::gradient(grad_lum, win_c4p());
//  io::pgm::save(grad_lum, "hsl2rgb_grad_lum.pgm");

  image2d<value::int_u8> grad;
  initialize(grad, grad_hue);
  mln_piter_(image2d<value::int_u8>) p(grad.domain());
  for_all(p)
    grad(p) = math::max(grad_hue(p), math::max(grad_sat(p), grad_lum(p)));
  io::pgm::save(grad, "hsl2rgb_grad.pgm");

  image2d<int_u8> clo = morpho::closing_area(grad, c4(), 10);
//  image2d<int_u8> clo = morpho::closing_volume(grad, c4(), atoi(argv[2]));
  io::pgm::save(clo, "hsl2rgb_clo_a100.pgm");

  label_16 nbasins;
  image2d<label_16> wshed = morpho::meyer_wst(clo, c4(), nbasins);
  io::dump::save(wshed, "hsl2rgb_wshed.dump");
  std::cout << "nbasins = " << nbasins << std::endl;

  io::pgm::save(level::transform(wshed, fun::l2l::wrap<int_u8>()),
		"hsl2rgb_wshed.pgm");
  io::ppm::save(labeling::mean_values(level::transform(input, hsl2rgb()), wshed, nbasins),
		"hsl2rgb_wshed_mean_colors.ppm");


  I out = level::transform(input, hsl2rgb());
  io::ppm::save(out, "hsl2rgb_out.ppm");

}
