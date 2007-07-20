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

/*! \file mln/value/props.hh
 *
 * \brief Define properties of value types.
 */

# include <climits>
# include <cfloat>

# include <mln/core/macros.hh>
# include <mln/value/kind.hh>


/// Get the minimum value of type \c T.
# define mln_min(T) mln::value::props< T >::min


/// Get the maximum value of type \c T.
# define mln_max(T) mln::value::props< T >::max


/// Get the number of values for value type \c T.
# define mln_card(T) mln::value::props< T >::card


/// Get the kind of value type \c T.
# define mln_kind(T) typename mln::value::props< T >::kind




namespace mln
{

  namespace value
  {


    /*! Class that defines the properties of the value type \c T.
     */
    template <typename T>
    struct props
    {
      /// Minimum value for type \c T.
      static const T min;

      /// Maximum value for type \c T.
      static const T max;

      /// Number of values for type \c T.
      static const std::size_t card;
    };



    // bool

    template <>
    struct props<bool>
    {
      static const bool min = false;
      static const bool max = true;
      static const std::size_t card = 2;
      typedef binary_kind kind;
    };


    // integers

    template <>
    struct props<unsigned char>
    {
      static const unsigned char min =   0;
      static const unsigned char max = 255;
      static const std::size_t  card = 256;
      typedef data_kind kind;
    };

    template <>
    struct props<signed char>
    {
      static const signed char  min = -128;
      static const signed char  max =  127;
      static const std::size_t card =  256;
      typedef data_kind kind;
    };

    template <>
    struct props<unsigned short>
    {
      static const unsigned short min =     0;
      static const unsigned short max = 65535;
      static const std::size_t   card = 65536;
      typedef data_kind kind;
    };

    template <>
    struct props<signed short>
    {
      static const signed short min = -32768;
      static const signed short max =  32767;
      static const std::size_t card = 655356;
      typedef data_kind kind;
    };

    template <>
    struct props<unsigned int>
    {
      static const unsigned int min = 0;
      static const unsigned int max = UINT_MAX;
      typedef data_kind kind;
      static const std::size_t card = 0;
    };

    template <>
    struct props<signed int>
    {
      static const signed int  min = INT_MIN;
      static const signed int  max = INT_MAX;
      typedef data_kind kind;
      static const std::size_t card = 0;
    };

    template <>
    struct props<unsigned long int>
    {
      static const unsigned long int min = 0;
      static const unsigned long int max = ULONG_MAX;
      typedef data_kind kind;
      static const std::size_t card = 0;
    };

    template <>
    struct props<signed long int>
    {
      static const signed long int min = LONG_MIN;
      static const signed long int max = LONG_MAX;
      typedef data_kind kind;
      static const std::size_t card = 0;
    };


    // floating

    template <>
    struct props<float>
    {
      static const float min() { return FLT_MIN; }
      static const float max() { return FLT_MAX; }
      typedef data_kind kind;
      static const std::size_t card = 0;
    };

    template <>
    struct props<double>
    {
      static const double min() { return DBL_MIN; }
      static const double max() { return DBL_MAX; }
      typedef data_kind kind;
      static const std::size_t card = 0;
    };

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_PROPS_HH
