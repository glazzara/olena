// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_FUN_V2V_HSL_TO_RGB_HH
# define MLN_FUN_V2V_HSL_TO_RGB_HH

/// \file
///
/// \brief Convert HSL values to RGB.
///
/// \todo Etienne: Remove the global variables!

# include <cmath>

# include <mln/math/round.hh>
# include <mln/math/max.hh>
# include <mln/math/min.hh>

# include <mln/trait/value_.hh>

# include <mln/value/rgb.hh>



namespace mln
{

  // Forward declarations
  namespace value
  {
    template <typename H, typename S, typename L> class hsl_;
    typedef hsl_<float, float, float> hsl_f;
    template <unsigned n> struct rgb;
  }

  namespace fun
  {

    namespace v2v
    {

      /// \brief Convert hsl values to rgb.
      ///
      /// \ingroup modfunv2v
      //
      template <typename T_rgb>
      struct f_hsl_to_rgb_ : public Function_v2v< f_hsl_to_rgb_<T_rgb> >
      {
	typedef T_rgb result;


	f_hsl_to_rgb_();

	template <typename T_hsl>
        T_rgb operator()(const T_hsl& hsl) const;

      };

      typedef f_hsl_to_rgb_< value::rgb<8> > f_hsl_to_rgb_3x8_t;
      typedef f_hsl_to_rgb_< value::rgb<16> > f_hsl_to_rgb_3x16_t;

      extern f_hsl_to_rgb_3x8_t f_hsl_to_rgb_3x8;
      extern f_hsl_to_rgb_3x16_t f_hsl_to_rgb_3x16;


# ifndef MLN_INCLUDE_ONLY

#  ifndef MLN_WO_GLOBAL_VARS

      /// Global variables.
      /// \{
      f_hsl_to_rgb_3x8_t f_hsl_to_rgb_3x8;
      f_hsl_to_rgb_3x16_t f_hsl_to_rgb_3x16;
      /// \}
#  endif // !MLN_WO_GLOBAL_VARS

      template <typename T_rgb>
      f_hsl_to_rgb_<T_rgb>::f_hsl_to_rgb_()
      {
      }

      /// This method implements the conversion from HSL to RGB as described by
      /// Max K. Agoston in `Computer Graphics and Geometric Modeling:
      /// Implementation and Algorithms (2005)'.
      template <typename T_rgb>
      template <typename T_hsl>
      inline
      T_rgb
      f_hsl_to_rgb_<T_rgb>::operator()(const T_hsl& hsl) const
      {
	typedef typename T_rgb::red_t   red_t;
	typedef typename T_rgb::green_t green_t;
	typedef typename T_rgb::blue_t  blue_t;

	static math::round<red_t>   to_r;
	static math::round<green_t> to_g;
	static math::round<blue_t>  to_b;

	const float q = (hsl.lum() < 0.5) ? hsl.lum() * (1.0 + hsl.sat()) :
					    hsl.lum() + hsl.sat() - (hsl.lum() * hsl.sat());
	const float p = 2.0 * hsl.lum() - q;
	const float hk = hsl.hue() / 360.0; // hk = normalized hue
	float tr = hk + (1.0 / 3.0);
	float tg = hk;
	float tb = hk - (1.0 / 3.0);

	if (tr < 0.0)
	  tr += 1.0;
	if (tr > 1.0)
	  tr -= 1.0;

	if (tg < 0.0)
	  tg += 1.0;
	if (tg > 1.0)
	  tg -= 1.0;

	if (tb < 0.0)
	  tb += 1.0;
	if (tb > 1.0)
	  tb -= 1.0;

	// Red.
	float red;
	if (tr < (1.0 / 6.0))
	  red = p + ((q - p) * 6 * tr);
	else if (tr < (1.0 / 2.0))
	  red = q;
	else if (tr < (2.0 / 3.0))
	  red = p + ((q - p) * 6 * ((2.0 / 3.0) - tr));
	else
	  red = p;

	// Green.
	float green;
	if (tg < (1.0 / 6.0))
	  green = p + ((q - p) * 6 * tg);
	else if (tg < (1.0 / 2.0))
	  green = q;
	else if (tg < (2.0 / 3.0))
	  green = p + ((q - p) * 6 * ((2.0 / 3.0) - tg));
	else
	  green = p;

	// Blue.
	float blue;
	if (tb < (1.0 / 6.0))
	  blue = p + ((q - p) * 6 * tb);
	else if (tb < (1.0 / 2.0))
	  blue = q;
	else if (tb < (2.0 / 3.0))
	  blue = p + ((q - p) * 6 * ((2.0 / 3.0) - tb));
	else
	  blue = p;

	// Each component is in [0, 1].
	red_t   r = to_r(red * 255);
	green_t g = to_g(green * 255);
	blue_t  b = to_b(blue * 255);

	T_rgb rgb_result(r, g, b);

	return rgb_result;
      }

# endif // !MLN_INCLUDE_ONLY

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln


#endif // ! MLN_FUN_V2V_HSL_TO_RGB_HH
