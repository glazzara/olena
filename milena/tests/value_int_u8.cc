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

/*! \file tests/value_int_u8.cc
 *
 * \brief Tests on mln::value::int_u8.
 */

#include <mln/value/int_u8.hh>

#define test_operator(T, OP, V1, V2)                    \
                                                        \
{							\
    T i = V1;						\
    T j = V2;						\
							\
    i = i OP j;						\
    mln_assertion(i == (V1 OP V2));			\
    mln_assertion(j == V2);				\
							\
    i OP##= i;						\
    mln_assertion(i == (((V1 OP V2) OP (V1 OP V2))));	\
}

#define test_interop(T1, T2, OP, V1, V2)                \
                                                        \
{							\
    T1 i = V1;						\
    T2 j = V2;						\
							\
    i = i OP j;						\
    mln_assertion(i == (V1 OP V2));			\
    mln_assertion(j == V2);				\
							\
    i OP##= i;						\
    mln_assertion(i == (((V1 OP V2) OP (V1 OP V2))));	\
}


// test_operator

int main()
{
  using namespace mln;
  using value::int_u8;

  int_u8 i = 3, j;

  // Assignment.
  {
    i = 51;
    mln_assertion(i == 51);

    i = 51u;
    mln_assertion(i == 51);

    signed char c = 51;
    i = c;
    mln_assertion(i == 51);

    j = i;
    mln_assertion(j == 51);

    i = 3;
    mln_assertion(3.0f == i);
    mln_assertion(i != 2.99f);

    // Error at run-time as expected :-)
    // i = 256;
    // i = -1;
    // i = 255, ++i;
  }


  // Comparaison
  {
    int_u8 i = 42;
    int_u8 j = 51;

    mln_assertion(i < j);
    mln_assertion(j > i);
    mln_assertion(i < 12345);
    mln_assertion(12345 > i);

    mln_assertion(i != j);
    mln_assertion(i == 42);
    mln_assertion(42 == i);
    mln_assertion(i != 69);
    mln_assertion(69 != i);

  }

  // Addition.
  {
    test_operator(int_u8, +, 5, 1);
    test_interop(int_u8, int,  +, 5, -1);
    test_interop(int_u8, char,  +, 4, 2);
    test_interop(int_u8, unsigned char,  +, 4, 2);

    int_u8 i = 234;

    i++;
    mln_assertion(i == 235);

    ++i;
    mln_assertion(i == 236);

    i = +i;
    mln_assertion(i == 236);

  }

  // Soustraction
  {
    test_operator(int_u8, -, 100, 5);
    test_interop(int_u8, int,  -, 100, 5);
    test_interop(int_u8, char,  -, 100, 5);
    test_interop(int_u8, unsigned char,  -, 100, 5);

    int_u8 c = 255;
    c -= c;

    mln_assertion(c == 0);

    int_u8 i = 236;

    i--;
    mln_assertion(i == 235);

    --i;
    mln_assertion(i == 234);

    std::cout << (-i) << " FIXME Matthieu: add .f to literals  " << i << std::endl;
    mln_assertion(-i == -234);   // FIXME: old version
    mln_assertion(-i == -234.f); // FIXME: new better version

    mln_assertion(i * -2 != 0.f);
    std::cout << (i) << " * -2 = "
	      << (i * -2) << ' '
	      << (-2 * i) << ' '
	      << (-2 * int(i))
	      << std::endl;
  }

  // Multiplication
  {
    test_operator(int_u8, *, 5, 1);
    test_interop(int_u8, int,  *, 5, 1);
    test_interop(int_u8, char,  *, 4, 2);
    test_interop(int_u8, unsigned char,  *, 4, 2);

    int_u8 c = 255;

    c *= 0;
    mln_assertion(c == 0);

    i *= 2;
    int k;  k *= i;

    unsigned char d = 0;
    i *= d;
    mln_assertion(i == 0);

    // Error at run-time as expected :-)
    // i = 128;
    // i *= 2;

  }

  // Division
  {
    test_operator(int_u8, /, 5, 1);
    test_interop(int_u8, int,  /, 5, 1);
    test_interop(int_u8, char,  /, 4, 2);
    test_interop(int_u8, unsigned char,  /, 4, 2);

    int_u8 c = 200;

    c /= 1;
    mln_assertion(c == 200);
    c /= 2;
    mln_assertion(c == 100);

    int_u8 d = 2;
    c /= 2;
    mln_assertion(c == 50);

  }


  // Modulo
  {
    test_operator(int_u8, %, 5, 10);
    test_interop(int_u8, int,  %, 5, 10);
    test_interop(int_u8, char,  %, 4, 20);
    test_interop(int_u8, unsigned char,  %, 4, 20);
  }
}
