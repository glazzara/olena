#include <cmath>

#include <mln/core/image_if_value.hh>
#include <mln/core/image/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/value/int_u.hh>
#include <mln/math/round.hh>

#include "../cmy/my_cmy.hh"
#include "../hsi/my_hsi.hh"
#include "../xyz/my_xyz.hh"
#include "../yiq/my_yiq.hh"
#include "../yuv/my_yuv.hh"
#include "torgb.hh"
#include "rgbto.hh"
#include "tored.hh"

#ifndef MLN_RED_HH
# define MLN_RED_HH

// namespace later
// {
//   template <typename T>
//   struct red;
// }

namespace mln
{

  namespace fun
  {
     
     namespace v2v
     {
        template <typename T>
        struct f_red_ : public Function_v2v< f_red_<T> >
        {
          typedef typename later::red<T>::result result;

          template <typename U>
          result operator()(const U& color) const
          {
	    value::rgb<8> rgb;

	    rgb = f_to_red_< value::rgb<8> >(color);
	    
	    return later::red<T>::on(rgb);
	  }
	};
     }
  }
}

// namespace later
// {
//   template <typename T>
//   struct red< mln::value::cmy_<T, T, T> >
//   {
//     typedef mln::value::cmy_<T, T, T> result;
//     static result on(const mln::value::rgb<8>& rgb)
//     {
//       result cmy;

//       cmy.cyan() = 1 - rgb.red();
//       cmy.magenta() = 1 - rgb.green();
//       cmy.yellow() = 1 - rgb.blue();
      
//       return cmy;
//     }
//   };

#endif // ! MLN_RGB_TO_CMY_HH
