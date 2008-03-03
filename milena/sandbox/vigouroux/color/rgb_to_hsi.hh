
#include <cmath>

#include <mln/core/image_if_value.hh>
#include <mln/core/inplace.hh>
#include <mln/core/w_window2d_int.hh>
#include <mln/display/show.hh>
#include <mln/io/ppm/save.hh>
#include <mln/display/save_and_show.hh>
#include <mln/level/fill.hh>

#include "my_hsi.hh"



namespace mln
{


  namespace fun
  {

    namespace v2v
    {

      // NEW

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
	  if (tmp < 0 or tmp > 1)
	    {
	      std::cout << "FIXME: " << tmp << std::endl;
	    }

	  hsi.hue() = atan2(beta, alpha) / 3.1415 * 180.0;
	  if (hsi.hue() < 0)
	    hsi.hue() = hsi.hue() + 360.0;
	  mln_invariant(hsi.hue() >= 0);
	  hsi.sat() = std::sqrt(alpha * alpha + beta * beta);
	  hsi.i() = sqrt3_3 * rgb.red() + sqrt3_3 * rgb.green() + sqrt3_3 * rgb.blue();

	  return hsi;
	}
      };

      typedef f_rgb_to_hsi_<value::hsi_3x8> f_rgb_to_hsi_3x8_t;

      f_rgb_to_hsi_3x8_t f_rgb_to_hsi_3x8;

      // end of NEW

            // NEW2

      template <typename T_rgb>
      struct f_hsi_to_rgb_ : public Function_v2v< f_hsi_to_rgb_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_hsi>
	T_rgb operator()(const T_hsi& hsi) const
	{
	  int r;
	  int g;
	  int b;
	  
	  static const double sqrt3_3 = sqrt(3) / 3;
	  static const double inv_sqrt6 = 1 / sqrt(6);
	  static const double inv_sqrt2 = 1 / sqrt(2);

	  double h = hsi.hue() / 180.0 * 3.1415;
	  double alpha = hsi.sat() * cos(h);
	  double beta = hsi.sat() * sin(h);
	  
	  r = int(sqrt3_3 * hsi.i() + 2 * inv_sqrt6 * beta);
	  g = int(sqrt3_3 * hsi.i() + inv_sqrt2 * alpha - inv_sqrt6 * beta);
	  b = int(sqrt3_3 * hsi.i() - inv_sqrt2 * alpha - inv_sqrt6 * beta);
	  
	  T_rgb rgb(r, g, b); 
	  
	  return rgb;
	}
      };

      typedef f_hsi_to_rgb_<value::rgb8> f_hsi_to_rgb_3x8_t;

      f_hsi_to_rgb_3x8_t f_hsi_to_rgb_3x8;

      // end of NEW2


    } // end of namespace fun::v2v

  } // end of namespace fun




  namespace convert
  {

    struct f_rgb_to_hsi
    {
      value::hsi<8> operator()(const value::rgb<8>& rgb) const
      {
	struct value::hsi<8> hsi;
	double sqrt3_3 = sqrt(3) / 3;
	double inv_sqrt6 = 1 / sqrt(6);
	double inv_sqrt2 = 1 / sqrt(2);

	double alpha = inv_sqrt2 * rgb.green() - inv_sqrt2 * rgb.blue();
	double beta = 2 * inv_sqrt6 * rgb.red() - inv_sqrt6 * rgb.green() - inv_sqrt6 * rgb.blue();

	hsi.hue(atan2(beta, alpha) / 3.1415 * 180.0);
	if (hsi.hue() < 0)
	  hsi.hue(hsi.hue() + 360.0);
	mln_precondition(hsi.hue() >= 0);
	hsi.sat(sqrt(alpha * alpha + beta * beta));
	hsi.i(sqrt3_3 * rgb.red() + sqrt3_3 * rgb.green() + sqrt3_3 * rgb.blue());

	return hsi;
      }
    };

    
    struct f_hsi_to_rgb
    {

      value::rgb<8> doit(const value::hsi<8>& hsi) const
      {
	int r;
	int g;
	int b;

	double sqrt3_3 = sqrt(3) / 3;
	double inv_sqrt6 = 1 / sqrt(6);
	double inv_sqrt2 = 1 / sqrt(2);

	double h = hsi.hue() / 180.0 * 3.1415;
	double alpha = hsi.sat() * cos(h);
	double beta = hsi.sat() * sin(h);

	r = int(sqrt3_3 * hsi.i() + 2 * inv_sqrt6 * beta);
	g = int(sqrt3_3 * hsi.i() + inv_sqrt2 * alpha - inv_sqrt6 * beta);
	b = int(sqrt3_3 * hsi.i() - inv_sqrt2 * alpha - inv_sqrt6 * beta);

	value::rgb<8> rgb(r, g, b); 
	
	return rgb;
      }
    };

  } // end of FIXME

} // end of FIXME
