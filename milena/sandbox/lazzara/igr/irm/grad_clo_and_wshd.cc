#include <iostream>
#include <mln/core/image/image2d.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/image/image_if.hh>

#include <mln/io/ppm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/dump/save.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>

#include <mln/level/transform.hh>

#include <mln/labeling/mean_values.hh>

#include <mln/convert/to_fun.hh>

#include <mln/morpho/gradient.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/meyer_wst.hh>

#include <mln/fun/l2l/wrap.hh>

#include <mln/core/var.hh>
#include <mln/morpho/elementary/dilation.hh>

#include <mln/core/routine/extend.hh>

mln::value::int_u8 foo(unsigned u)
{
  return u == 0 ?
    0 : // wshed line
    1 + (u - 1) % 255; // basin
}


int main(int argc, char *argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;
  using value::label_16;

  if (argc < 4)
  {
    std::cout << "Usage: " << argv[0] << " <ima.pgm> <ima.ppm> <closure_lambda>"
	      << std::endl;
    return 1;
  }

  image2d<int_u8> irm;
  io::pgm::load(irm, argv[1]);

  image2d<rgb8> irm_color;
  io::ppm::load(irm_color, argv[2]);


  image2d<int_u8> grad = morpho::gradient(irm, win_c4p());
  io::pgm::save(grad, "tmp_grad_c4p.pgm");

  image2d<int_u8> clo = morpho::closing::area(grad, c4(), atoi(argv[3]));
  io::pgm::save(clo, "tmp_clo_a100.pgm");

  label_16 nbasins;
  image2d<label_16> wshed = morpho::meyer_wst(clo, c4(), nbasins);
  std::cout << "nbasins = " << nbasins << std::endl;

  io::pgm::save(level::transform(wshed, fun::l2l::wrap<int_u8>()),
		"tmp_wshed.pgm");


  mln_VAR(vol2_,
      morpho::elementary::dilation(extend(wshed | (pw::value(wshed) == 0u),
					  wshed),
				    c8()));

  data::fill((wshed | (pw::value(wshed) == 0u)).rw(), vol2_);

  io::ppm::save(labeling::mean_values(irm_color, wshed, nbasins),
		"tmp_wshed_mean_colors.ppm");

  data::fill((irm_color | (pw::value(wshed) == 0u)).rw(), literal::yellow);

  io::ppm::save(irm_color, "tmp_wshed_color.ppm");
  io::dump::save(wshed, "tmp_wshed.dump");

}
