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

#include <mln/core/alias/point1d.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/site_set/p_array.hh>

#include <mln/win/segment1d.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/convert/to_p_array.hh>


int main()
{
  using namespace mln;

  typedef p_array<point1d> vec1d;
  point1d p1 = point1d(6);
  win::segment1d win1d(5);
  vec1d v1(convert::to_p_array(win1d, p1));

  {
    def::coord col = 4;
    for (size_t i=0; i < v1.nsites(); i++)
      mln_assertion(v1[i] == point1d(col++));
  }

  typedef p_array<point2d> vec2d;
  point2d p2 = point2d(10,10);
  win::rectangle2d win2d(3, 3);
  vec2d v2(convert::to_p_array(win2d, p2));

  def::coord row = 9;
  for (unsigned col = 0; col < v2.nsites(); ++col)
  {
    if (col > 0 && !(col%3))
      ++row;
    mln_assertion(v2[col] == point2d(row, static_cast<def::coord>(9 + (col%3))));
  }
}
