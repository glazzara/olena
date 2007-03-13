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

#ifndef OLN_CORE_INTERNAL_UTILS_HH
# define OLN_CORE_INTERNAL_UTILS_HH


namespace oln
{

  namespace internal
  {


    // FIXME: Rename attributes as first / second / third / fourth?



    /// Simple singleton class.

    template <typename T>
    struct singleton
    {
      singleton()
      {}
      singleton(T value)
	: value(value)
      {}
      T value;
    };


    /// Simple pair class.

    template <typename T1, typename T2>
    struct pair
    {
      pair()
      {}
      pair(T1 value1, T2 value2)
	: value1(value1),
	  value2(value2)
      {}
      T1 value1;
      T2 value2;
    };


    /// Simple triplet class.

    template <typename T1, typename T2, typename T3>
    struct triplet
    {
      triplet()
      {}
      triplet(T1 value1, T2 value2, T3 value3)
	: value1(value1),
	  value2(value2),
	  value3(value3)
      {}
      T1 value1;
      T2 value2;
      T3 value3;
    };


    /// Simple quartet class.

    template <typename T1, typename T2, typename T3, typename T4>
    struct quartet
    {
      quartet()
      {}
      quartet(T1 value1, T2 value2, T3 value3, T4 value4)
	: value1(value1),
	  value2(value2),
	  value3(value3),
	  value4(value4)
      {}
      T1 value1;
      T2 value2;
      T3 value3;
      T4 value4;
    };


  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_UTILS_HH
