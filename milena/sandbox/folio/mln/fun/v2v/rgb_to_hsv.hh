// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

/// \file mln/fin/v2v/rgb_to_hsv.hh
///
/// Conversion between RGB and HSV.
///
/// \todo Write a better doc.
/// \todo Correct/write preconditions.


#ifndef MLN_FUN_V2V_RGB_TO_HSV_HH
# define MLN_FUN_V2V_RGB_TO_HSV_HH

# include <cmath>

# include <mln/trait/value_.hh>
# include <mln/math/max.hh>
# include <mln/math/min.hh>

# include <mln/value/rgb.hh>

namespace mln
{

  namespace fun
  {

    namespace v2v
    {

      template <typename T_hsv>
      struct f_rgb_to_hsv_ : public Function_v2v< f_rgb_to_hsv_<T_hsv> >
      {
	typedef T_hsv result;

	template <typename T_rgb>
	T_hsv operator()(const T_rgb& rgb) const;
      };

      template <typename T_rgb>
      struct f_hsv_to_rgb_ : public Function_v2v< f_hsv_to_rgb_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_hsv>
        T_rgb operator()(const T_hsv& hsv) const;
      };


# ifndef MLN_INCLUDE_ONLY


      template <typename T_hsv>
      template <typename T_rgb>
      inline
      T_hsv
      f_rgb_to_hsv_<T_hsv>::operator()(const T_rgb& rgb) const
      {
	T_hsv hsv;

	// FIXME: take rgb [[0..1], [0..1], [0..1]]

	typename T_rgb::red_t rmax =
	  math::max(rgb.red(), math::max(rgb.green(), rgb.blue()));

	typename T_rgb::red_t rmin =
	  math::min(rgb.red(), math::min(rgb.green(), rgb.blue()));

	if (rmax == rmin)
	  hsv.hue() = 0;
	else if (rmax == rgb.red())
	  hsv.hue() = (60. * (rgb.green() - rgb.blue()) / (rmax - rmin));
	else if (rmax == rgb.green())
	  hsv.hue() = (60. * (rgb.blue() - rgb.red()) / (rmax - rmin)) + 120.;
	else if (rmax == rgb.blue())
	  hsv.hue() = (60. * (rgb.red() - rgb.green()) / (rmax - rmin)) + 240.;

	hsv.sat() = (rmax == 0) ? 0 : (1 - rmin / rmax);
	hsv.val() = rmax;

	// return hsv [[0..360], [0..1], [0..1]]
	return hsv;
      }


      template <typename T_rgb>
      template <typename T_hsv>
      inline
      T_rgb
      f_hsv_to_rgb_<T_rgb>::operator()(const T_hsv& hsv) const
      {
	// take hsv [[0..360], [0..1], [0..1]]

	float i = floor(hsv.hue() / 60);
	float f = hsv.hue() / 60 - i;
	float p = hsv.val() * (1 - hsv.sat());
	float q = hsv.val() * (1 - f * hsv.sat());
	float t = hsv.val() * (1 - (1 - f) * hsv.sat());

	T_rgb rgb;
	switch(i)
	{
	  case 0: rgb = T_rgb(hsv.val(), t, p); break;
	  case 1: rgb = T_rgb(q, hsv.val(), p); break;
	  case 2: rgb = T_rgb(p, hsv.val(), t); break;
	  case 3: rgb = T_rgb(p, q, hsv.val()); break;
	  case 4: rgb = T_rgb(t, p, hsv.val()); break;
	  case 5: rgb = T_rgb(hsv.val(), p, q); break;
	}

	// FIXME: return rgb [[0..1], [0..1], [0..1]]
	return rgb;
      }

# endif // !MLN_INCLUDE_ONLY

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_FUN_V2V_RGB_TO_HSV_HH
