#include <cmath>

#include <mln/core/image_if_value.hh>

#include <mln/core/alias/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/math/round.hh>

#include "../color/my_hsi.hh"

#ifndef MLN_HSI_BLUE_HH
# define MLN_HSI_BLUE_HH

namespace mln
{


  namespace fun
  {

    namespace v2v
    {
      template <typename T_rgb>
      struct f_blue_hsi_ : public Function_v2v< f_blue_hsi_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_hsi>
	T_rgb operator()(const T_hsi& hsi) const
	{
	  typedef typename T_rgb::blue_t   blue_t;

	  static math::round<blue_t>   to_b;

	  static const float
	    sqrt3_3 = std::sqrt(3) / 3,
	    inv_sqrt6 = 1 / std::sqrt(6),
	    inv_sqrt2 = 1 / std::sqrt(2);

	  float
	    h = hsi.hue() / 180.0 * 3.1415,
	    alpha = hsi.sat() * std::cos(h),
	    beta = hsi.sat() * std::sin(h);


	  blue_t   b = to_b(sqrt3_3 * hsi.inty() - inv_sqrt2 * alpha - inv_sqrt6 * beta);

	  T_rgb rgb(0, 0, b);

	  return rgb;
	}
      };

      typedef f_blue_hsi_<value::rgb8> f_hsi_blue_t;

      f_hsi_blue_t f_hsi_blue;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_HSI_BLUE_HH
