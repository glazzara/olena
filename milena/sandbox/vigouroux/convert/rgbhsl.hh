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


#ifndef OLENA_CONVERT_NRGBHSL_HH
# define OLENA_CONVERT_NRGBHSL_HH

# include <oln/basics.hh>
# include <oln/convert/abstract/colorconv.hh>

# include <ntg/basics.hh>
# include <ntg/color/nrgb.hh>
# include <ntg/color/hsl.hh>

# include <mlc/contract.hh>

# include <cstdlib>
# include <sstream>

/*! \file  olena/oln/convert/rgbhsl.hh
**
** REF: The formulas used here come from ``Color space conversion''; Paul
** Bourke.
*/
namespace oln {

  using namespace ntg;

  namespace convert {
    /*! Functor for conversion from RGB to HSL color space.
    **
    ** \note Every conversion should go though the RGB color space.
    **
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <oln/convert/rgbhsl.hh>
    ** #include <ntg/all.hh>
    **
    ** int main()
    ** {
    **   oln::image2d<ntg::rgb_8> lena_rgb = oln::load(IMG_IN "lena.ppm");
    **
    **   oln::image2d<ntg::hsl_8> lena_hsl = apply(oln::convert::f_rgb_to_hsl<8, 8>(), lena_rgb);
    **   oln::image2d<ntg::hsl_8>::iter_type it(lena_hsl);
    **   for_all(it)
    **     lena_hsl[it][ntg::hsl_L] = 127;
    **
    **   oln::io::save(apply(oln::convert::f_hsl_to_rgb<8, 8>(), lena_hsl),
    ** 		IMG_OUT "oln_convert_f_rgb_to_hsl.pgm");
    ** }
    ** \endcode
    ** \image html lena_ppm.png
    ** \image latex lena_ppm.png
    ** =>
    ** \image html oln_convert_f_rgb_to_hsl.png
    ** \image latex oln_convert_f_rgb_to_hsl.png
    */
    template<unsigned inbits, unsigned outbits>
    struct f_rgb_to_hsl
      : public abstract::color_conversion<3, inbits, rgb_traits,
					  3, outbits, hsl_traits, f_rgb_to_hsl<inbits, outbits> >
    {
      color<3, outbits, hsl_traits>
      doit(const color<3, inbits, rgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;

	float max_in = ntg::max(in[rgb_R], std::max(in[rgb_B], in[rgb_G]));
	float min_in = ntg::min(in[rgb_R], std::min(in[rgb_B], in[rgb_G]));
	float diff = max_in-min_in;

	out[hsl_L] = (max_in + min_in) / 2;
	if (std::abs(diff) <= FLT_EPSILON){
	  out[hsl_S] = 0;
	  out[hsl_H] = 0; // undefined
	}
	else {
	  if (out[hsl_L] <= 0.5)
	    out[hsl_S] = diff / (max_in + min_in);
	  else
	    out[hsl_S] = diff / (2 - max_in - min_in);


	  float r_dist = (max_in - in[rgb_R]) / diff;
	  float g_dist = (max_in - in[rgb_G]) / diff;
	  float b_dist = (max_in - in[rgb_B]) / diff;

	  if (in[rgb_R] == max_in)
	    out[hsl_H] = b_dist - g_dist;
	  else if(in[rgb_G] == max_in)
	    out[hsl_H] = 2 + r_dist - b_dist;
	  else if(in[rgb_B] == max_in)
	    out[hsl_H] = 4 + g_dist - r_dist;

	  out[hsl_H] *= 60;
	  if(out[hsl_H] < 0)
	    out[hsl_H] += 360;
	}
	return out;
      }

      static std::string
      name()
      {
	std::ostringstream s;
	s << "f_rgb_to_hsl<" << inbits << ", " << outbits << '>';
	s.str();
      }
    };

    /*! Conversion from RGB to HSL color space.
    **
    ** \see f_rgb_to_hsl
    */
    template <unsigned inbits, unsigned outbits>
    color<3, inbits, hsl_traits>
    rgb_to_hsl(const color<3, outbits, rgb_traits>& v)
    {
      f_rgb_to_hsl<inbits, outbits> f;
      return f(v);
    }

    namespace internal {
      float
      RGB(float q1, float q2, float hue)
      {
	if (hue >= 360)
	  hue -= 360;
	if (hue < 0)
	  hue += 360;
	if (hue < 60)
	  return q1 + (q2 - q1) * hue / 60;
	else if (hue < 180)
	  return q2;
	else if (hue < 240)
	  return q1 + (q2 - q1) * (240 - hue) / 60;
	else
	  return q1;
      }
    }

    /*! Functor for conversion from HSL to RGB color space.
    **
    ** \see f_rgb_to_hsl
    */
    template<unsigned inbits, unsigned outbits>
    struct f_hsl_to_rgb
      : public abstract::color_conversion<3, inbits, hsl_traits,
					  3, outbits, rgb_traits, f_hsl_to_rgb<inbits, outbits> >
    {
      color<3, outbits, rgb_traits>
      doit(const color<3, inbits, hsl_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	float p2;

 	if(in[hsl_L] < 0.5)
 	  p2 = in[hsl_L] * (1+in[hsl_S]);
 	else
 	  p2 = in[hsl_L] + in[hsl_S] - (in[hsl_L] * in[hsl_S]);

 	float p1 = 2 * in[hsl_L] - p2;

	if(in[hsl_S] == 0)
	  out[rgb_R] = out[rgb_G] = out[rgb_B] = in[hsl_L];
	else
	  {
	    out[rgb_R] = internal::RGB(p1, p2, in[hsl_H] + 120);
	    out[rgb_G] = internal::RGB(p1, p2, in[hsl_H]);
	    out[rgb_B] = internal::RGB(p1, p2, in[hsl_H] - 120);
	  }

	return out;
      }

      static std::string
      name()
      {
	std::ostringstream s;
	s << "f_hsl_to_rgb<" << inbits << ", " << outbits << '>';
	s.str();
      }
    };

    /*! Conversion from HSL to RGB color space.
    **
    ** \see f_rgb_to_hsl
    */
    template<unsigned inbits, unsigned outbits>
    color<3, outbits, rgb_traits>
    hsl_to_rgb(const color<3, inbits, hsl_traits>& v)
    {
      f_hsl_to_rgb<inbits, outbits> f;
      return f(v);
    }

  } // convert
} // oln

#endif // OLENA_CONVERT_RGBHSL_HH
