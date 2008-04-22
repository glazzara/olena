#include <cmath>

#include <mln/core/image_if_value.hh>
#include <mln/core/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/math/round.hh>

#include "../color/my_cmy.hh"

#ifndef MLN_CMY_BLUE_HH
# define MLN_CMY_BLUE_HH

namespace mln
{


  namespace fun
  {

    namespace v2v
    {
      template <typename T_rgb>
      struct f_blue_cmy_ : public Function_v2v< f_blue_cmy_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_cmy>
	T_rgb operator()(const T_cmy& cmy) const
	{
	  typedef typename T_rgb::blue_t   blue_t;

	  static math::round<blue_t>   to_b;

	  static const float
	    sqrt3_3 = std::sqrt(3) / 3,
	    inv_sqrt6 = 1 / std::sqrt(6),
	    inv_sqrt2 = 1 / std::sqrt(2);

	  float
	    h = cmy.hue() / 180.0 * 3.1415,
	    alpha = cmy.sat() * std::cos(h),
	    beta = cmy.sat() * std::sin(h);


	  blue_t   b = to_b(1 - cmy.yellow());

	  T_rgb rgb(0, 0, b);

	  return rgb;
	}
      };

      typedef f_blue_cmy_<value::rgb8> f_cmy_blue_t;

      f_cmy_blue_t f_cmy_blue;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_CMY_BLUE_HH
