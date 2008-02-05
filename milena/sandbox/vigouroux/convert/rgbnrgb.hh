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


#ifndef OLENA_CONVERT_RGBNRGB_HH
# define OLENA_CONVERT_RGBNRGB_HH

# include <oln/convert/abstract/colorconv.hh>

# include <ntg/color/rgb.hh>
# include <ntg/color/nrgb.hh>
# include <ntg/basics.hh>

# include <sstream>

/*! \file  olena/oln/convert/rgbnrgb.hh
**
** REF: The formulas used here come from ``Digital Image Processing
** Algorithms and Applications'', I. Pitas; Wiley-Interscience.
*/

namespace oln {

  using namespace ntg;

  namespace convert {

    /*! Functor for conversion from RGB to N-RGB.
    **
    ** \see f_rgb_to_hsl
    */
    template<unsigned inbits, unsigned outbits>
    struct f_rgb_to_nrgb
      : public abstract::color_conversion<3, inbits, rgb_traits,
					  3, outbits, nrgb_traits, f_rgb_to_nrgb<inbits, outbits> >
    {
      color<3, outbits, nrgb_traits>
      doit(const color<3, inbits, rgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[nrgb_R] =
	  + 0.8417270*in[rgb_R] + 0.1560987*in[rgb_G] + 0.0906747*in[rgb_B];
	out[nrgb_G] =
	  - 0.1290152*in[rgb_R] + 1.3189264*in[rgb_G] - 0.2031832*in[rgb_B];
	out[nrgb_B] =
	  + 0.0074943*in[rgb_R] - 0.0688480*in[rgb_G] + 0.8972327*in[rgb_B];
	return out;
      }

      static std::string
      name()
      {
	std::ostringstream s;
	s << "f_rgb_to_nrgb<" << inbits << ", " << outbits << '>';
	s.str();
      }
    };

    /*! Conversion from RGB to N-RGB.
    **
    ** \see f_rgb_to_hsl
    */
    template <unsigned inbits, unsigned outbits>
    color<3, outbits, nrgb_traits>
    rgb_to_nrgb(const color<3, inbits, rgb_traits>& v)
    {
      f_rgb_to_nrgb<inbits, outbits> f;

      return f(v);
    }

    /*! Functor for conversion from N-RGB to RGB.
    **
    ** \see f_rgb_to_hsl
    */
    template <unsigned inbits, unsigned outbits>
    struct f_nrgb_to_rgb
      : public abstract::color_conversion<3, inbits, nrgb_traits,
					  3, outbits, rgb_traits, f_nrgb_to_rgb<inbits, outbits> >
    {
      color<3, outbits, rgb_traits>
      doit(const color<3, inbits, nrgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[rgb_R] =
	  + 1.167 * in[nrgb_R] - 0.146 * in[nrgb_G] - 0.151 * in[nrgb_B];
	out[rgb_G] =
	  + 0.114 * in[nrgb_R] + 0.753 * in[nrgb_G] + 0.159 * in[nrgb_B];
	out[rgb_B] =
	  - 0.001 * in[nrgb_R] + 0.059 * in[nrgb_G] + 1.128 * in[nrgb_B];
	return out;
      }

      static std::string
      name()
      {
	std::ostringstream s;
	s << "f_nrgb_to_rgb<" << inbits << ", " << outbits << '>';
	s.str();
      }
    };

    /*! Conversion from N-RGB to RGB.
    **
    ** \see f_rgb_to_hsl
    */
    template <unsigned inbits, unsigned outbits>
    color<3, outbits, rgb_traits>
    nrgb_to_rgb(const color<3, inbits, nrgb_traits>& v)
    {
      f_nrgb_to_rgb<inbits, outbits> f;

      return f(v);
    }

  } // convert
} // oln

#endif // OLENA_CONVERT_RGBNRGB_HH
