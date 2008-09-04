#include <cmath>

#include <mln/core/image_if_value.hh>
#include <mln/core/image/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/math/round.hh>

#include "../color/my_xyz.hh"

#ifndef MLN_XYZ_GREEN_HH
# define MLN_XYZ_GREEN_HH

namespace mln
{


  namespace fun
  {

    namespace v2v
    {
      template <typename T_rgb>
      struct f_green_xyz_ : public Function_v2v< f_green_xyz_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_xyz>
	T_rgb operator()(const T_xyz& xyz) const
	{
	  typedef typename T_rgb::green_t   green_t;

	  static math::round<green_t>   to_g;

	  green_t   g = to_g(-0.515 * xyz.x() + 1.425 * xyz.y() + 0.089 * xyz.z());

	  T_rgb rgb(0, g, 0);

	  return rgb;
	}
      };

      typedef f_green_xyz_<value::rgb8> f_xyz_green_t;

      f_xyz_green_t f_xyz_green;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_XYZ_GREEN_HH
