// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/value/int_s16.hh>

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
  using value::int_s16;

  int_s16 i = 3, j;

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

    // FIXME: Is this an incorrect behavior or what?
    // Error at run-time as expected :-)
    // i = 256;
    // i = -1;
    // i = 255, ++i;
  }


  // Comparaison
  {
    int_s16 i = 42;
    int_s16 j = 51;

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
    test_operator(int_s16, +, -5, 1);
    test_interop(int_s16, int,  +, 5, -1);
    test_interop(int_s16, char,  +, -4, 2);
    test_interop(int_s16, unsigned char,  +, 4, 2);

    int_s16 i = 234;

    i++;
    mln_assertion(i == 235);

    ++i;
    mln_assertion(i == 236);

    i = +i;
    mln_assertion(i == 236);

  }

  // Soustraction
  {
    test_operator(int_s16, -, 100, 5);
    test_interop(int_s16, int,  -, 100, 5);
    test_interop(int_s16, char,  -, 100, 5);
    test_interop(int_s16, unsigned char,  -, 100, 5);

    int_s16 c = 255;
    c -= c;

    mln_assertion(c == 0);

    int_s16 i = 236;

    i--;
    mln_assertion(i == 235);

    --i;
    mln_assertion(i == 234);

    mln_assertion(-i == -234);
  }

  // Multiplication
  {
    test_operator(int_s16, *, 5, 1);
    test_interop(int_s16, int,  *, 5, 1);
    test_interop(int_s16, char,  *, 4, 2);
    test_interop(int_s16, unsigned char,  *, 4, 2);

    int_s16 c = 255;

    c *= 0;
    mln_assertion(c == 0);

    i *= 2;
    int k;  k *= i;

    unsigned char d = 0;
    i *= d;
    mln_assertion(i == 0);

    // FIXME: Is this an incorrect behavior or what?
    // Error at run-time as expected :-)
    // i = 128;
    // i *= 2;

  }

  // Division
  {
    test_operator(int_s16, /, 5, 1);
    test_interop(int_s16, int,  /, 5, 1);
    test_interop(int_s16, char,  /, 4, 2);
    test_interop(int_s16, unsigned char,  /, 4, 2);

    int_s16 c = 200;

    c /= 1;
    mln_assertion(c == 200);
    c /= 2;
    mln_assertion(c == 100);

    int_s16 d = 2;
    c /= d;
    mln_assertion(c == 50);

  }


  // Modulo
  {
    test_operator(int_s16, %, 5, 10);
    test_interop(int_s16, int,  %, 5, 10);
    test_interop(int_s16, char,  %, 4, 20);
    test_interop(int_s16, unsigned char,  %, 4, 20);
  }
}
