#include <cmath>

#include <mln/core/image_if_value.hh>
#include <mln/core/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/math/round.hh>

#include "../color/my_yiq.hh"

#ifndef MLN_YIQ_GREEN_HH
# define MLN_YIQ_GREEN_HH

namespace mln
{

  namespace fun
  {

    namespace v2v
    {
      template <typename T_rgb>
      struct f_green_yiq_ : public Function_v2v< f_green_yiq_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_yiq>
	T_rgb operator()(const T_yiq& yiq) const
	{
	  typedef typename T_rgb::green_t   green_t;

	  static math::round<green_t>   to_g;

	  green_t   g = to_g(1.026 * yiq.y() - 0.2718 * yiq.i() - 0.1458 * yiq.q());

	  T_rgb rgb(0, g, 0);

	  return rgb;
	}
      };

      typedef f_green_yiq_<value::rgb8> f_yiq_green_t;

      f_yiq_green_t f_yiq_green;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_YIQ_GREEN_HH
