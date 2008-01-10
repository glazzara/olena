// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

/*! \file tests/value/builtin.cc
 *
 * \brief Tests on mln::trait::op with a builtin.
 */

#include <iostream>

#include <mln/value/scalar.hh>
#include <mln/value/int_u8.hh>


namespace mln
{
  namespace trait
  {

    namespace op
    {

      template <typename T>
      struct test;

      template <typename L, typename R>
      struct test_2;

    }

    // unary

    template < typename S >
    struct set_unary_< op::test, mln::value::Scalar, S >
    {
      typedef double ret;
    };

    // FIXME: Dead code?
//     template <typename I>
//     struct set_unary_< op::test,
// 		       mln::value::Integer, I >
//     {
//       typedef float ret;
//     };

//     template <typename I>
//     struct set_unary_< op::test,
// 		       mln::value::Built_In, I >
//     {
//       typedef bool ret;
//     };


    // binary

    template < typename S1, typename S2 >
    struct set_binary_< op::test_2,
			mln::value::Scalar, S1,
			mln::value::Scalar, S2 >
    {
      typedef double ret;
    };

    // FIXME: Dead code?
//     template < typename I, typename S >
//     struct set_binary_< op::test_2,
// 			mln::value::Integer, I,
// 			mln::value::Scalar,  S >
//     {
//       typedef float ret;
//     };

//     template < typename B, typename S >
//     struct set_binary_< op::test_2,
// 			mln::value::Built_In, B,
// 			mln::value::Scalar,   S >
//     {
//       typedef bool ret;
//     };

  }
}


int main()
{
  using namespace mln;

  {
    mln::trait::solve_binary< trait::op::test_2, int, value::scalar_< value::int_u8 > >::ret tmp;
    double& d = tmp;
    // Avoid a warning about unused variable D.
    d = d;
  }
  {
    mln::trait::solve_unary< trait::op::test, int >::ret tmp;
    double& d = tmp;
    // Avoid a warning about unused variable D.
    d = d;
  }
}
