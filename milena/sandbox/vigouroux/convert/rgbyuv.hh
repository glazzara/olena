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


#ifndef OLENA_CONVERT_NRGBYUV_HH
# define OLENA_CONVERT_NRGBYUV_HH

# include <oln/convert/abstract/colorconv.hh>

# include <ntg/color/rgb.hh>
# include <ntg/color/yuv.hh>
# include <ntg/basics.hh>

# include <sstream>

/*!\file  olena/oln/convert/rgbyuv.hh
**
** REF: The formulas used here come from ``Colour Space Conversions'',
** IAdrian Ford and Alan Roberts; August 11,1998.
*/
namespace oln {

  using namespace ntg;

  namespace convert {
    /* Functor for conversion from RGB to YUV.
    **
    ** \see f_rgb_to_hsl
    */
    template <unsigned inbits, unsigned outbits>
    struct f_rgb_to_yuv
      : public abstract::color_conversion<3, inbits, rgb_traits,
					  3, outbits, yuv_traits, f_rgb_to_yuv<inbits, outbits> >
    {
      color<3, outbits, yuv_traits>
      doit(const color<3, inbits, rgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[yuv_Y] =
	  + 0.177 * in[rgb_R] + 0.813  * in[rgb_G] + 0.01 * in[rgb_B];
	out[yuv_U] =
	  - 0.083 * in[rgb_R] - 0.434 * in[rgb_G] + 0.437 * in[rgb_B];
	out[yuv_V] =
	  + 0.583 * in[rgb_R] - 0.576  * in[rgb_G] + 0.071 * in[rgb_B];
	return out;
      }

      static std::string
      name()
      {
	std::ostringstream s;
	s << "f_rgb_to_yuv<" << inbits << ", " << outbits << '>';
	s.str();
      }
    };
    /* Conversion from RGB to YUV.
    **
    ** \see f_rgb_to_hsl
    */
    template <unsigned inbits, unsigned outbits>
    color<3, outbits, yuv_traits>
    rgb_to_yuv(const color<3, inbits, rgb_traits>& v)
    {
      f_rgb_to_yuv<inbits, outbits> f;

      return f(v);
    }
    /* Functor for conversion from YUV to RGB.
    **
    ** \see f_rgb_to_hsl
    */
    template<unsigned inbits, unsigned outbits>
    struct f_yuv_to_rgb
      : public abstract::color_conversion<3, inbits, yuv_traits,
					  3, outbits, rgb_traits, f_yuv_to_rgb<inbits, outbits> >
    {
      color<3, outbits, rgb_traits>
      doit(const color<3, inbits, yuv_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[rgb_R] = 0.8702 * in[yuv_Y] - 0.2487 * in[yuv_U] + 1.4250 * in[yuv_V];
	out[rgb_G] = 1.0259 * in[yuv_Y] + 0.0259 * in[yuv_U] - 0.3072 * in[yuv_V];
	out[rgb_B] = 1.1837 * in[yuv_Y] + 2.2642 * in[yuv_U] - 0.0359 * in[yuv_V];

	return out;
      }

      static std::string
      name()
      {
	std::ostringstream s;
	s << "f_yuv_to_rgb<" << inbits << ", " << outbits << '>';
	s.str();
      }
    };

    /* Conversion from YUV to RGB.
    **
    ** \see f_rgb_to_hsl
    */
    template <unsigned inbits, unsigned outbits>
    color<3, outbits, rgb_traits>
    yuv_to_rgb(const color<3, inbits, yuv_traits>& v)
    {
      f_yuv_to_rgb<inbits, outbits> f;

      return f(v);
    }

  } // convert
} // oln

#endif // OLENA_CONVERT_RGBYUV_HH
