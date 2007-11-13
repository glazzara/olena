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

/*! \file tests/norm/l1.hh
 *
 * \brief Test the L1-norm.
 */

#include <cassert>

#include <mln/metal/vec.hh>
#include <mln/math/abs.hh>
#include <mln/norm/l1.hh>

// FIXME: We should have a almost_equal function somewhere in Milena.
static const float epsilon = 0.0001;

using mln::metal::vec;
using mln::norm::l1;

template <typename V>
void check_l1 (const V& vec1, const V& vec2)
{
  assert (mln::math::abs(mln::norm::l1(vec1) - mln::norm::l1(vec2))
	  < epsilon);
}

template <typename V, typename S>
void check_l1_distance (const V& vec1, const V& vec2, const S& ref_val)
{
  assert (mln::math::abs(mln::norm::l1_distance(vec1, vec2) - ref_val)
	  < epsilon);
}

int main ()
{
  vec<3, int> t; t.set (1, -2, 3);
  vec<3, int> u; u.set (5,  1, 0);
  int d = (5 - 1) + (1 + 2) + 3;

  check_l1(t, u);
  check_l1_distance (t, u, d);

  int v[] = {1, -2, 3};
  int w[] = {5,  1, 0};

  check_l1(v, w);
  check_l1_distance (v, w, d);
}
