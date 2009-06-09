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

#include <iostream>
#include <mln/core/alias/point2d.hh>
#include <mln/literal/all.hh>



int main()
{
  using namespace mln;

  point2d p, q;

  // assignment

  p[0] = 4;
  p.row() += 1;
  mln_assertion(p.row() == 5 && p[0] == 5);
  p[1] = 0;
  p.col() += 1;
  mln_assertion(p.col() == 1 && p[1] == 1);

  // construction
  q = point2d(5, 1);
  mln_assertion(p == q);
  {
    point2d q_;
    q_ = point2d(5, 1);
    mln_assertion(q_ == q);
  }
  {
    point2d q_(5, 1);
    mln_assertion(q_ == q);
  }
  {
    point2d O(0,0);
    point2d O_ = literal::origin;
    mln_assertion(O_ == O);
    mln_assertion(O == literal::origin);
  }

  q.set_all(0);
  for (unsigned i = 0; i < p.dim; ++i)
    mln_assertion(q[i] == 0);
}
