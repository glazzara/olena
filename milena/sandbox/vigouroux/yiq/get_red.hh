#include <cmath>

#include <mln/core/image_if_value.hh>
#include <mln/core/image/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/math/round.hh>

#include "my_yiq.hh"

#ifndef MLN_YIQ_FUN_HH
# define MLN_YIQ_FUN_HH

namespace mln
{


  namespace fun
  {

    namespace v2v
    {
      template <typename T_rgb>
      struct f_yiq_ : public Function_v2v< f_yiq_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_yiq>
	T_rgb operator()(const T_yiq& yiq) const
	{
           int r;
              
           r = int(0.87 * yiq.y() + 1.3223 * yiq.i() + 0.5628 * yiq.q());
   
           T_rgb rgb(r, 0, 0); 
           
           return rgb;
	}
      };

      typedef f_yiq_<value::rgb8> f_yiq_3x8_t;

      f_yiq_3x8_t f_yiq_3x8;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_YIQ_FUN_HH
