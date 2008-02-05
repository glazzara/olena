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


#ifndef OLENA_CONVERT_NRGBXYZ_HH
# define OLENA_CONVERT_NRGBXYZ_HH

# include <oln/convert/abstract/colorconv.hh>

# include <ntg/color/nrgb.hh>
# include <ntg/color/xyz.hh>
# include <ntg/basics.hh>

# include <sstream>

/*! \file olena/oln/convert/nrgbxyz.hh
**
** REF: The formulas used here come from ``Digital Image Processing
** Algorithms and Applications'', I. Pitas; Wiley-Interscience.
*/

namespace oln {

  // FIXME: should it be removed?
  using namespace ntg;

  namespace convert {

    /*! Functor for conversion from N-RGB to XYZ color space.
    **
    ** \deprecated A composition should be performed with nrgb->rgb and rgb->xyz. It has
    ** not been replaced within the function because a double conversion 'reduces'
    ** the color space. See the following example:
    ** \code
    ** // Obsolete:
    ** //
    ** // #include <oln/convert/nrgbxyz.hh>
    ** // #include <ntg/all.hh>
    ** // int main(int argc, char **argv)
    ** // {
    ** //   ntg::nrgb_8 in(100, 60, 64);
    ** //   ntg::xyz_8 out = oln::convert::f_nrgb_to_xyz<8, 8>()(in);
    ** // }
    ** //
    ** // Should be replaced by:
    ** //
    ** #include <oln/convert/rgbxyz.hh>
    ** #include <oln/convert/rgbnrgb.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   ntg::nrgb_8 in(100, 60, 64);
    **   ntg::xyz_8 out = oln::convert::f_rgb_to_xyz<8, 8>()
    **     (oln::convert::f_nrgb_to_rgb<8, 8>()(in));
    ** }
    ** \endcode
    */
    template <unsigned inbits, unsigned outbits>
    struct f_nrgb_to_xyz
      : public abstract::color_conversion<3, inbits, nrgb_traits,
					  3, outbits, xyz_traits, f_nrgb_to_xyz<inbits, outbits> >
    {
      color<3, outbits, xyz_traits>
      doit(const color<3, inbits, nrgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[xyz_X] =
	  0.606734 * in[nrgb_R] + 0.173564 * in[nrgb_G] + 0.200112 * in[nrgb_B];

	out[xyz_Y] =
	  0.298839 * in[nrgb_R] + 0.586811 * in[nrgb_G] + 0.114350 * in[nrgb_B];

	out[xyz_Z] =
	                          0.0661196 * in[nrgb_G] + 1.11491 * in[nrgb_B];

	return out;
      }

      static std::string
      name()
      {
	std::ostringstream s;
	s << "f_nrgb_to_xyz<" << inbits << ", " << outbits << '>';
	s.str();
      }
    };

    /*! Conversion from N-RGB to XYZ color space.
    **
    ** \deprecated A composition should be performed with nrgb->rgb and rgb->xyz.
    **
    ** \see f_nrgb_to_xyz for more information.
    */
    template <unsigned inbits, unsigned outbits>
    color<3, outbits, xyz_traits>
    nrgb_to_xyz(const color<3, inbits, nrgb_traits>& v)
    {
      f_nrgb_to_xyz<inbits, outbits> f;

      return f(v);
    }

    /*! Functor for conversion from XYZ to N-RGB color space.
    **
    ** \deprecated A composition should be performed with xyz->rgb and rgb->nrgb.
    **
    ** \see f_nrgb_to_xyz for more information.
    */
    template<unsigned inbits, unsigned outbits>
    struct f_xyz_to_nrgb
      : public abstract::color_conversion<3, inbits, xyz_traits,
					  3, outbits, nrgb_traits, f_xyz_to_nrgb<inbits, outbits> >
    {
      color<3, outbits, nrgb_traits>
      doit(const color<3, inbits, xyz_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[nrgb_R] =
	    1.91049 * in[xyz_X] - 0.532592 * in[xyz_Y] - 0.288284 * in[xyz_Z];
	out[nrgb_G] =
	  - 0.984310 * in[xyz_X] + 1.99845 * in[xyz_Y] - 0.0282980 * in[xyz_Z];
	out[nrgb_B] =
	    0.0583744 * in[xyz_X] - 0.118518 * in[xyz_Y] + 0.898611 * in[xyz_Z];
	return out;
      }

      static std::string
      name()
      {
	std::ostringstream s;
	s << "f_xyz_to_nrgb<" << inbits << ", " << outbits << '>';
	s.str();
      }
    };


    /*! Conversion from XYZ to N-RGB color space.
    **
    ** \deprecated a composition should be performed with xyz->rgb and rgb->nrgb.
    **
    ** \see f_nrgb_to_xyz for more information.
    */
    template <unsigned inbits, unsigned outbits>
    color<3, outbits, nrgb_traits>
    xyz_to_nrgb(const color<3, inbits, xyz_traits>& v)
    {
      f_xyz_to_nrgb<inbits, outbits> f;

      return f(v);
    }

  } // convert
} // oln

#endif // OLENA_CONVERT_NRGBXYZ_HH
