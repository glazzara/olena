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

#ifndef MLN_FUN_V2V_RGB_TO_HSI_HH
# define MLN_FUN_V2V_RGB_TO_HSI_HH

#include <cmath>

#include <mln/value/rgb8.hh>
#include <mln/math/round.hh>

#include <mln/value/hsi.hh>


// FIXME: Split interface and implementation.

namespace mln
{


  namespace fun
  {

    namespace v2v
    {

      template <typename T_hsi>
      struct f_rgb_to_hsi_ : public Function_v2v< f_rgb_to_hsi_<T_hsi> >
      {
	typedef T_hsi result;

	template <typename T_rgb>
	T_hsi operator()(const T_rgb& rgb) const
	{
	  // Locals.

	  // wtf?

// 	  static const double sqrt3_3 = std::sqrt(3) / 3;
// 	  static const double inv_sqrt6 = 1 / std::sqrt(6);
// 	  static const double inv_sqrt2 = 1 / std::sqrt(2);

 	  T_hsi hsi;

// 	  double alpha = inv_sqrt2 * rgb.green() - inv_sqrt2 * rgb.blue();
// 	  double beta =
// 	    2 * inv_sqrt6 * rgb.red() -
// 	    inv_sqrt6 * rgb.green() -
// 	    inv_sqrt6 * rgb.blue();


// 	  hsi.hue() = atan2(beta, alpha) / 3.1415 * 180.0;
// 	  if (hsi.hue() < 0)
// 	    hsi.hue() = hsi.hue() + 360.0;
// 	  mln_invariant(hsi.hue() >= 0);
// 	  hsi.sat() = std::sqrt(alpha * alpha + beta * beta);
// 	  hsi.inty() =
// 	    sqrt3_3 * rgb.red() +
// 	    sqrt3_3 * rgb.green() +
// 	    sqrt3_3 * rgb.blue();

	  hsi.inty() = (double) (rgb.red() + rgb.green() + rgb.blue()) / 3 / mln_max(typename T_rgb::red_t);

	  typename T_rgb::red_t min = rgb.red();
	  if (rgb.green() < min)
	    min = rgb.green();

	  if (rgb.blue() < min)
	    min = rgb.blue();


	  if (hsi.inty())
	    hsi.sat() = (1 - (double) 3 * min / (double)(rgb.red() + rgb.green() + rgb.blue()));
	  else
	    hsi.sat() = 0; // this is black for any value of saturation

	  if (hsi.sat())
	    hsi.hue() = acos( 0.5 * (2 * rgb.red() - rgb.green() - rgb.blue()) /
			      std::sqrt((rgb.red() - rgb.green()) * (rgb.red() - rgb.green()) + (rgb.red() - rgb.blue()) * (rgb.green() - rgb.blue()))) / 3.1416;
	    else
	      hsi.hue() = 0; // this is grey for any value of hue

 	  if (hsi.hue() < 0)
 	    hsi.hue() = hsi.hue() + 1.0;

 	  mln_invariant(hsi.hue() >= 0);
 	  mln_invariant(hsi.hue() < 360);
 	  mln_invariant(hsi.sat() >= 0);
 	  mln_invariant(hsi.inty() >= 0);

	  return hsi;
	}
      };

      typedef f_rgb_to_hsi_<value::hsi_f> f_rgb_to_hsi_f_t;

      // FIXME: Warning: global object.
      f_rgb_to_hsi_f_t f_rgb_to_hsi_f;


      template <typename T_rgb>
      struct f_hsi_to_rgb_ : public Function_v2v< f_hsi_to_rgb_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_hsi>
	T_rgb operator()(const T_hsi& hsi) const
	{
	  typedef typename T_rgb::red_t   red_t;
	  typedef typename T_rgb::green_t green_t;
	  typedef typename T_rgb::blue_t  blue_t;

	  static math::round<red_t>   to_r;
	  static math::round<green_t> to_g;
	  static math::round<blue_t>  to_b;

	  static const float
	    sqrt3_3 = std::sqrt(3) / 3,
	    inv_sqrt6 = 1 / std::sqrt(6),
	    inv_sqrt2 = 1 / std::sqrt(2);

	  float
	    h = hsi.hue() / 180.0 * 3.1415,
	    alpha = hsi.sat() * std::cos(h),
	    beta = hsi.sat() * std::sin(h);


	  red_t   r = to_r(sqrt3_3 * hsi.inty() + 2 * inv_sqrt6 * beta);
	  green_t g =
	    to_g(sqrt3_3 * hsi.inty() + inv_sqrt2 * alpha - inv_sqrt6 * beta);
	  blue_t  b =
	    to_b(sqrt3_3 * hsi.inty() - inv_sqrt2 * alpha - inv_sqrt6 * beta);

	  T_rgb rgb(r, g, b);

	  return rgb;
	}
      };

      typedef f_hsi_to_rgb_<value::rgb8> f_hsi_to_rgb_3x8_t;

      // FIXME: Warning: global object.
      f_hsi_to_rgb_3x8_t f_hsi_to_rgb_3x8;

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_FUN_V2V_RGB_TO_HSI_HH
