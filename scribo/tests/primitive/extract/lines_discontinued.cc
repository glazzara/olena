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

// \file

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/win/hline2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/value/label_8.hh>
#include <mln/make/box2d.hh>
#include <scribo/primitive/extract/lines_discontinued.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using namespace scribo;

  image2d<bool> input;
  io::pbm::load(input, SCRIBO_IMG_DIR "/lines_discontinued.pbm");

  typedef value::label_8 V;
  typedef image2d<V> I;

  // Horizontal
  {
    win::hline2d win(51);
    V nlines;
    component_set<I>
      comps = primitive::extract::lines_discontinued(input, c4(), nlines,
						   win, 3);

    mln_assertion(comps.nelements() == 2);

    mln_assertion(comps(1).bbox() == make::box2d(21, 37, 24, 176));
    mln_assertion(comps(1).mass_center() == point2d(23, 107));
    mln_assertion(comps(1).card() == 560);

    mln_assertion(comps(2).bbox() == make::box2d(94, 32, 97, 91));
    mln_assertion(comps(2).mass_center() == point2d(96, 62));
    mln_assertion(comps(2).card() == 240);
  }

  // Vertical
  {
    win::vline2d win(51);
    V nlines;
    component_set<I>
      comps = primitive::extract::lines_discontinued(input, c4(), nlines,
						   win, 3);

    mln_assertion(comps.nelements() == 3);

    mln_assertion(comps(1).bbox() == make::box2d(8, 23, 186, 26));
    mln_assertion(comps(1).mass_center() == point2d(97, 25));
    mln_assertion(comps(1).card() == 716);

    mln_assertion(comps(2).bbox() == make::box2d(42, 98, 162, 100));
    mln_assertion(comps(2).mass_center() == point2d(102, 99));
    mln_assertion(comps(2).card() == 363);

    mln_assertion(comps(3).bbox() == make::box2d(57, 137, 173, 144));
    mln_assertion(comps(3).mass_center() == point2d(115, 141));
    mln_assertion(comps(3).card() == 936);
  }
}
