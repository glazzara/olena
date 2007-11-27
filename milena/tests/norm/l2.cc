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

/*! \file tests/norm/l2.hh
 *
 *  \brief Test the L2-norm.
 */

#include <cmath>
#include <cassert>

#include <tests/norm/common.hh>

#include <mln/metal/vec.hh>
#include <mln/norm/l2.hh>


namespace test
{
  template <typename V, typename S>
  void
  check_l2_norm_and_distance(const V& vec1, const V& vec2, const S& ref_val)
  {
    // Pointer on mln::norm::l2.
    typedef mln_sum_(int) (*l2_t)(const V&);
    l2_t l2 = mln::norm::l2;

    test::check_norm(l2, vec1, vec2);

    // Pointer on mln::norm::l2_distance.
    typedef mln_sum_(int) (*l2_distance_t)(const V&, const V&);
    l2_distance_t l2_distance = mln::norm::l2_distance;

    test::check_distance(l2_distance, vec1, vec2, ref_val);
  }
}

int main()
{
  typedef mln::metal::vec<3, int> vec_t;

  // Reference value.
  float d = std::sqrt((4 - 2) * (4 - 2) +
		      (1 + 2) * (1 + 2) +
		      (0 - 3) * (0 - 3));

  // Tests using mln::metal::vec.
  vec_t t, u;
  t.set(2, -2, 3);
  u.set(4,  1, 0);
  test::check_l2_norm_and_distance(t, u, d);

  // Tests using plain arrays.
  int v[] = {2, -2, 3};
  int w[] = {4,  1, 0};
  test::check_l2_norm_and_distance(v, w, d);
}
