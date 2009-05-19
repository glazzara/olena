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

/// \file mln/fin/v2v/hsl_to_hsv.hh
///
/// Conversion between HSL and HSV.
///
/// \todo Write a better doc.
/// \todo Correct/write preconditions.


#ifndef MLN_FUN_V2V_HSL_TO_HSV_HH
# define MLN_FUN_V2V_HSL_TO_HSV_HH

# include <cmath>

# include <mln/trait/value_.hh>
# include <mln/math/max.hh>
# include <mln/math/min.hh>

# include "../../value/hsl.hh"
# include "../../value/hsv.hh"

namespace mln
{

  namespace fun
  {

    namespace v2v
    {

      template <typename T_hsv>
      struct f_hsl_to_hsv_ : public Function_v2v< f_hsl_to_hsv_<T_hsv> >
      {
	typedef T_hsv result;

	template <typename T_hsl>
	T_hsv operator()(const T_hsl& hsl) const;
      };

      extern f_hsl_to_hsv_<value::hsv_16f> f_hsl_to_hsv_16f;
      extern f_hsl_to_hsv_<value::hsv_8f> f_hsl_to_hsv_8f;

      template <typename T_hsl>
      struct f_hsv_to_hsl_ : public Function_v2v< f_hsv_to_hsl_<T_hsl> >
      {
	typedef T_hsl result;

	template <typename T_hsv>
        T_hsl operator()(const T_hsv& hsv) const;
      };

      extern f_hsv_to_hsl_<value::hsl_8f> f_hsv_to_hsl_8f;
      extern f_hsv_to_hsl_<value::hsl_16f> f_hsv_to_hsl_16f;


# ifndef MLN_INCLUDE_ONLY

      /// Global variables.
      /// \{
      f_hsl_to_hsv_<value::hsv_16f> f_hsl_to_hsv_16f;
      f_hsl_to_hsv_<value::hsv_8f> f_hsl_to_hsv_8f;
      f_hsv_to_hsl_<value::hsl_8f> f_hsv_to_hsl_8f;
      f_hsv_to_hsl_<value::hsl_16f> f_hsv_to_hsl_16f;
      /// \}

      template <typename T_hsv>
      template <typename T_hsl>
      inline
      T_hsv
      f_hsl_to_hsv_<T_hsv>::operator()(const T_hsl& hsl) const
      {
	T_hsv hsv;

	// FIXME: take hsl [[0..360], [0..1], [0..1]]

	hsv.hue() = hsl.hue();
	float l = hsl.lum() * 2;
	float s = hsl.sat() * ((hsl.lum() <= 1) ? hsl.lum() : 2 - hsl.lum());
	hsv.val() = (l + s) / 2;
	hsv.sat() = (2 * s) / (l + s);

	// return hsv [[0..360], [0..1], [0..1]]
	return hsv;
      }


      template <typename T_hsl>
      template <typename T_hsv>
      inline
      T_hsl
      f_hsv_to_hsl_<T_hsl>::operator()(const T_hsv& hsv) const
      {
	T_hsl hsl;

	// take hsv [[0..360], [0..1], [0..1]]

	hsl.hue() = hsv.hue();
	hsl.lum() = (2 - hsv.sat()) * hsv.val();
	hsl.sat() = hsv.sat() * hsv.val();
	hsl.sat() /= (hsl.lum() <= 1) ? hsl.lum() : 2 - hsl.lum();
	hsl.lum() /= 2;

	// FIXME: return hsl [[0..360], [0..1], [0..1]]
	return hsl;
      }

# endif // !MLN_INCLUDE_ONLY

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_FUN_V2V_HSL_TO_HSV_HH
