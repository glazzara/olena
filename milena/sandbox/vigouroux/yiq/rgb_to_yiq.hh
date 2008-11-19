#include <cmath>

#include <mln/core/image_if_value.hh>

#include <mln/core/alias/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/value/int_u.hh>
#include <mln/math/round.hh>

#include "my_yiq.hh"

#ifndef MLN_RGB_TO_YIQ_HH
# define MLN_RGB_TO_YIQ_HH

namespace mln
{

  namespace fun
  {

     namespace v2v
     {
        template <typename T_yiq>
        struct f_rgb_to_yiq_ : public Function_v2v< f_rgb_to_yiq_<T_yiq> >
        {
          typedef T_yiq result;

          template <typename T_rgb>
          T_yiq operator()(const T_rgb& rgb) const
          {
             T_yiq yiq;
	
             yiq.y() = 0.1768 * rgb.red() + 0.8130 * rgb.green() + 0.0101 * rgb.blue();
             yiq.i() = 0.5346 * rgb.red() - 0.2461 * rgb.green() - 0.1791 * rgb.blue();
             yiq.q() = 0.2474 * rgb.red() - 0.6783 * rgb.green() + 0.4053 * rgb.blue();

             return yiq;
          }
        };
    
        typedef f_rgb_to_yiq_< mln::value::yiq_<double, double, double> > f_rgb_to_yiq_d;
        
        template <typename T_rgb>
        struct f_yiq_to_rgb_ : public Function_v2v< f_yiq_to_rgb_<T_rgb> >
        {
          typedef T_rgb result;

          template <typename T_yiq>
          T_rgb operator()(const T_yiq& yiq) const
          {
             int r;
             int g;
             int b;
             
             r = int(0.87 * yiq.y() + 1.3223 * yiq.i() + 0.5628 * yiq.q());
             g = int(1.026 * yiq.y() - 0.2718 * yiq.i() - 0.1458 * yiq.q());
             b = int(1.186 * yiq.y() - 1.2620 * yiq.i() + 1.8795 * yiq.q());

             struct value::rgb<8> rgb(r, g, b); 
             
             return rgb;
          }
        };

      typedef f_yiq_to_rgb_<value::rgb8> f_yiq_to_rgb_3x8_t;

      f_yiq_to_rgb_3x8_t f_yiq_to_rgb_3x8;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_RGB_TO_YIQ_HH
