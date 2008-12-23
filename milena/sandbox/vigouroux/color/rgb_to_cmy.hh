#include <mln/core/image/image_if.hh>

#include <mln/core/alias/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/data/fill.hh>
#include <mln/value/int_u.hh>

#include "my_cmy.hh"

namespace mln {
  namespace convert {
    struct f_rgb_to_cmy
    {
      struct value::cmy<8>
      doit(const struct value::rgb<8> rgb) const
      {
	struct value::cmy<8> cmy;

	cmy.c(1 - rgb.red());
	cmy.m(1 - rgb.green());
	cmy.y(1 - rgb.blue());

	return (cmy);
      }
    };

    struct f_cmy_to_rgb
    {
      struct value::rgb<8>
      doit(const struct value::cmy<8> cmy) const
      {
	int red;
	int green;
	int blue;

	red = int(1 - cmy.c());
	green = int(1 - cmy.m());
	blue = int(1 - cmy.y());
	struct value::rgb<8> rgb(red, green, blue);

	return (rgb);
      }
    };
  }
}
