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

#include <mln/algebra/vec.hh>
#include <mln/fun/v2v/norm.hh>

#include <tests/norm/common.hh>


int main()
{
  typedef mln::algebra::vec<3, int> vec_t;

  // L1-norm.
  {
    vec_t t, u;
    t.set (1, -2, 3);
    u.set (5,  1, 0);
    mln::fun::v2v::l1_norm<vec_t, float> l1;
    test::check_norm(l1, t, u);
  }

  // L2-norm.
  {
    vec_t t, u;
    t.set (2, -2, 3);
    u.set (4,  1, 0);
    mln::fun::v2v::l2_norm<vec_t, float> l2;
    test::check_norm(l2, t, u);
  }

  // L-infinity-norm.
  {
    vec_t t, u;
    t.set (2, -2, 4);
    u.set (4,  1, 0);
    mln::fun::v2v::linfty_norm<vec_t, int> linfty;
    test::check_norm(linfty, t, u);
  }
}
