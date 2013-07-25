// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

/// \file
/// \brief Exercise mln::draw::dashed_line.

#include <mln/core/image/image2d.hh>

#include <mln/draw/line.hh>
#include <mln/draw/dashed_line.hh>

#include <mln/data/fill.hh>
#include <mln/data/compare.hh>

int main()
{
  using namespace mln;

  point2d p1 = point2d(2,3);
  point2d p2 = point2d(2,3);

  image2d<bool> ima1(10,10);
  data::fill(ima1, false);
  draw::dashed_line(ima1, p1, p2, true);

  image2d<bool> ima2(10,10);
  data::fill(ima2, false);
  draw::line(ima2, p1, p2, true);

  mln_assertion(ima1 <= ima2);
}
