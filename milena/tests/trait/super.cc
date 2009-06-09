// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file
 *
 * \brief Tests on mln::trait::solve.
 */

#include <mln/trait/op/uminus.hh>
#include <mln/core/concept/object.hh>
#include <mln/value/int_u8.hh>


namespace my
{


  // Value.

  template <typename E> struct Value;

  template <> struct Value<void> { typedef mln::Object<void> super; };

  template <typename E>
  struct Value
  {
    typedef Value<void> category;
  protected:
    Value() {}
  };


  // Integer.

  template <typename E> struct Integer;

  template <> struct Integer<void> { typedef Value<void> super; };

  template <typename E>
  struct Integer
  {
    typedef Integer<void> category;
  };

  template <>
  struct Integer<int>
  {
    typedef Integer<void> category;
    int i;
    Integer(int i) : i(i) {}
  };


  // Built_In.

  template <typename E> struct Built_In;

  template <> struct Built_In<void> { typedef void* super; }; // cause several posible


  // test.

  struct test // like a built-in: no inheritance!
  {
    void is_test() {}
  };

  // FIXME: Dead code?
//   template <typename I>
//   void foo(const Integer<I>&)
//   {
//     std::cout << "Int" << std::endl;
//   }

//   template <typename I>
//   void foo(const Value<I>&)
//   {
//     std::cout << "Value" << std::endl;
//   }

} // my



namespace mln
{

  template <>
  struct category< my::test >
  {
    typedef my::Built_In< my::Integer<void> > ret;
  };

//   template <>
//   struct category< int >
//   {
//     typedef my::Built_In< my::Integer<void> > ret;
//   };

  namespace trait
  {

    template <typename T>
    struct set_unary_< op::uminus, my::Integer, T >
    {
      typedef bool ret;
    };

  } // mln::trait

} // mln


int main()
{
  using namespace mln;

  mln_trait_op_uminus_(my::test) tmp;
  tmp = true;

//   int i;
//   my::foo<int>(i);

//   mln_trait_op_plus_(value::int_u8, int) t;
//   void* v = t;
}
