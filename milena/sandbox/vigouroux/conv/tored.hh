#include <cmath>

#include <mln/core/image_if_value.hh>
#include <mln/core/image/inplace.hh>
#include <mln/core/alias/w_window2d_int.hh>
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

#ifndef MLN_TO_RED_HH
# define MLN_TO_RED_HH

namespace later
{
  template <typename T>
  struct red;
}

namespace mln
{

  namespace fun
  {
     
     namespace v2v
     {
        template <typename T_rgb>
        struct f_to_red_ : public Function_v2v< f_to_red_<T_rgb> >
        {
	  typedef T_rgb result;

	  template <typename T>
          T_rgb operator()(const value::cmy_<T, T, T>& color) const
          {
	    int red;
             
	    red = int(1 - color.cyan());
	    T_rgb rgb(red, 0, 0);
	
	    return rgb;
	  }

	  template <typename T>
          T_rgb operator()(const value::hsi_<T, T, T>& color) const
          {
	    typedef typename T_rgb::red_t   red_t;
	    
	    static math::round<red_t>   to_r;
	    
	    static const float
	      sqrt3_3 = std::sqrt(3) / 3,
	      inv_sqrt6 = 1 / std::sqrt(6),
	      inv_sqrt2 = 1 / std::sqrt(2);

	    float
	      h = color.hue() / 180.0 * 3.1415,
	      alpha = color.sat() * std::cos(h),
	      beta = color.sat() * std::sin(h);
	    
	    red_t   r = sqrt3_3 * color.inty() + 2 * inv_sqrt6 * beta;
	    
	    T_rgb rgb(r, 0, 0);

	    return rgb;
	  }

	  template <typename T>
          T_rgb operator()(const value::xyz_<T, T, T>& color) const
          {
	    int r;
	  
	    r = int(2.365 * color.x() - 0.896 * color.y() - 0.468 * color.z());
	  
	    struct value::rgb<8> rgb(r, 0, 0); 
	    
	    return rgb;
	  }

	  template <typename T>
          T_rgb operator()(const value::yuv_<T, T, T>& color) const
          {
	    int r;
             
	    r = int(color.y() + 1.13983 * color.v());
	    
	    struct value::rgb<8> rgb(r, 0, 0); 
            
	    return (rgb);
	  }

	  template <typename T>
          T_rgb operator()(const value::yiq_<T, T, T>& color) const
          {
	    int r;
            
	    r = int(0.87 * color.y() + 1.3223 * color.i() + 0.5628 * color.q());

	    struct value::rgb<8> rgb(r, 0, 0); 
            
	    return rgb;
	  }


	};
     }
  }
}

#endif // ! MLN_TO_RED_HH
