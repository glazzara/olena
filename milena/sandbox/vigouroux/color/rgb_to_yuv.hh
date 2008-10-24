#include <mln/core/image/image_if.hh>

#include <mln/core/alias/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/value/int_u.hh>

#include "my_yuv.hh"

namespace mln {
  namespace convert {
    struct f_rgb_to_yuv
    {
      struct mln::value::yuv<8>
      doit(const struct value::rgb<8> rgb) const
      {
	struct value::yuv<8> yuv;

	yuv.y(0.299 * rgb.red() + 0.587 * rgb.green() + 0.114 * rgb.blue());
	yuv.u(0.436 * (rgb.blue() - yuv.y()) / (1 - 0.114));
	yuv.v(0.615 * (rgb.red() - yuv.y()) / (1 - 0.299));

	return (yuv);
      }
    };

    struct f_yuv_to_rgb
    {
      struct value::rgb<8>
      doit(const struct value::yuv<8> yuv) const
      {
	int r;
	int g;
	int b;

	r = int(yuv.y() + 1.13983 * yuv.v());
	g = int(yuv.y() - 0.39465 * yuv.u() - 0.58060 * yuv.v());
	b = int(yuv.y() + 2.03211 * yuv.u());

	struct value::rgb<8> rgb(r, g, b);

	return (rgb);
      }
    };
  }
}
