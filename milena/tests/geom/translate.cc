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

# include <mln/core/image/image2d.hh>
# include <mln/geom/translate.hh>
# include <mln/make/image.hh>
# include <mln/data/compare.hh>

#include <mln/debug/println.hh>

int main()
{
  using namespace mln;

  bool values[][5] = { { 0, 0, 1, 0, 0 },
                       { 0, 0, 1, 0, 0 },
                       { 0, 0, 1, 0, 0 },
                       { 0, 0, 1, 0, 0 },
                       { 0, 0, 1, 0, 0 } };

  image2d<bool> ima = make::image(values);

  box2d b = make::box2d(-2, -2, 2, 2);
  image2d<bool>
    ima_rot = geom::translate(ima,
			      -1 * ima.domain().center().to_vec(),
			      literal::zero, b);

  mln_piter_(image2d<bool>)
    p1(ima.domain()),
    p2(ima_rot.domain());

  for_all_2(p1,p2)
    mln_assertion(ima(p1) == ima_rot(p2));
}
