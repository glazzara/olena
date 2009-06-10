// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <cassert>

#include <mln/algebra/vec.hh>
#include <mln/fun/vv2v/max.hh>

int main()
{
  // Scalars.
  mln::fun::vv2v::max<int> max_int;
  assert (max_int(42, 51) == 51);

  // FIXME: We don't have vectors-with-an-order (yet), so this won't
  // work.
#if 0
  // Vectors.
  typedef mln::algebra::vec<3, int> vec_t;
  mln::fun::vv2v::max<vec_t> max_vec_t;
  vec_t t, u;
  t.set (1, -2, 3);
  u.set (5,  1, 0);
  assert (max_vec_t(t, u) == u);
#endif
}
