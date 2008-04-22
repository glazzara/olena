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

#ifndef MLN_YIQ_BLUE_HH
# define MLN_YIQ_BLUE_HH

namespace mln
{


  namespace fun
  {

    namespace v2v
    {
      template <typename T_rgb>
      struct f_blue_yiq_ : public Function_v2v< f_blue_yiq_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_yiq>
	T_rgb operator()(const T_yiq& yiq) const
	{
	  typedef typename T_rgb::blue_t   blue_t;

	  static math::round<blue_t>   to_b;

	  blue_t   b = to_b(1.186 * yiq.y() - 1.2620 * yiq.i() + 1.8795 * yiq.q());

	  T_rgb rgb(0, 0, b);

	  return rgb;
	}
      };

      typedef f_blue_yiq_<value::rgb8> f_yiq_blue_t;

      f_yiq_blue_t f_yiq_blue;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_YIQ_BLUE_HH
