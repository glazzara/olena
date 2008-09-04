#include <cmath>

#include <mln/core/image_if_value.hh>
#include <mln/core/image/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/math/round.hh>

#include "../color/my_hsi.hh"

#ifndef MLN_HSI_FUN_HH
# define MLN_HSI_FUN_HH

namespace mln
{


  namespace fun
  {

    namespace v2v
    {
      template <typename T_rgb>
      struct f_hsi_ : public Function_v2v< f_hsi_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_hsi>
	T_rgb operator()(const T_hsi& hsi) const
	{
	  typedef typename T_rgb::red_t   red_t;

	  static math::round<red_t>   to_r;

	  static const float
	    sqrt3_3 = std::sqrt(3) / 3,
	    inv_sqrt6 = 1 / std::sqrt(6),
	    inv_sqrt2 = 1 / std::sqrt(2);

	  float
	    h = hsi.hue() / 180.0 * 3.1415,
	    alpha = hsi.sat() * std::cos(h),
	    beta = hsi.sat() * std::sin(h);


	  red_t   r = to_r(sqrt3_3 * hsi.inty() + 2 * inv_sqrt6 * beta);

	  T_rgb rgb(r, 0, 0);

	  return rgb;
	}
      };

      typedef f_hsi_<value::rgb8> f_hsi_3x8_t;

      f_hsi_3x8_t f_hsi_3x8;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_HSI_FUN_HH
