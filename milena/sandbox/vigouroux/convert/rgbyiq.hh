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


#ifndef OLENA_CONVERT_RGBYIQ_HH
# define OLENA_CONVERT_RGBYIQ_HH

# include <oln/convert/abstract/colorconv.hh>

# include <ntg/color/rgb.hh>
# include <ntg/color/yiq.hh>
# include <ntg/basics.hh>

# include <sstream>

/*! \file  olena/oln/convert/rgbyiq.hh
**
** REF: The formulas used here come from ``Digital Image Processing
** Algorithms and Applications'', I. Pitas; Wiley-Interscience.
*/
namespace oln {

  using namespace ntg;

  namespace convert {

    /* Functor for conversion from RGB to YIQ.
    **
    ** \see f_rgb_to_hsl
    */
    template<unsigned inbits, unsigned outbits>
    struct f_rgb_to_yiq
      : public abstract::color_conversion<3, inbits, rgb_traits,
					  3, outbits, yiq_traits, f_rgb_to_yiq<inbits, outbits> >
    {
      color<3, inbits, yiq_traits>
      doit(const color<3, outbits, rgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[yiq_Y] =
	  0.1768 * in[rgb_R] + 0.8130 * in[rgb_G] + 0.0101 * in[rgb_B];
	out[yiq_I] =
	  0.5346 * in[rgb_R] - 0.2461 * in[rgb_G] - 0.1791 * in[rgb_B];
	out[yiq_Q] =
	  0.2474 * in[rgb_R] - 0.6783 * in[rgb_G] + 0.4053 * in[rgb_B];
	return out;
      }

      static std::string
      name()
      {
	std::ostringstream s;
	s << "f_rgb_to_yiq<" << inbits << ", " << outbits << '>';
	s.str();
      }
    };

    /* Conversion from RGB to YIQ.
    **
    ** \see f_rgb_to_hsl
    */
    template <unsigned inbits, unsigned outbits>
    color<3, outbits, yiq_traits>
    rgb_to_yiq(const color<3, inbits, rgb_traits>& v)
    {
      f_rgb_to_yiq<inbits, outbits> f;

      return f(v);
    }

    /* Functor for conversion from YIQ to RGB.
    **
    ** \see f_rgb_to_hsl
    */
    template<unsigned inbits, unsigned outbits>
    struct f_yiq_to_rgb
      : public abstract::color_conversion<3, inbits, yiq_traits,
					  3, outbits, rgb_traits, f_yiq_to_rgb<inbits, outbits> >
    {
      color<3, inbits, rgb_traits>
      doit(const color<3, outbits, yiq_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[rgb_R] = 0.87   * in[yiq_Y] + 1.3223 * in[yiq_I] + 0.5628 * in[yiq_Q];
	out[rgb_G] = 1.026  * in[yiq_Y] - 0.2718 * in[yiq_I] - 0.1458 * in[yiq_Q];
	out[rgb_B] = 1.186  * in[yiq_Y] - 1.2620 * in[yiq_I] + 1.8795 * in[yiq_Q];
	return out;
      }

      static std::string
      name()
      {
	std::ostringstream s;
	s << "f_yiq_to_rgb<" << inbits << ", " << outbits << '>';
	s.str();
      }
    };

    /* Conversion from YIQ to RGB.
    **
    ** \see f_rgb_to_hsl
    */
    template <unsigned inbits, unsigned outbits>
    color<3, outbits, rgb_traits>
    yiq_to_rgb(const color<3, inbits, yiq_traits>& v)
    {
      f_yiq_to_rgb<inbits, outbits> f;

      return f(v);
    }

  } // convert
} // oln

#endif // OLENA_CONVERT_RGBYIQ_HH
