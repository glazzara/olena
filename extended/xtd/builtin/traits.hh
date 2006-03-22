// Copyright (C) 2002, 2005, 2006  EPITA Research and Development Laboratory
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

#ifndef EXTENDED_BUILTIN_TRAITS_HH
# define EXTENDED_BUILTIN_TRAITS_HH

# include <xtd/traits.hh>


namespace xtd
{

  // FIXME: this is dummy and incomplete code!


  // plus

  template <typename T>
  struct plus_trait_ < T, T >
  {
    typedef T ret;
  };

  template <> struct plus_trait_ < float, int > { typedef float ret; };
  template <> struct plus_trait_ < int, float > { typedef float ret; };
  template <> struct plus_trait_ < double, int > { typedef double ret; };
  template <> struct plus_trait_ < int, double > { typedef double ret; };
  template <> struct plus_trait_ < double, float > { typedef double ret; };
  template <> struct plus_trait_ < float, double > { typedef double ret; };

  template <> struct plus_trait_ < long double, int > { typedef long double ret; };
  template <> struct plus_trait_ < int, long double > { typedef long double ret; };
  template <> struct plus_trait_ < long double, float > { typedef long double ret; };
  template <> struct plus_trait_ < float, long double > { typedef long double ret; };
  template <> struct plus_trait_ < long double, double > { typedef long double ret; };
  template <> struct plus_trait_ < double, long double > { typedef long double ret; };


  // mult

  template <typename T>
  struct mult_trait_ < T, T >
  {
    typedef T ret;
  };

  template <> struct mult_trait_ < float, int > { typedef float ret; };
  template <> struct mult_trait_ < int, float > { typedef float ret; };
  template <> struct mult_trait_ < double, int > { typedef double ret; };
  template <> struct mult_trait_ < int, double > { typedef double ret; };
  template <> struct mult_trait_ < double, float > { typedef double ret; };
  template <> struct mult_trait_ < float, double > { typedef double ret; };

  template <> struct mult_trait_ < long double, int > { typedef long double ret; };
  template <> struct mult_trait_ < int, long double > { typedef long double ret; };
  template <> struct mult_trait_ < long double, float > { typedef long double ret; };
  template <> struct mult_trait_ < float, long double > { typedef long double ret; };
  template <> struct mult_trait_ < long double, double > { typedef long double ret; };
  template <> struct mult_trait_ < double, long double > { typedef long double ret; };


} // end of namespace xtd



#endif // ! EXTENDED_BUILTIN_TRAITS_HH
