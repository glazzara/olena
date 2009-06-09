// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/alias/box1d.hh>
#include <mln/core/alias/box2d.hh>
#include <mln/core/alias/box3d.hh>
#include <mln/fun/p2p/fold.hh>


int main()
{
  using namespace mln;

  {
    box1d b(2);
    fun::p2p::fold<point1d> f(b);
    point1d p(2);
    mln_assertion( f(p) == point1d(0) );
  }
  {
    box2d b(2, 3);
    point2d p(2,3);

    fun::p2p::fold<point2d,1,1> f_11(b);
    mln_assertion( f_11(p) == point2d(0,0) );

    fun::p2p::fold<point2d,0,1> f_01(b);
    mln_assertion( f_01(p) == point2d(2,0) );

    fun::p2p::fold<point2d,1,0> f_10(b);
    mln_assertion( f_10(p) == point2d(0,3) );
  }
  {
    box3d b(2, 3, 4);
    point3d p(2, 3, 4);

    fun::p2p::fold<point3d,1,1,1> f_111(b);
    mln_assertion( f_111(p) == point3d(0,0,0) );

    fun::p2p::fold<point3d,0,0,1> f_001(b);
    mln_assertion( f_001(p) == point3d(2,3,0) );

    fun::p2p::fold<point3d,0,1,0> f_010(b);
    mln_assertion( f_010(p) == point3d(2,0,4) );

    fun::p2p::fold<point3d,1,0,0> f_100(b);
    mln_assertion( f_100(p) == point3d(0,3,4) );
  }
}
