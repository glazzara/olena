#include <cmath>

#include <mln/core/image_if_value.hh>
#include <mln/core/image/inplace.hh>
#include <mln/core/alias/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/math/round.hh>

#include "../color/my_yuv.hh"

#ifndef MLN_YUV_BLUE_HH
# define MLN_YUV_BLUE_HH

namespace mln
{


  namespace fun
  {

    namespace v2v
    {
      template <typename T_rgb>
      struct f_blue_yuv_ : public Function_v2v< f_blue_yuv_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_yuv>
	T_rgb operator()(const T_yuv& yuv) const
	{
	  typedef typename T_rgb::blue_t   blue_t;

	  static math::round<blue_t>   to_b;

	  blue_t   b = to_b(yuv.y() + 2.03211 * yuv.u());

	  T_rgb rgb(0, 0, b);

	  return rgb;
	}
      };

      typedef f_blue_yuv_<value::rgb8> f_yuv_blue_t;

      f_yuv_blue_t f_yuv_blue;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_YUV_BLUE_HH
