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

#include <mln/value/int_u8.hh>
#include <tests/value/macros.hh>


int main()
{
  using namespace mln;
  using value::int_u8;

  int_u8 i = 3, j;

  {
    int k = 0;
    k += value::scalar(k);
  }

  // Assignment.
  {
    i = 51;
    mln_assertion(i == 51u);
    mln_assertion(-i == -51);

    i = 51u;
    mln_assertion(i == 51u);

    signed char c = 51;
    i = c;
    mln_assertion(i == 51u);

    j = i;
    mln_assertion(j == 51u);

    i = 3;
    sym_compare_assert(3.0f, ==, i);
    sym_compare_assert(i, !=, 2.99f);

    // FIXME: Is this an incorrect behavior or what?
    // Error at run-time as expected :-)
    // i = 256;
    // i = -1;
    // i = 255, ++i;
  }


  // Comparaison
  {
    int_u8 i = 42;
    int_u8 j = 51;

    asym_compare_assert(i, <, j);
    asym_compare_assert(j, >, i);
    asym_compare_assert(i, <, 12345.f);
    asym_compare_assert(12345.f, >, i);

    sym_compare_assert(i, !=, j);
    sym_compare_assert(i, ==, 42.f);
    sym_compare_assert(42.f, ==, i);
    sym_compare_assert(i, !=, 69.f);
    sym_compare_assert(69.f, !=, i);

  }

  // Addition.
  {
    test_operator(int_u8, +, 5, 1);
    test_interop(int_u8, int,  +, 5, -1);
    test_interop(int_u8, char,  +, 4, 2);
    test_interop(int_u8, unsigned char,  +, 4, 2);

    int_u8 i = 234;

    i++;
    sym_compare_assert(i, ==, 235.f);

    ++i;
    sym_compare_assert(i, ==, 236.f);

    i = +i;
    sym_compare_assert(i, ==, 236.f);

  }

  // Soustraction
  {
    test_operator(int_u8, -, 100, 5);
    test_interop(int_u8, int,  -, 100, 5);
    test_interop(int_u8, char,  -, 100, 5);
    test_interop(int_u8, unsigned char,  -, 5, 5);

    int_u8 c = 255;
    c -= c;

    sym_compare_assert(c, ==, 0.f);

    int_u8 i = 236;

    i--;
    sym_compare_assert(i, ==, 235.f);

    --i;
    sym_compare_assert(i, ==, 234.f);

    sym_compare_assert(-i, ==, -234.f);

    sym_compare_assert(i * -2, !=, 0.f);
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
    sym_compare_assert(c, ==, 0.f);

    i *= 2;
    int k;  k *= i;

    unsigned char d = 0;
    i *= d;
    sym_compare_assert(i, ==, 0.f);

    // FIXME: Is this an incorrect behavior or what?
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
    sym_compare_assert(c, ==, 200.f);
    c /= 2;
    sym_compare_assert(c, ==, 100.f);

    int_u8 d = 2;
    c /= d;
    sym_compare_assert(c, ==, 50.f);

    // FIXME: Triggers a warning about signed vs unsigned comparison.
    // Read the todo and the remark in mln/core/routine/ops.hh.
    //
    // d /= 2.4f;
  }


  // Modulo
  {
    test_operator(int_u8, %, 5, 10);
    test_interop(int_u8, int,  %, 5, 10);
    test_interop(int_u8, char,  %, 4, 20);
    test_interop(int_u8, unsigned char,  %, 4, 20);
  }

}
