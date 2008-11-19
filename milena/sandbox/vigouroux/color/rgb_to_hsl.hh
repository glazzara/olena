#ifndef OLENA_CONVERT_NRGBHSL_HH
# define OLENA_CONVERT_NRGBHSL_HH

# include <mln/core/image_if_value.hh>

# include <mln/core/alias/w_window2d_int.hh>
# include <mln/display/show.hh>
# include <mln/io/ppm/save.hh>
# include <mln/display/save_and_show.hh>
# include <mln/level/fill.hh>

# include "my_hsl.hh"

namespace mln
{

  namespace fun
  {

    namespace v2v
    {
      template <typename T_hsl>
      struct f_rgb_to_hsl_ : public Function_v2v< f_rgb_to_hsl_<T_hsl> >
      {
	typedef T_hsl result;

	template <typename T_rgb>
	T_hsl operator()(const T_rgb& rgb) const
	{
	  T_hsl hsl;
	  
	  double max_in = std::max(rgb.red(), std::max(rgb.blue(), rgb.green()));
	  double min_in = std::min(rgb.red(), std::min(rgb.blue(), rgb.green()));
	  double diff = max_in - min_in;
	  
	  hsl.l((max_in + min_in) / 2);
	  hsl.h();
	  hsl.s();
	  hsl.l();

	  return hsl;
	}
      };

      typedef f_rgb_to_hsl_<value::hsl_f> f_rgb_to_hsl_f_t;

      f_rgb_to_hsl_f_t f_rgb_to_hsl_f;
    
      template <typename T_rgb>
      struct f_hsl_to_rgb_ : public Function_v2v< f_hsl_to_rgb_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_hsl>
	T_rgb operator()(const T_hsl& hsl) const
	{
	  T_rgb rgb;
	  int r;
	  int g;
	  int b;

	  double sqrt3_3 = sqrt(3) / 3;
	  double inv_sqrt6 = 1 / sqrt(6);
	  double inv_sqrt2 = 1 / sqrt(2);
	  
	  double alpha = inv_sqrt2 * rgb.green() - inv_sqrt2 * rgb.blue();
	  double beta = 2 * inv_sqrt6 * rgb.red() - inv_sqrt6 * rgb.green() - inv_sqrt6 * rgb.blue();
	  
	  r = int(sqrt3_3 * hsl.i() + 2 * inv_sqrt6 * beta);
	  g = int(sqrt3_3 * hsl.i() + inv_sqrt2 * alpha - inv_sqrt6 * beta);
	  b = int(sqrt3_3 * hsl.i() - inv_sqrt2 * alpha - inv_sqrt6 * beta);
	  
	  struct value::rgb<8> rgb(r, g, b); 
	
	  return rgb;
	}
      };
    }
  }
}

#endif // OLENA_CONVERT_RGBHSL_HH
