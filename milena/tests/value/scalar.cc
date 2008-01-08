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

/*! \file tests/value/scalar.cc
 *
 * \brief Tests on mln::value::scalar.
 */

#include <iostream>

#include <mln/value/scalar.hh>
#include <mln/value/int_u8.hh>


template <typename T>
void foo(const T& t)
{
  std::cout << mln::value::scalar(t) << std::endl;
}

namespace mln
{
  namespace trait
  {
    // FIXME: Dead code?
//     template <typename O1, typename O2>
//     struct set_binary_< op::less, Object,O1, Object,O2 >  { typedef bool ret; };

//     template < typename Vl, typename Vr >
//     struct set_binary_< op::less, mln::value::Scalar, Vl, mln::value::Scalar, Vr >
//     {
//       typedef double ret;
//     };

//     template <typename B, typename O>
//     struct set_binary_< op::less,
// 			mln::value::Integer, B,
// 			mln::value::Scalar, O >
//     {
//       typedef bool ret;
//     };

  }
}


int main()
{
  using namespace mln;

  // FIXME: Dead code?
//   int i = 51;
//   foo(i);
//   foo( value::scalar(i) );


  {
    bool b;
    value::int_u8 i, j;

    b = 1 > value::scalar(j);

    b = value::scalar(j) < value::scalar(j);

  // FIXME: Dead code?
//     mln_trait_op_less_(value::scalar_< value::int_u8 >, value::scalar_< value::int_u8 >) tmp;
//     void* v = tmp;

//     mln_trait_op_less_(int, value::scalar_< value::int_u8 >) tmp;
//     void* v = tmp;
  }

}
