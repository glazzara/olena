// Copyright (C) 2013, 2014 EPITA Research and Development Laboratory (LRDE).
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

#include <iostream>
#include <mln/io/pbm/load.hh>
#include <mln/make/box2d.hh>
#include <scribo/layout/xy_cut.hh>

#include "tests/data.hh"

using namespace mln;

int main()
{

  image2d<bool> input;
  io::pbm::load(input, SCRIBO_IMG_DIR "/alignment_3.pbm");
  util::array<box2d> bb = scribo::layout::xy_cut(input, 10, 10);

  mln_assertion(bb.nelements() == 4);
  mln_assertion(bb[0] == make::box2d(20,66, 38,140));
  mln_assertion(bb[1] == make::box2d(11,171, 38,221));
  mln_assertion(bb[2] == make::box2d(0,237, 38,384));
  mln_assertion(bb[3] == make::box2d(50,60, 215,402));
}
