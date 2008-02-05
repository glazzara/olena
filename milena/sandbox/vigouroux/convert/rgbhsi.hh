// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.


#ifndef OLENA_CONVERT_RGBHSI_HH
# define OLENA_CONVERT_RGBHSI_HH

# include <oln/convert/abstract/colorconv.hh>

# include <ntg/basics.hh>
# include <ntg/color/rgb.hh>
# include <ntg/color/hsi.hh>

# include <sstream>

/*! \file  olena/oln/convert/rgbhsi.hh
**
** REF: The formulas used here come from ``Digital Image Processing
** Algorithms and Applications'', I. Pitas; Wiley-Interscience.
*/

namespace oln {

  using namespace ntg;

  namespace convert {

    static const float sqrt3_3   = sqrt(3) / 3;
    static const float inv_sqrt6 = 1 / sqrt(6);
    static const float inv_sqrt2 = 1 / sqrt(2);

    /*! Functor for conversion from RGB to HSI color space.
    **
    ** \see f_rgb_to_hsl
    */
    template<unsigned inbits, unsigned outbits>
    struct f_rgb_to_hsi
      : public abstract::color_conversion<3, inbits, rgb_traits,
					  3, outbits, hsi_traits, f_rgb_to_hsi<inbits, outbits> >
    {
      color<3, inbits, hsi_traits>
      doit(const color<3, outbits, rgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[hsi_I] =
	  sqrt3_3 * in[rgb_R] + sqrt3_3 * in[rgb_G] + sqrt3_3 * in[rgb_B];
	const float v1 = inv_sqrt2 * in[rgb_G] - inv_sqrt2 * in[rgb_B];
	const float v2 = 2 * inv_sqrt6 * in[rgb_R] - inv_sqrt6 * in[rgb_G]
	  - inv_sqrt6 * in[rgb_B];
	out[hsi_H] = atan2(v2, v1) / M_PI * 180.0;
	if (out[hsi_H] < 0)
	  out[hsi_H] += 360.0;
	assert(out[hsi_H] >= 0);
	out[hsi_S] = sqrt(v1 * v1 + v2 * v2);
	return out;
      }

      static std::string
      name()
      {
	std::ostringstream s;
	s << "f_rgb_to_hsi<" << inbits << ", " << outbits << '>';
	s.str();
      }
    };

    /*! Conversion from RGB to HSI color space.
    **
    ** \see f_rgb_to_hsl
    */
    template <unsigned inbits, unsigned outbits>
    color<3, outbits, hsi_traits>
    rgb_to_hsi(const color<3, inbits, rgb_traits>& v)
    {
      f_rgb_to_hsi<inbits, outbits> f;

      return f(v);
    }

    /*! Functor conversion from HSI to RGB color space.
    **
    ** \see f_rgb_to_hsl
    */
    template<unsigned inbits, unsigned outbits>
    struct f_hsi_to_rgb
      : public abstract::color_conversion<3, inbits, hsi_traits,
					  3, outbits, rgb_traits, f_hsi_to_rgb<inbits, outbits> >
    {
      color<3, outbits, rgb_traits>
      doit(const color<3, inbits, hsi_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	const float h = in[hsi_H] / 180.0 * M_PI;
	const float v1 = in[hsi_S] * cos(h);
	const float v2 = in[hsi_S] * sin(h);
	out[rgb_R] = sqrt3_3 * in[hsi_I] + 2 * inv_sqrt6 * v2;
	out[rgb_G] = sqrt3_3 * in[hsi_I] + inv_sqrt2 * v1 - inv_sqrt6 * v2;
	out[rgb_B] = sqrt3_3 * in[hsi_I] - inv_sqrt2 * v1 - inv_sqrt6 * v2;
	return out;
      }

      static std::string
      name()
	{
	std::ostringstream s;
	s << "f_hsi_to_rgb<" << inbits << ", " << outbits << '>';
	s.str();
      }
    };

    /*! Conversion from HSI to RGB color space.
    **
    ** \see f_rgb_to_hsl
    */
    template <unsigned inbits, unsigned outbits>
    color<3, outbits, rgb_traits>
    hsi_to_rgb (const color<3, inbits, hsi_traits>& v)
    {
      f_hsi_to_rgb<inbits, outbits> f;

      return f(v);
    }

  } // convert
} // oln

#endif // OLENA_CONVERT_RGBHSI_HH
