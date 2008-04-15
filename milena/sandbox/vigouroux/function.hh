
#include <mln/core/image_if_value.hh>
#include <mln/core/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/math/round.hh>
#include <mln/metal/is_a.hh>

#include "color/my_hsi.hh"
#include <mln/value/rgb.hh>

#include "hsi/fun.hh"
#include "cmy/fun.hh"
#include "yuv/fun.hh"
#include "yiq/fun.hh"

namespace mln
{


  namespace fun
  {

    namespace v2v
    {

      template <typename T_red>
      struct f_get_red_ : public Function_v2v< f_get_red_<T_red> >
      {
            typedef T_red result;

	template <typename T_col>
	T_red operator()(const T_col& col) const
	{
          T_red r;

	  if (mlc_is_a(T_col, mln::value::HSI)::value == 1)
          {
             r = mln::fun::v2v::f_hsi_3x8(col);
             std::cout << "hsi" << std::endl;
          }
// 	  else if (mlc_is_a(T_col, mln::value::CMY)::value == 1)
//           {
//              r = mln::fun::v2v::f_cmy_3x8(col);
//              std::cout << "cmy" << std::endl;
//           }
// 	  else if (mlc_is_a(T_col, mln::value::YUV)::value == 1)
//           {
//              r = mln::fun::v2v::f_yuv_3x8(col);
//              std::cout << "yuv" << std::endl;
//           }
// 	  else if (mlc_is_a(T_col, mln::value::YIQ)::value == 1)
//           {
//              r = mln::fun::v2v::f_yiq_3x8(col);
//              std::cout << "yiq" << std::endl;
//           }
          else
          {
//              r = mln::fun::v2v::f_cmy_3x8(col);
             std::cout << "else" << std::endl;
          }
	  return r;
	}
      };

//       typedef f_rgb_to_hsi_<value::hsi_f> f_r// gb_to_hsi_f_t;

//       f_rgb_to_hsi_f_t f_rgb_to_hsi_f;


//       template <typename T_rgb>
//       struct f_hsi_to_rgb_ : public Function_v2v< f_hsi_to_rgb_<T_rgb> >
//       {
// 	typedef T_rgb result;

// 	template <typename T_hsi>
// 	T_rgb operator()(const T_hsi& hsi) const
// 	{
// 	  typedef typename T_rgb::red_t   red_t;
// 	  typedef typename T_rgb::green_t green_t;
// 	  typedef typename T_rgb::blue_t  blue_t;

// 	  static math::round<red_t>   to_r;
// 	  static math::round<green_t> to_g;
// 	  static math::round<blue_t>  to_b;

// 	  static const float
// 	    sqrt3_3 = std::sqrt(3) / 3,
// 	    inv_sqrt6 = 1 / std::sqrt(6),
// 	    inv_sqrt2 = 1 / std::sqrt(2);

// 	  float
// 	    h = hsi.hue() / 180.0 * 3.1415,
// 	    alpha = hsi.sat() * std::cos(h),
// 	    beta = hsi.sat() * std::sin(h);


// 	  red_t   r = sqrt3_3 * hsi.inty() + 2 * inv_sqrt6 * beta;
// 	  green_t g = sqrt3_3 * hsi.inty() + inv_sqrt2 * alpha - inv_sqrt6 * beta;
// 	  blue_t  b = sqrt3_3 * hsi.inty() - inv_sqrt2 * alpha - inv_sqrt6 * beta;

// 	  T_rgb rgb(r, g, b);

// 	  return rgb;
// 	}
//       };

//       typedef f_hsi_to_rgb_<value::rgb8> f_hsi_to_rgb_3x8_t;

//       f_hsi_to_rgb_3x8_t f_hsi_to_rgb_3x8;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln
