// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
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

/// \file
/// \brief Exercise mln::draw::line.

#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/sub_image.hh>

#include <mln/data/fill.hh>
#include <mln/data/compare.hh>
#include <mln/draw/line.hh>
#include <mln/debug/println.hh>

int main()
{
  using namespace mln;

  point2d p1 = point2d(0,0);
  point2d p2 = point2d(6,9);
  p_line2d l(p1, p2);
  mln_assertion(l.nsites() == 10);

  image2d<bool> ima(10,10);
  data::fill(ima, false);
  draw::line(ima, p1, p2, true);

  image2d<bool> ima2(10,10);
  data::fill(ima2, false);
  data::paste(pw::cst(true) | l, ima2);
  mln_assertion(ima2 == ima);

  image2d<bool> ima3(10,10);
  data::fill(ima3, false);
  data::fill((ima3 | l).rw(), true);
  mln_assertion(ima3 == ima);
}
