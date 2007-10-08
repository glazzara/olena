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

/*! \file tests/value_interop.cc
 *
 * \brief Tests interoperability on mln::value::*.
 */

#include <mln/value/int_u8.hh>
#include <mln/value/int_s8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/float01_8.hh>

int main()
{
  using namespace mln;
  using value::int_u8;
  using value::int_s8;
  using value::int_u16;
  using value::float01_8;

  {
    // Operations on int_u<n>
    int_u8 i = 128;
    int_u16 j = 42;
    int_s8 k = 42;
    float01_8 x;

    (j = j + i) = i + j;
    assert(j == (128 + 42));

    (k = j - i) = i - j;
    assert(k == (-42));

    j = 2;
    (j = j * i) = i * j;
    assert(j == (2 * 128));

    (x = (j / i) / ((j / i) + 0.1))
      = (i / j) / ((i / j) + 0.1);

//     std::cout << i + i << std::endl;
//     float01_8 f = i / 200.5;
//     std::cout << x << std::endl;
  }

  {
    // Operations on int_u<n> and int / float
    int_u16 j = 42;
    int_s8 k = 42;
    float x;

    // int
    (k = j - 123) = 123 - j;
    (j =  j + 123) = 123 + j;
    (j =  j * 4) = 4 * j;

    (j =  j / 4) = 4 / j;

    // float
    x = (j / 4.5 * 3.4 + 3.5 - 5.6) / 0.0234;
  }


  {
    // Operations on int_u<n> and float01_8
    int_u16 j = 42;
    float01_8 x = 0.456;

    x = x / j;
    assert(x < 0 && x < 1);

    x = x * j;
    assert(x < 0 && x < 1);

    x = x * j;
    assert(x < 0 && x < 1);

  }
}
