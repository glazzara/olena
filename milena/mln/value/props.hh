// Copyright (C) 2007 EPITA Research and Development Laboratory
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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_VALUE_PROPS_HH
# define MLN_VALUE_PROPS_HH

# include <climits>
# include <cfloat>

# define mln_min(T) mln::value::props<T>::min()
# define mln_max(T) mln::value::props<T>::max()


namespace mln
{

  namespace value
  {

    template <typename T>
    struct props
    {
      static T min();
      static T max();
    };


# ifndef MLN_INCLUDE_ONLY

    template <>
    struct props<bool>
    {
      static bool min() { return false; }
      static bool max() { return true; }
    };

    // integers

    template <>
    struct props<unsigned char>
    {
      static unsigned char min() { return 0; }
      static unsigned char max() { return 255; }
    };

    template <>
    struct props<signed char>
    {
      static signed char min() { return -128; }
      static signed char max() { return  127; }
    };

    template <>
    struct props<unsigned short>
    {
      static unsigned short min() { return 0; }
      static unsigned short max() { return 65535; }
    };

    template <>
    struct props<signed short>
    {
      static signed short min() { return -32768; }
      static signed short max() { return  32767; }
    };

    template <>
    struct props<unsigned int>
    {
      static unsigned int min() { return 0; }
      static unsigned int max() { return UINT_MAX; }
    };

    template <>
    struct props<signed int>
    {
      static signed int min() { return INT_MIN; }
      static signed int max() { return INT_MAX; }
    };

    template <>
    struct props<unsigned long int>
    {
      static unsigned long int min() { return 0; }
      static unsigned long int max() { return ULONG_MAX; }
    };

    template <>
    struct props<signed long int>
    {
      static signed long int min() { return LONG_MIN; }
      static signed long int max() { return LONG_MAX; }
    };

    // floating

    template <>
    struct props<float>
    {
      static float min() { return FLT_MIN; }
      static float max() { return FLT_MAX; }
    };

    template <>
    struct props<double>
    {
      static double min() { return DBL_MIN; }
      static double max() { return DBL_MAX; }
    };

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_PROPS_HH
