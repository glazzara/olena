// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_V2V_RGB_TO_HSV_HH
# define MLN_FUN_V2V_RGB_TO_HSV_HH

#include <mln/math/max.hh>
#include <mln/math/min.hh>

#include <mln/trait/promote.hh>

#include <mln/value/hsv.hh>
#include <mln/value/rgb.hh>

/// \fiie
///
/// This is the millet [millet.phd.2008.pdf] transformation from RGB
/// space to HSV space.

namespace mln
{

  namespace value
  {
    template <typename H, typename S, typename V> class hsv_;
    typedef hsv_<float, float, float>    hsv_f;
    typedef hsv_<double, double, double> hsv_d;
    template <unsigned n> struct rgb;
  }

  namespace fun
  {

    namespace v2v
    {

      template <typename T_hsv>
      struct f_rgb_to_hsv_ : public Function_v2v< f_rgb_to_hsv_<T_hsv> >
      {
	typedef T_hsv result;


	/// \brief HSV implementation from millet.2008.phd.pdf p67
	///
	/// \param[in] rgb the input rgb pixel.
	///
	/// \return a HSV pixel.
	///
	/// This is the Millet implementation of its transformation
	/// operator to go from RGB space to HSV space. When pixels
	/// are achromatic, hue equals -1.
	template <typename T_rgb>
	T_hsv operator()(const T_rgb& rgb) const;

      };

      typedef f_rgb_to_hsv_<value::hsv_f> f_rgb_to_hsv_f_t;

      extern f_rgb_to_hsv_f_t f_rgb_to_hsv_f;


# ifndef MLN_INCLUDE_ONLY

      /// Global variables.
      /// \{
      f_rgb_to_hsv_f_t f_rgb_to_hsv_f;
      /// \}


      template <typename T_hsv>
      template <typename T_rgb>
      inline
      T_hsv
      f_rgb_to_hsv_<T_hsv>::operator()(const T_rgb& rgb) const
      {
	typedef typename T_rgb::red_t                      t_red;
	typedef typename T_rgb::green_t                    t_green;
	typedef typename T_rgb::blue_t                     t_blue;
	typedef typename T_hsv::h_type                     t_hue;
	typedef typename T_hsv::s_type                     t_sat;
	typedef typename T_hsv::v_type                     t_val;
	typedef mln_trait_promote(t_hue, t_sat)            t_max_hue_sat;
	typedef mln_trait_promote(t_max_hue_sat, t_val)    t_max_hsv;

	// To obtain red between 0 and 1
	const t_max_hsv max_red = mln_max(t_red);
	const t_max_hsv min_red = mln_min(t_red);
	const t_max_hsv red     = (rgb.red() - min_red)/(max_red - min_red);

	// To obtain green between 0 and 1
	const t_max_hsv max_green = mln_max(t_green);
	const t_max_hsv min_green = mln_min(t_green);
	const t_max_hsv green    =(rgb.green()-min_green)/(max_green-min_green);

	// To obtain blue between 0 and 1
	const t_max_hsv max_blue = mln_max(t_blue);
	const t_max_hsv min_blue = mln_min(t_blue);
	const t_max_hsv blue     = (rgb.blue()-min_blue)/(max_blue-min_blue);

	const t_max_hsv max           = math::max(red, math::max(green, blue));
	const t_max_hsv min           = math::min(red, math::min(green, blue));
	const t_max_hsv max_minus_min = max-min;
	const t_max_hsv s             = 0.03;

	// Locals.
	T_hsv hsv;

	// SPECIFIC CASE WHEN PIXEL IS ACHROMATIC
	if (s > math::abs(red - blue) &&
	    s > math::abs(blue - red) &&
	    s > math::abs(red - green))
	{
	  hsv.val() = (red + green + blue)/3.0;
	  hsv.sat() = 0.0;
	  hsv.hue() = -1.0;
	}
	else
	{
	  hsv.val() = max;
	  hsv.sat() = max_minus_min / max;
	  if (max == red)
	    hsv.hue() = 60 * ((green - blue) / max_minus_min);
	  else if (max == green)
	    hsv.hue() = 60 * (2 + (blue - red) / max_minus_min);
	  else // (max == blue)
	    hsv.hue() = 60 * (4 + (red - green) / max_minus_min);
	}
	return hsv;
      }


# endif // !MLN_INCLUDE_ONLY

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_FUN_V2V_RGB_TO_HSV_HH
