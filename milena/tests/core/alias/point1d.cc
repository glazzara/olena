// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

#include <mln/core/alias/point1d.hh>



int main()
{
  using namespace mln;

  point1d p, q;

  // assignment

  p[0] = 4;
  algebra::vec<1,float> v = p;
  (void) v;

  p.ind() += 1;
  mln_assertion(p.ind() == 5 && p[0] == 5);

  // construction
  q.ind() = 5;
  mln_assertion(p == q);

  q.set_all(0);
  for (unsigned i = 0; i < p.dim; ++i)
    mln_assertion(q[i] == 0);
}
