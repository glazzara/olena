#include <cmath>

#include <mln/core/image_if_value.hh>

#include <mln/core/alias/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/math/round.hh>

#include "../color/my_xyz.hh"

#ifndef MLN_XYZ_BLUE_HH
# define MLN_XYZ_BLUE_HH

namespace mln
{


  namespace fun
  {

    namespace v2v
    {
      template <typename T_rgb>
      struct f_blue_xyz_ : public Function_v2v< f_blue_xyz_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_xyz>
	T_rgb operator()(const T_xyz& xyz) const
	{
	  typedef typename T_rgb::blue_t   blue_t;

	  static math::round<blue_t>   to_b;

	  blue_t   b = to_b(0.005 * xyz.x() - 0.014 * xyz.y() + 1.01 * xyz.z());

	  T_rgb rgb(0, 0, b);

	  return rgb;
	}
      };

      typedef f_blue_xyz_<value::rgb8> f_xyz_blue_t;

      f_xyz_blue_t f_xyz_blue;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_XYZ_BLUE_HH
