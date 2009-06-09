// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
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

#include <tests/norm/common.hh>

#include <mln/algebra/vec.hh>
#include <mln/norm/l1.hh>


namespace test
{
  template <typename V, typename S>
  void
  check_l1_norm_and_distance(const V& vec1, const V& vec2, const S& ref_val)
  {
    // Pointer on mln::norm::l1.
    typedef mln_sum_(int) (*l1_t)(const V&);
    l1_t l1 = mln::norm::l1;

    test::check_norm(l1, vec1, vec2);

    // Pointer on mln::norm::l1_distance.
    typedef mln_sum_(int) (*l1_distance_t)(const V&, const V&);
    l1_distance_t l1_distance = mln::norm::l1_distance;

    test::check_distance(l1_distance, vec1, vec2, ref_val);
  }
}

int main()
{
  typedef mln::algebra::vec<3, int> vec_t;

  // Reference value.
  int d = (5 - 1) + (1 + 2) + 3;

  // Tests using mln::algebra::vec.
  vec_t t, u;
  t.set(1, -2, 3);
  u.set(5,  1, 0);
  test::check_l1_norm_and_distance(t, u, d);

  // Tests using plain arrays.
  int v[] = {1, -2, 3};
  int w[] = {5,  1, 0};
  test::check_l1_norm_and_distance(v, w, d);
}
