// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file
///
/// Test the L-infinity-norm.

#include <tests/norm/common.hh>

#include <mln/algebra/vec.hh>
#include <mln/math/abs.hh>
#include <mln/norm/linfty.hh>


namespace test
{
  template <typename V, typename S>
  void
  check_linfty_norm_and_distance(const V& vec1, const V& vec2, const S& ref_val)
  {
    // Pointer on mln::norm::linfty.
    typedef int (*linfty_t)(const V&);
    linfty_t linfty = mln::norm::linfty;

    test::check_norm(linfty, vec1, vec2);

    // Pointer on mln::norm::linfty_distance.
    typedef int (*linfty_distance_t)(const V&, const V&);
    linfty_distance_t linfty_distance = mln::norm::linfty_distance;

    test::check_distance(linfty_distance, vec1, vec2, ref_val);
  }
}

int main()
{
  typedef mln::algebra::vec<3, int> vec_t;

  // Reference value.
  float d = float(std::max(std::abs(4 - 2),
			   std::max(std::abs(1 + 2),
				    std::abs(0 - 4))));

  vec_t t, u;
  t.set(2, -2, 4);
  u.set(4,  1, 0);
  test::check_linfty_norm_and_distance(t, u, d);

  int v[] = {2, -2, 4};
  int w[] = {4,  1, 0};
  test::check_linfty_norm_and_distance(v, w, d);
}
