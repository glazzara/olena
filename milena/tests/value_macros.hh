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

/*! \file tests/value_macros.hh
 *
 * \brief Utilities to tests value types.
 */

#define sym_compare_assert(V1, OP, V2)		\
						\
    mln_assertion(V1 OP V2);			\
    mln_assertion(-V1 OP -V2);			\
    mln_assertion(V2 OP V1);			\
    mln_assertion(-V2 OP -V1);

#define asym_compare_assert(V1, OP, V2)     	\
						\
    mln_assertion(V1 OP V2);			\
    mln_assertion(!(-V1 OP -V2));               \
    mln_assertion(-V2 OP -V1);			\
    mln_assertion(!(V2 OP V1));

// For unsigned types, V1 > V2 if op is minus
#define test_interop(T1, T2, OP, V1, V2)		\
							\
{							\
    T1 i = V1;						\
    T2 j = V2;						\
							\
    i = i OP j;						\
    sym_compare_assert(i, ==, float(V1 OP V2));		\
    sym_compare_assert(j, ==, float(V2));		\
							\
    i = V1;						\
    j = V2;						\
							\
    j = i OP j;						\
    sym_compare_assert(j, ==, float(V1 OP V2));		\
    sym_compare_assert(i, ==, float(V1));		\
							\
    i = V1;						\
    i OP##= i;						\
    sym_compare_assert(i, ==, float(V1 OP V1));		\
							\
    i = V1;						\
    j = V2;						\
    i OP##= j;						\
    sym_compare_assert(i, ==, float(V1 OP V2));		\
							\
    j OP##= j;						\
    sym_compare_assert(j, ==, float(V2 OP V2));		\
							\
    i = V1;						\
    j = V2;						\
    j OP##= i;						\
    sym_compare_assert(j, ==, float(V2 OP V1));		\
							\
    i = V1;						\
    i OP##= i;						\
    sym_compare_assert(i, ==, float(((V1 OP V1))));	\
}

#define test_operator(T, OP, V1, V2)		\
						\
{						\
    T i = V1;					\
    T j = V2;					\
						\
    i = i OP j;					\
    sym_compare_assert(i, ==, float(V1 OP V2));	\
    sym_compare_assert(j, ==, float(V2));	\
						\
    i = V1;					\
    i OP##= i;					\
    sym_compare_assert(i, ==, float(V1 OP V1));	\
						\
    i = V1;					\
    j = V2;					\
    i OP##= j;					\
}
