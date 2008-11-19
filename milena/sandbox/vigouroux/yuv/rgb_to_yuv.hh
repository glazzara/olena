#include <cmath>

#include <mln/core/image_if_value.hh>

#include <mln/core/alias/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/value/int_u.hh>
#include <mln/math/round.hh>

#include "my_yuv.hh"

#ifndef MLN_RGB_TO_YUV_HH
# define MLN_RGB_TO_YUV_HH

namespace mln 
{

  namespace fun
  {

     namespace v2v
     {
        template <typename T_yuv>
        struct f_rgb_to_yuv_ : public Function_v2v< f_rgb_to_yuv_<T_yuv> >
        {
          typedef T_yuv result;
          
          template <typename T_rgb>
          T_yuv operator()(const T_rgb& rgb) const
          {
             T_yuv yuv;
	
             yuv.y() = 0.299 * rgb.red() + 0.587 * rgb.green() + 0.114 * rgb.blue();
             yuv.u() = 0.436 * (rgb.blue() - yuv.y()) / (1 - 0.114);
             yuv.v() = 0.615 * (rgb.red() - yuv.y()) / (1 - 0.299);

             return yuv;
          }
        };

        typedef f_rgb_to_yuv_< mln::value::yuv_<double, double, double> > f_rgb_to_yuv_d;
        typedef f_rgb_to_yuv_< mln::value::yuv_<float, float, float> > f_rgb_to_yuv_f;
        
        template <typename T_rgb>
        struct f_yuv_to_rgb_ : public Function_v2v< f_yuv_to_rgb_<T_rgb> >
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

      typedef f_yuv_to_rgb_<value::rgb8> f_yuv_to_rgb_3x8_t;

      f_yuv_to_rgb_3x8_t f_yuv_to_rgb_3x8;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_RGB_TO_YUV_HH
