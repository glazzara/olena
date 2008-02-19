#include <mln/core/image_if_value.hh>
#include <mln/core/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>

#include "my_hsi.hh"

namespace mln {
  namespace convert {
    struct f_rgb_to_hsi
    {
      struct value::hsi<8>
      doit(const struct value::rgb<8> rgb) const
      {
	struct value::hsi<8> hsi;
	double sqrt3_3 = sqrt(3) / 3;
	double inv_sqrt6 = 1 / sqrt(6);
	double inv_sqrt2 = 1 / sqrt(2);

	double alpha = inv_sqrt2 * rgb.green() - inv_sqrt2 * rgb.blue();
	double beta = 2 * inv_sqrt6 * rgb.red() - inv_sqrt6 * rgb.green() - inv_sqrt6 * rgb.blue();

	hsi.h(atan2(beta, alpha) / 3.1415 * 180.0);
	if (hsi.h() < 0)
	  hsi.h(hsi.h() + 360.0);
	mln_precondition(hsi.h() >= 0);
	hsi.s(sqrt(alpha * alpha + beta * beta));
	hsi.i(sqrt3_3 * rgb.red() + sqrt3_3 * rgb.green() + sqrt3_3 * rgb.blue());

	return (hsi);
      }
    };
    
    struct f_hsi_to_rgb
    {
      struct value::rgb<8>
      doit(const struct value::hsi<8> hsi) const
      {
	int r;
	int g;
	int b;

	double sqrt3_3 = sqrt(3) / 3;
	double inv_sqrt6 = 1 / sqrt(6);
	double inv_sqrt2 = 1 / sqrt(2);

	double h = hsi.h() / 180.0 * 3.1415;
	double alpha = hsi.s() * cos(h);
	double beta = hsi.s() * sin(h);

	r = int(sqrt3_3 * hsi.i() + 2 * inv_sqrt6 * beta);
	g = int(sqrt3_3 * hsi.i() + inv_sqrt2 * alpha - inv_sqrt6 * beta);
	b = int(sqrt3_3 * hsi.i() - inv_sqrt2 * alpha - inv_sqrt6 * beta);

	struct value::rgb<8> rgb(r, g, b); 
	
	return (rgb);
      }
    };
  }
}
