// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_VALUE_INTERNAL_CONVERT_HH
# define MLN_VALUE_INTERNAL_CONVERT_HH

/// \file
///
/// Define a conversion between an index and an iterable value.

# include <mln/core/contract.hh>
# include <mln/trait/value_.hh>


namespace mln
{

  namespace value
  {

    namespace internal
    {
      /// 
      template <typename T>
      struct convert_
      {
	// FIXME: Check that we have a type T compatible with 'int'.

	static T value_at_index(unsigned i);
	static unsigned index_of_value(const T& v);
      };

      /// 
      template <>
      struct convert_<bool>
      {
	static bool value_at_index(unsigned i);
	static unsigned index_of_value(bool v);
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename T>
      inline
      T
      convert_<T>::value_at_index(unsigned i)
      {
	return T( int(mln_min(T)) + int(i) );
      }

      template <typename T>
      inline
      unsigned
      convert_<T>::index_of_value(const T& v)
      {
	return unsigned( int(v) - int(mln_min(T)) );
      }

      // Case of 'bool'.

      inline
      bool
      convert_<bool>::value_at_index(unsigned i)
      {
	mln_precondition(i < 2);
	return i == 1 ? true : false;
      }

      inline
      unsigned
      convert_<bool>::index_of_value(bool v)
      {
	return v ? 1 : 0;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::value::internal

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_INTERNAL_CONVERT_HH
