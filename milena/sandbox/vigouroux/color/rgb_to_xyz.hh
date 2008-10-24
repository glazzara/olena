#ifndef OLENA_CONVERT_RGBXYZ_HH
# define OLENA_CONVERT_RGBXYZ_HH

# include <mln/core/image/image_if.hh>

# include <mln/core/alias/w_window2d_int.hh>
# include <mln/display/show.hh>
# include <mln/io/ppm/save.hh>
# include <mln/display/save_and_show.hh>
# include <mln/level/fill.hh>

# include "my_xyz.hh"

namespace mln
{

  namespace fun
  {

    namespace v2v
    {
      template <typename T_xyz>
      struct f_rgb_to_xyz_ : public Function_v2v< f_rgb_to_xyz_<T_xyz> >
      {
	typedef T_xyz result;

	template <typename T_rgb>
	T_xyz operator()(const T_rgb& rgb) const
	{
	  T_xyz xyz;

	  xyz.x(0.490 * rgb.red() + 0.310 * rgb.green() + 0.200 * rgb.blue());
	  xyz.y(0.177 * rgb.red() + 0.812 * rgb.green() + 0.011 * rgb.blue());
	  xyz.z(0.010 * rgb.green() + 0.990 * rgb.blue());

	  return xyz;
	}
      };

      typedef f_rgb_to_xyz_<value::xyz_f> f_rgb_to_xyz_f_t;

      f_rgb_to_xyz_f_t f_rgb_to_xyz_f;

      template <typename T_rgb>
      struct f_xyz_to_rgb_ : public Function_v2v< f_xyz_to_rgb_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_xyz>
	T_rgb operator()(const T_xyz& xyz) const
	{
	  int r;
	  int g;
	  int b;

	  r = int(2.365 * xyz.x() - 0.896 * xyz.y() - 0.468 * xyz.z());
	  g = int(-0.515 * xyz.x() + 1.425 * xyz.y() + 0.089 * xyz.z());
	  b = int(0.005 * xyz.x() - 0.014 * xyz.y() + 1.01 * xyz.z());

	  struct value::rgb<8> rgb(r, g, b);

	  return rgb;
	}
      };

      typedef f_xyz_to_rgb_<value::rgb8> f_xyz_to_rgb_3x8_t;

      f_xyz_to_rgb_3x8_t f_xyz_to_rgb_3x8;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // OLENA_CONVERT_RGBXYZ_HH
