// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/value/rgb.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/int_u8.hh>
#include <mln/literal/all.hh>


#define sym_compare_assert(V1, OP, V2)		\
						\
    mln_assertion(V1 OP V2);

#define compute_rgb(T, V1, OP, V2)		\
						\
    T(						\
      V1.red() OP V2.red() ,			\
      V1.green() OP V2.green() ,		\
      V1.blue() OP V2.blue()			\
    )

#define compute_rgb_sc(T, V1, OP, V2)		\
						\
    T(						\
      V1.red() OP V2 ,				\
      V1.green() OP V2 ,			\
      V1.blue() OP V2				\
    )

// Interop between 2 rgbs.
#define test_interop(T1, T2, OP, V1, V2)			\
{								\
    T1 i = V1;							\
    T2 j = V2;							\
								\
    i = i OP j;							\
    sym_compare_assert(i, ==, compute_rgb(T1, V1, OP, V2));	\
    sym_compare_assert(j, ==, V2);				\
								\
    i = V1;							\
    j = V2;							\
								\
    j = i OP j;							\
    sym_compare_assert(j, ==, compute_rgb(T1,V1, OP, V2));	\
    sym_compare_assert(i, ==, V1);				\
								\
    i = V1;							\
    i OP##= i;							\
    sym_compare_assert(i, ==, compute_rgb(T1,V1, OP, V1));	\
								\
    i = V1;							\
    j = V2;							\
    i OP##= j;							\
    sym_compare_assert(i, ==, compute_rgb(T1,V1, OP, V2));	\
								\
    j OP##= j;							\
    sym_compare_assert(j, ==, compute_rgb(T1,V2, OP, V2));	\
								\
    i = V1;							\
    j = V2;							\
    j OP##= i;							\
    sym_compare_assert(j, ==, compute_rgb(T1,V2, OP, V1));	\
								\
    i = V1;							\
    i OP##= i;							\
    sym_compare_assert(i, ==, compute_rgb(T1,V1, OP, V1));	\
}


// T1 is rgb, T2 is scalar
#define test_interop_sc(T1, T2, OP, V1, V2)		       \
{							       \
    T1 i = V1;						       \
    T2 j = V2;						       \
							       \
    i = i OP j;						       \
    sym_compare_assert(i, ==, compute_rgb_sc(T1,V1, OP, V2));  \
    sym_compare_assert(j, ==, V2);			       \
							       \
    i = V1;						       \
    j = V2;						       \
    i OP##= j;						       \
    sym_compare_assert(i, ==, compute_rgb_sc(T1, V1, OP, V2)); \
}

int main()
{
  using namespace mln;
  using value::rgb;
  using value::rgb8;
  using value::int_u8;
  using value::int_u;

  using literal::blue;
  using literal::white;

  {
    test_interop(rgb8, rgb8, +, rgb8(4,5,6), rgb8(1,2,3));
    test_interop(rgb8, rgb8, -, rgb8(4,5,6), rgb8(1,2,3));

    test_interop(rgb<10>, rgb<10>, +, rgb<10>(4,5,6), rgb<10>(1,2,3));
    test_interop(rgb<10>, rgb<10>, -, rgb<10>(4,5,6), rgb<10>(1,2,3));

    test_interop(rgb<16>, rgb<16>, +, rgb<16>(4,5,6), rgb<16>(1,2,3));
    test_interop(rgb<16>, rgb<16>, -, rgb<16>(4,5,6), rgb<16>(1,2,3));

    test_interop_sc(rgb8, int, *, rgb8(4,5,6), 4);
    test_interop_sc(rgb8, int, /, rgb8(40,50,60), 10);

    test_interop_sc(rgb8, unsigned, *, rgb8(4,5,6), 4);
    test_interop_sc(rgb8, unsigned, /, rgb8(40,50,60), 10);

    // FIXME : operators *,/ are ambiguous for (rgb8, float)...
    //     test_interop_sc(rgb8, float, *, rgb8(4,5,6), 4);
    //     test_interop_sc(rgb8, float, /, rgb8(40,50,60), 10);

    test_interop_sc(rgb8, char, *, rgb8(4,5,6), 4);
    test_interop_sc(rgb8, char, /, rgb8(40,50,60), 10);

    test_interop_sc(rgb8, int_u8, *, rgb8(4,5,6), 4);
    test_interop_sc(rgb8, int_u8, /, rgb8(40,50,60), 10);

    test_interop_sc(rgb8, int_u<7>, *, rgb8(4,5,6), 4);
    test_interop_sc(rgb8, int_u<7>, /, rgb8(40,50,60), 10);
  }
}
