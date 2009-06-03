// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file tests/fun/v2w_22v/norm.cc
 *
 *  \brief Test the norm functors.
 */

#include <mln/algebra/vec.hh>
#include <mln/fun/v2w_w2v/norm.hh>
#include <tests/norm/common.hh>

int main()
{
  typedef mln::algebra::vec<3, float> vec_t;

  // L1-norm.
  {
    vec_t t, u;
    t.set (1, -2, 3);
    u.set (5,  1, 0);
    mln::fun::v2w_w2v::l1_norm<vec_t, float> l1;
    test::check_norm(l1, t, u);

    float v = l1(t);
    if (!(l1.f_1(t, v) == t))
      return 1; // FIXME : how to check it cleanly ?
  }

  // L2-norm.
  {
    vec_t t, u;
    t.set (2, -2, 3);
    u.set (4,  1, 0);
    mln::fun::v2w_w2v::l2_norm<vec_t, float> l2;
    test::check_norm(l2, t, u);
  }

  // L-infinity-norm.
  {
    vec_t t, u;
    t.set (2, -2, 4);
    u.set (4,  1, 0);
    mln::fun::v2w_w2v::linfty_norm<vec_t, int> linfty;
    test::check_norm(linfty, t, u);
  }
}
