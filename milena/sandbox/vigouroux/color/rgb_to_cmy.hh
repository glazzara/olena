#include <mln/core/image_if_value.hh>
#include <mln/core/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>

#include "my_cmy.hh"

namespace mln {
  namespace convert {
    struct f_rgb_to_cmy
    {
      struct value::cmy
      doit(const struct value::rgb<8> rgb) const
      {
	struct value::cmy cmy;
	
	cmy.c(1 - rgb.red());
	cmy.m(1 - rgb.green());
	cmy.y(1 - rgb.blue());

	return (cmy);
      }
    };
    
    struct f_cmy_to_rgb
    {
      struct value::rgb<8>
      doit(const struct value::cmy cmy) const
      {
	struct value::rgb<8> rgb;

	rgb.red() = 1 - cmy.c();
	rgb.green() = 1 - cmy.m();
	rgb.blue() = 1 - cmy.y();
	
	return (rgb);
      }
    };
  }
}
