#ifndef OLENA_CONVERT_NRGBHSV_HH
# define OLENA_CONVERT_NRGBHSV_HH

# include <mln/core/image_if_value.hh>
# include <mln/core/image/inplace.hh>
# include <mln/core/w_window2d_int.hh>
# include <mln/display/show.hh>
# include <mln/io/ppm/save.hh>
# include <mln/display/save_and_show.hh>
# include <mln/level/fill.hh>

# include "my_hsv.hh"

namespace mln {
  namespace convert {
    struct f_rgb_to_hsv
    {
      struct value::hsv<8>
      doit(const struct value::rgb<8> rgb) const
      {
	struct value::hsv<8> hsv;

	hsv.h();
	hsv.s();
	hsv.v();

	return (hsv);
      }
    };
    
    struct f_hsv_to_rgb
    {
      struct value::rgb<8>
      doit(const struct value::hsv<8> hsv) const
      {
	struct value::rgb<8> rgb;
	int r;
	int g;
	int b;

	double sqrt3_3 = sqrt(3) / 3;
	double inv_sqrt6 = 1 / sqrt(6);
	double inv_sqrt2 = 1 / sqrt(2);

	double alpha = inv_sqrt2 * rgb.green() - inv_sqrt2 * rgb.blue();
	double beta = 2 * inv_sqrt6 * rgb.red() - inv_sqrt6 * rgb.green() - inv_sqrt6 * rgb.blue();

	r = int(sqrt3_3 * hsv.i() + 2 * inv_sqrt6 * beta);
	g = int(sqrt3_3 * hsv.i() + inv_sqrt2 * alpha - inv_sqrt6 * beta);
	b = int(sqrt3_3 * hsv.i() - inv_sqrt2 * alpha - inv_sqrt6 * beta);

	struct value::rgb<8> rgb(r, g, b); 
	
	return (rgb);
      }
    };
  }
}

#endif // OLENA_CONVERT_RGBHSV_HH
