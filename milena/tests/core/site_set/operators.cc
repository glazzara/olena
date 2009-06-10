// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/site_set/p_set.hh>
#include <mln/core/routine/ops.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/alias/box2d.hh>


int main()
{
  using namespace mln;

  point2d a(0,0), b(1,1), c(2,2);

  {
    p_set<point2d> s1, s2;
    s1.insert(a);
    s2.insert(b);
    mln_assertion(s1 != s2);
    mln_assertion(! (s1 == s2));
    mln_assertion(! (s1 < s2));
    mln_assertion(! (s1 <= s2));
    mln_assertion(! (s1 > s2));
    mln_assertion(! (s1 >= s2));
  }

  {
    p_set<point2d> s1, s2;
    s1.insert(a);
    s2.insert(b);
    s2.insert(a);
    mln_assertion(s1 != s2);
    mln_assertion(s1 < s2);
    mln_assertion(s1 <= s2);
    mln_assertion(s2 > s1);
    mln_assertion(! (s1 == s2));
    mln_assertion(! (s1 > s2));
    mln_assertion(! (s1 >= s2));
  }

  {
    box2d
      b1 = make::box2d(0,0, 1,1),
      b2 = make::box2d(0,0, 1,2);
    mln_assertion(b1 < b2);
    mln_assertion(! (b1 < b1));
    mln_assertion(! (b2 < b1));
  }
}
