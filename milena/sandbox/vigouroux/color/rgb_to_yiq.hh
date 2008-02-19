#include <mln/core/image_if_value.hh>
#include <mln/core/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/value/int_u.hh>

#include "my_yiq.hh"

namespace mln {
  namespace convert {
    struct f_rgb_to_yiq
    {
      struct mln::value::yiq<8>
      doit(const struct value::rgb<8> rgb) const
      {
	struct value::yiq<8> yiq;
	
	yiq.y(0.1768 * rgb.red() + 0.8130 * rgb.green() + 0.0101 * rgb.blue());
	yiq.i(0.5346 * rgb.red() - 0.2461 * rgb.green() - 0.1791 * rgb.blue());
	yiq.q(0.2474 * rgb.red() - 0.6783 * rgb.green() + 0.4053 * rgb.blue());

	return (yiq);
      }
    };
    
    struct f_yiq_to_rgb
    {
      struct value::rgb<8>
      doit(const struct value::yiq<8> yiq) const
      {
	int r;
	int g;
	int b;

	r = int(0.87 * yiq.y() + 1.3223 * yiq.i() + 0.5628 * yiq.q());
	g = int(1.026 * yiq.y() - 0.2718 * yiq.i() - 0.1458 * yiq.q());
	b = int(1.186 * yiq.y() - 1.2620 * yiq.i() + 1.8795 * yiq.q());

	struct value::rgb<8> rgb(r, g, b); 

	return (rgb);
      }
    };
  }
}
