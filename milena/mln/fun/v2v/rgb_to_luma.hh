// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_FUN_V2V_RGB_TO_LUMA_HH
# define MLN_FUN_V2V_RGB_TO_LUMA_HH

# include <mln/value/rgb.hh>

namespace mln
{

  namespace fun
  {

    namespace v2v
    {

      template <typename T_luma>
      struct rgb_to_luma : public Function_v2v< rgb_to_luma<T_luma> >
      {
	typedef T_luma result;

	template <typename T_rgb>
	T_luma operator()(const T_rgb& rgb) const;

      };


# ifndef MLN_INCLUDE_ONLY

      template <typename T_luma>
      template <typename T_rgb>
      inline
      T_luma
      rgb_to_luma<T_luma>::operator()(const T_rgb& rgb) const
      {
	float luma = 0.299 * rgb.red() + 0.587 * rgb.green() + 0.114 * rgb.blue();
	return unsigned(luma + 0.49999);
      }


# endif // !MLN_INCLUDE_ONLY

    } // end of namespace fun::v2v

  } // end of namespace fun

} // end of namespace mln

#endif // ! MLN_FUN_V2V_RGB_TO_LUMA_HH
