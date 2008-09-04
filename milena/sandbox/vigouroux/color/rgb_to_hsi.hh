
#include <cmath>

#include <mln/core/image_if_value.hh>
#include <mln/core/image/inplace.hh>
#include <mln/core/alias/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>
#include <mln/math/round.hh>

#include "my_hsi.hh"

namespace mln
{

  namespace fun
  {

    namespace v2v
    {
      template <typename T_hsi>
      struct f_rgb_to_hsi_ : public Function_v2v< f_rgb_to_hsi_<T_hsi> >
      {
	typedef T_hsi result;

	template <typename T_rgb>
	T_hsi operator()(const T_rgb& rgb) const
	{
	  // Locals.
	  static const double sqrt3_3 = std::sqrt(3) / 3;
	  static const double inv_sqrt6 = 1 / std::sqrt(6);
	  static const double inv_sqrt2 = 1 / std::sqrt(2);

	  T_hsi hsi;

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

      typedef f_rgb_to_hsi_<value::hsi_f> f_rgb_to_hsi_f_t;

      f_rgb_to_hsi_f_t f_rgb_to_hsi_f;


      template <typename T_rgb>
      struct f_hsi_to_rgb_ : public Function_v2v< f_hsi_to_rgb_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_hsi>
	T_rgb operator()(const T_hsi& hsi) const
	{
	  typedef typename T_rgb::red_t   red_t;
	  typedef typename T_rgb::green_t green_t;
	  typedef typename T_rgb::blue_t  blue_t;

	  static math::round<red_t>   to_r;
	  static math::round<green_t> to_g;
	  static math::round<blue_t>  to_b;

	  static const float
	    sqrt3_3 = std::sqrt(3) / 3,
	    inv_sqrt6 = 1 / std::sqrt(6),
	    inv_sqrt2 = 1 / std::sqrt(2);

	  float
	    h = hsi.hue() / 180.0 * 3.1415,
	    alpha = hsi.sat() * std::cos(h),
	    beta = hsi.sat() * std::sin(h);


	  red_t   r = sqrt3_3 * hsi.inty() + 2 * inv_sqrt6 * beta;
	  green_t g = sqrt3_3 * hsi.inty() + inv_sqrt2 * alpha - inv_sqrt6 * beta;
	  blue_t  b = sqrt3_3 * hsi.inty() - inv_sqrt2 * alpha - inv_sqrt6 * beta;

	  T_rgb rgb(r, g, b);

	  return rgb;
	}
      };

      typedef f_hsi_to_rgb_<value::rgb8> f_hsi_to_rgb_3x8_t;

      f_hsi_to_rgb_3x8_t f_hsi_to_rgb_3x8;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln
