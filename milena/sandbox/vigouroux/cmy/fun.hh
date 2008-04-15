
#include <cmath>

#include <mln/core/image_if_value.hh>
#include <mln/core/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/math/round.hh>

#include "my_cmy.hh"



namespace mln
{


  namespace fun
  {

    namespace v2v
    {
      template <typename T_rgb>
      struct f_cmy_ : public Function_v2v< f_cmy_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_cmy>
	T_rgb operator()(const T_cmy& cmy) const
	{
           int red;
           
           red = int(1 - cmy.cyan());
           
           T_rgb rgb(red, 0, 0);
           
           return rgb;
	}
      };

      typedef f_cmy_<value::rgb8> f_cmy_3x8_t;

      f_cmy_3x8_t f_cmy_3x8;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln
