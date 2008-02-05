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


#ifndef OLENA_CONVERT_RGBXYZ_HH
# define OLENA_CONVERT_RGBXYZ_HH

# include <oln/convert/abstract/colorconv.hh>

# include <ntg/color/rgb.hh>
# include <ntg/color/xyz.hh>
# include <ntg/basics.hh>

# include <sstream>

/*! \file  olena/oln/convert/rgbxyz.hh
**
** REF: The formulas used here come from ``Digital Image Processing
** Algorithms and Applications'', I. Pitas; Wiley-Interscience.
*/

namespace oln {

  using namespace ntg;

  namespace convert {

    /* Functor for conversion from RGB to XYZ.
    **
    ** \see f_rgb_to_hsl
    */
    template <unsigned inbits, unsigned outbits>
    struct f_rgb_to_xyz
      : public abstract::color_conversion<3, inbits, rgb_traits,
					  3, outbits, xyz_traits, f_rgb_to_xyz <inbits, outbits> >
    {
      color<3, outbits, xyz_traits>
      doit(const color<3, inbits, rgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[xyz_X] = 0.490 * in[rgb_R] + 0.310 * in[rgb_G] + 0.200 * in[rgb_B];
	out[xyz_Y] = 0.177 * in[rgb_R] + 0.812 * in[rgb_G] + 0.011 * in[rgb_B];
	out[xyz_Z] =                     0.010 * in[rgb_G] + 0.990 * in[rgb_B];
	return out;
      }

      static std::string
      name()
      {
	std::ostringstream s;
	s << "f_rgb_to_xyz<" << inbits << ", " << outbits << '>';
	s.str();
      }
    };


    /* Conversion from RGB to XYZ.
    **
    ** \see f_rgb_to_hsl
    */
    template <unsigned inbits, unsigned outbits>
    color<3, outbits, xyz_traits>
    rgb_to_xyz(const color<3, inbits, rgb_traits>& v)
    {
      f_rgb_to_xyz<inbits, outbits> f;

      return f(v);
    }

    /* Functor for conversion from XYZ to RGB
    **
    ** \see f_rgb_to_hsl
    */
    template <unsigned inbits, unsigned outbits>
    struct f_xyz_to_rgb
      : public abstract::color_conversion<3, inbits, xyz_traits,
					  3, outbits, rgb_traits, f_xyz_to_rgb <inbits, outbits> >
    {
      color<3, outbits, rgb_traits>
      doit(const color<3, inbits, xyz_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[rgb_R] =
	    2.365 * in[xyz_X] - 0.896 * in[xyz_Y] - 0.468 * in[xyz_Z];
	out[rgb_G] =
	  - 0.515 * in[xyz_X] + 1.425 * in[xyz_Y] + 0.089 * in[xyz_Z];
	out[rgb_B] =
	    0.005 * in[xyz_X] - 0.014 * in[xyz_Y] + 1.01 * in[xyz_Z];
	return out;
      }

      static std::string
      name()
      {
	std::ostringstream s;
	s << "f_xyz_to_rgb<" << inbits << ", " << outbits << '>';
	s.str();
      }
    };

    /* Functor for conversion from RGB to XYZ.
    **
    ** \see f_rgb_to_hsl
    */
    template <unsigned inbits, unsigned outbits>
    color<3, outbits, rgb_traits>
    xyz_to_rgb(const color<3, outbits, xyz_traits>& v)
    {
      f_xyz_to_rgb<inbits, outbits> f;

      return f(v);
    }

  } // convert
} // oln

#endif // OLENA_CONVERT_RGBXYZ_HH
