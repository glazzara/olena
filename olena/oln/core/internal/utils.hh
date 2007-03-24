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

# include <mlc/contract.hh>


namespace oln
{

  namespace internal
  {


    /// Simple singleton class.

    template <typename T>
    struct singleton
    {
      singleton()
      {}
      singleton(const T& value)
	: value(value)
      {}
      T value;
    };

    template <typename T>
    struct singleton< T* >;


    /// Simple pair class.

    template <typename T1, typename T2>
    struct pair
    {
      pair()
      {}
      pair(const T1& first, const T2& second)
	: first(first),
	  second(second)
      {}
      T1 first;
      T2 second;
    };

    template <typename T1, typename T2>
    struct pair< T1*, T2 >
    {
      typedef pair<T1*,T2> self_t;

      // Not impled.
      pair();
      pair(const self_t&);
      void operator=(const self_t&);
      // end of Not impled.

      pair(T1* p_first, const T2& second)
	: first(*p_first),
	  second(second)
      {
	precondition(p_first != 0);
      }
      T1& first;
      T2  second;
      ~pair()
      {
	delete &first;
      }
    };

    template <typename T1, typename T2>
    struct pair< T1*, T2* >
    {
      typedef pair<T1*,T2*> self_t;

      // Not impled.
      pair();
      pair(const self_t&);
      void operator=(const self_t&);
      // end of Not impled.

      pair(T1* p_first, T2* p_second)
	: first(*p_first),
	  second(*p_second)
      {
	precondition(p_first  != 0);
	precondition(p_second != 0);
      }
      T1& first;
      T2& second;
      ~pair()
      {
	delete &first;
	delete &second;
      }
    };

    template <typename T1, typename T2>
    struct pair< T1, T2* >;


    /// Simple triplet class.

    template <typename T1, typename T2, typename T3>
    struct triplet
    {
      triplet()
      {}
      triplet(const T1& first, const T2& second, const T3& third)
	: first(first),
	  second(second),
	  third(third)
      {}
      T1 first;
      T2 second;
      T3 third;
    };

    template <typename T1, typename T2, typename T3>
    struct triplet< T1*, T2, T3 >
    {
      typedef triplet<T1*,T2,T3> self_t;

      // Not impled.
      triplet();
      triplet(const self_t&);
      void operator=(const self_t&);
      // end of Not impled.

      triplet(T1* p_first, const T2& second, const T3& third)
	: first(*p_first),
	  second(second),
	  third(third)
      {
	precondition(p_first != 0);
      }
      T1& first;
      T2  second;
      T3  third;
      ~triplet()
      {
	delete &first;
      }
    };


    /// Simple quartet class.

    template <typename T1, typename T2, typename T3, typename T4>
    struct quartet
    {
      quartet()
      {}
      quartet(const T1& first, const T2& second, const T3& third, const T4& fourth)
	: first(first),
	  second(second),
	  third(third),
	  fourth(fourth)
      {}
      T1 first;
      T2 second;
      T3 third;
      T4 fourth;
    };


  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_UTILS_HH
