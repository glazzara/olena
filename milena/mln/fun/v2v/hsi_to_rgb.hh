// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_FUN_V2V_HSI_TO_RGB_HH
# define MLN_FUN_V2V_HSI_TO_RGB_HH

/// \file
///
/// Convert a rgb value to hsi.

#include <cmath>

#include <mln/value/rgb8.hh>
#include <mln/math/round.hh>

#include <mln/value/hsi.hh>


namespace mln
{


  namespace fun
  {

    namespace v2v
    {

      /// \brief Convert a hsi value to rgb.
      ///
      /// \ingroup modfunv2v
      //
      template <typename T_rgb>
      struct f_hsi_to_rgb_ : public Function_v2v< f_hsi_to_rgb_<T_rgb> >
      {
	typedef T_rgb result;

	template <typename T_hsi>
        T_rgb operator()(const T_hsi& hsi) const;

      };

      typedef f_hsi_to_rgb_<value::rgb8> f_hsi_to_rgb_3x8_t;

      extern f_hsi_to_rgb_3x8_t f_hsi_to_rgb_3x8;


# ifndef MLN_INCLUDE_ONLY

      /// Global variable.
      /// \{
      f_hsi_to_rgb_3x8_t f_hsi_to_rgb_3x8;
      /// \}

      template <typename T_rgb>
      template <typename T_hsi>
      inline
      T_rgb
      f_hsi_to_rgb_<T_rgb>::operator()(const T_hsi& hsi) const
      {
	typedef typename T_rgb::red_t   red_t;
	typedef typename T_rgb::green_t green_t;
	typedef typename T_rgb::blue_t  blue_t;

	static math::round<red_t>   to_r;
	static math::round<green_t> to_g;
	static math::round<blue_t>  to_b;

	static const float
	  sqrt3_3   = std::sqrt(3) / 3,
	  inv_sqrt6 = 1 / std::sqrt(6),
	  inv_sqrt2 = 1 / std::sqrt(2);

	float
	  h	= hsi.hue() / 180.0 * 3.1415,
	  alpha = hsi.sat() * std::cos(h),
	  beta	= hsi.sat() * std::sin(h);


	red_t   r = to_r(sqrt3_3 * hsi.inty() + 2 * inv_sqrt6 * beta);
	green_t g =
	  to_g(sqrt3_3 * hsi.inty() + inv_sqrt2 * alpha - inv_sqrt6 * beta);
	blue_t  b =
	  to_b(sqrt3_3 * hsi.inty() - inv_sqrt2 * alpha - inv_sqrt6 * beta);

	T_rgb rgb(r, g, b);

	return rgb;
      }

# endif // !MLN_INCLUDE_ONLY

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_FUN_V2V_HSI_TO_RGB_HH
