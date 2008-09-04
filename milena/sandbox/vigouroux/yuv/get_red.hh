#include <cmath>

#include <mln/core/image_if_value.hh>
#include <mln/core/image/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/math/round.hh>

#include "my_yuv.hh"

#ifndef MLN_YUV_FUN_HH
# define MLN_YUV_FUN_HH

namespace mln
{


  namespace fun
  {

    namespace v2v
    {
      template <typename T_rgb>
      struct f_yuv_ : public Function_v2v< f_yuv_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_yuv>
	T_rgb operator()(const T_yuv& yuv) const
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

      typedef f_yuv_<value::rgb8> f_yuv_3x8_t;

      f_yuv_3x8_t f_yuv_3x8;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_VALUE_YUV_HH
