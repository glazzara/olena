#include <cmath>

#include <mln/core/image_if_value.hh>

#include <mln/core/alias/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/data/fill.hh>
#include <mln/value/int_u.hh>
#include <mln/math/round.hh>

#include "my_cmy.hh"

#ifndef MLN_RGB_TO_CMY_HH
# define MLN_RGB_TO_CMY_HH

namespace mln
{

  namespace fun
  {
     
     namespace v2v
     {
        template <typename T_cmy>
        struct f_rgb_to_cmy_ : public Function_v2v< f_rgb_to_cmy_<T_cmy> >
        {
          typedef T_cmy result;

          template <typename T_rgb>
          T_cmy operator()(const T_rgb& rgb) const
          {
             T_cmy cmy;
	
             cmy.cyan() = 1 - rgb.red();
             cmy.magenta() = 1 - rgb.green();
             cmy.yellow() = 1 - rgb.blue();

             return cmy;
          }
        };
        
        typedef f_rgb_to_cmy_< mln::value::cmy_<double, double, double> > f_rgb_to_cmy_d;
        typedef f_rgb_to_cmy_< mln::value::cmy_<float, float, float> > f_rgb_to_cmy_f;
        
        template <typename T_rgb>
        struct f_cmy_to_rgb_ : public Function_v2v< f_cmy_to_rgb_<T_rgb> >
        {
          typedef T_rgb result;

          template <typename T_cmy>
          T_rgb operator()(const T_cmy& cmy) const
          {
             int red;
             int green;
             int blue;
             
             red = int(1 - cmy.cyan());
             green = int(1 - cmy.magenta());
             blue = int(1 - cmy.yellow());
             T_rgb rgb(red, green, blue);
	
             return rgb;
          }
        };
    
      typedef f_cmy_to_rgb_<value::cmy_f> f_cmy_to_rgb_3x8_t;

      f_cmy_to_rgb_3x8_t f_cmy_to_rgb_3x8;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_RGB_TO_CMY_HH
