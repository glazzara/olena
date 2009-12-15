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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/make/image.hh>
#include <mln/fun/i2v/array.hh>

#include <scribo/core/object_image.hh>

#include <mln/debug/println.hh>

int main(int argc, char* argv[])
{
  using namespace mln;

  unsigned data[4][4] = { {1, 0, 0, 0},
			  {0, 0, 2, 0},
			  {3, 0, 0, 0},
			  {0, 0, 4, 0} };

  typedef image2d<unsigned> I;
  I ima = make::image(data);

  object_image(I) lbl(ima, 4);

  fun::i2v::array<unsigned> f(5);
  f(0) = 0;
  f(1) = 1;
  f(2) = 4;
  f(3) = 3;
  f(4) = 4;

  // { {1, 0, 0, 0},
  //   {0, 0, 2, 0},
  //   {3, 0, 0, 0},
  //   {0, 0, 2, 0} };
  lbl.relabel(f);


  mln_assertion(lbl.bbox(1) == make::box2d(0, 0, 0,0));
  mln_assertion(lbl.mass_center(1) == point2d(0,0));

  mln_assertion(lbl.bbox(2) == make::box2d(1, 2, 3,2));
  mln_assertion(lbl.mass_center(2) == point2d(2,2));

  mln_assertion(lbl.bbox(3) == make::box2d(2, 0, 2,0));
  mln_assertion(lbl.mass_center(3) == point2d(2,0));
}
