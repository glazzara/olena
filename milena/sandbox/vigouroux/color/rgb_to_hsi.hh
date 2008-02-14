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
	
	hsi.h(0);  // not yet implemented
	hsi.s(0);  // not yet implemented
	hsi.i(0);  // not yet implemented

	return (hsi);
      }
    };
    
    struct f_hsi_to_rgb
    {
      struct value::rgb<8>
      doit(const struct value::hsi<8> hsi) const
      {
	struct value::rgb<8> rgb;

	rgb.red() = 0;		// not yet implemented
	rgb.green() = 0;	// not yet implemented
	rgb.blue() = 0;		// not yet implemented
	
	return (rgb);
      }
    };
  }
}
