#include <cmath>

#include <mln/core/image_if_value.hh>

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

#ifndef MLN_RGB_TO_CMY_HH
# define MLN_RGB_TO_CMY_HH

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
        template <typename T>
        struct f_rgb_to_ : public Function_v2v< f_rgb_to_<T> >
        {
          typedef typename later::red<T>::result result;

          template <typename T_rgb>
          result operator()(const T_rgb& rgb) const
          {
	    return later::red<T>::on(rgb);
	  }
	};
     }
  }
}

namespace later
{
  template <typename T>
  struct red< mln::value::cmy_<T, T, T> >
  {
    typedef mln::value::cmy_<T, T, T> result;
    static result on(const mln::value::rgb<8>& rgb)
    {
      result cmy;

      cmy.cyan() = 1 - rgb.red();
      cmy.magenta() = 1 - rgb.green();
      cmy.yellow() = 1 - rgb.blue();
      
      return cmy;
    }
  };

  template <typename T>
  struct red< mln::value::hsi_<T, T, T> >
  {
    typedef mln::value::hsi_<T, T, T> result;
    static result on(const mln::value::rgb<8>& rgb)
    {
      static const double sqrt3_3 = std::sqrt(3) / 3;
      static const double inv_sqrt6 = 1 / std::sqrt(6);
      static const double inv_sqrt2 = 1 / std::sqrt(2);
      
      result hsi;
      
      double alpha = inv_sqrt2 * rgb.green() - inv_sqrt2 * rgb.blue();
      double beta = 2 * inv_sqrt6 * rgb.red() - inv_sqrt6 * rgb.green() - inv_sqrt6 * rgb.blue();
      

      float tmp = atan2(beta, alpha) / 3.1415 * 180.0;
      
      hsi.hue() = atan2(beta, alpha) / 3.1415 * 180.0;
      if (hsi.hue() < 0)
	hsi.hue() = hsi.hue() + 360.0;
      mln_invariant(hsi.hue() >= 0);
      hsi.sat() = std::sqrt(alpha * alpha + beta * beta);
      hsi.inty() = sqrt3_3 * rgb.red() + sqrt3_3 * rgb.green() + sqrt3_3 * rgb.blue();
      
      return hsi;
    }
  };
  
  template <typename T>
  struct red< mln::value::xyz_<T, T, T> >
  {
    typedef mln::value::xyz_<T, T, T> result;
    static result on(const mln::value::rgb<8>& rgb)
    {
      result xyz;
      
      xyz.x() = 0.490 * rgb.red() + 0.310 * rgb.green() + 0.200 * rgb.blue();
      xyz.y() = 0.177 * rgb.red() + 0.812 * rgb.green() + 0.011 * rgb.blue();
      xyz.z() = 0.010 * rgb.green() + 0.990 * rgb.blue();
      
      return xyz;
    }
  };
  
  template <typename T>
  struct red< mln::value::yiq_<T, T, T> >
  {
    typedef mln::value::yiq_<T, T, T> result;
    static result on(const mln::value::rgb<8>& rgb)
    {
      result yiq;
      
      yiq.y() = 0.1768 * rgb.red() + 0.8130 * rgb.green() + 0.0101 * rgb.blue();
      yiq.i() = 0.5346 * rgb.red() - 0.2461 * rgb.green() - 0.1791 * rgb.blue();
      yiq.q() = 0.2474 * rgb.red() - 0.6783 * rgb.green() + 0.4053 * rgb.blue();
      
      return yiq;
    }
  };

  template <typename T>
  struct red< mln::value::yuv_<T, T, T> >
  {
    typedef mln::value::yuv_<T, T, T> result;
    static result on(const mln::value::rgb<8>& rgb)
    {
      result yuv;
      
      yuv.y() = 0.299 * rgb.red() + 0.587 * rgb.green() + 0.114 * rgb.blue();
      yuv.u() = 0.436 * (rgb.blue() - yuv.y()) / (1 - 0.114);
      yuv.v() = 0.615 * (rgb.red() - yuv.y()) / (1 - 0.299);

      return yuv;
    }
  };

}


#endif // ! MLN_RGB_TO_CMY_HH
