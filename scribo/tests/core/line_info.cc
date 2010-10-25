// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/label_16.hh>
#include <mln/io/pbm/load.hh>
#include <scribo/primitive/extract/components.hh>
#include <scribo/primitive/link/with_single_left_link.hh>
#include <scribo/primitive/group/from_single_link.hh>
#include <scribo/core/line_set.hh>

#include "tests/data.hh"


int main()
{
  using namespace scribo;
  using namespace mln;


  int ref[][9] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		   // baseline, meanline, ascent, descent, x_height, d_height, a_height, char_space, char_width
		   {         2,        2,      2,       2,        1,        0,        1,          1,          1 },
		   {         4,        4,      4,       4,        1,        0,        1,          0,          1 },
		   {         6,        6,      6,       6,        1,        0,        1,          0,          1 },
		   {         7,        7,      7,       7,        1,        0,        1,          0,          1 },
		   {        12,       12,      10,     12,        1,        0,        3,          0,          1 },
		   {        17,       15,      15,     17,        3,        0,        3,          0,          4 } };

  std::string img = SCRIBO_IMG_DIR "/pixels.pbm";

  image2d<bool> input;
  io::pbm::load(input, img.c_str());

  value::label_16 nbboxes;
  typedef image2d<value::label_16> L;
  component_set<L>
    text = primitive::extract::components(input, c8(), nbboxes);

  object_links<L> links = primitive::link::with_single_left_link(text, 30);

  object_groups<L> groups = primitive::group::from_single_link(links);

  line_set<L> lines = scribo::line_set<L>(groups);


  for_all_lines(l, lines)
  {
    mln_assertion(lines(l).baseline() == ref[l][0]);
    mln_assertion(lines(l).meanline() == ref[l][1]);
    mln_assertion(lines(l).ascent() == ref[l][2]);
    mln_assertion(lines(l).descent() == ref[l][3]);
    mln_assertion(lines(l).x_height() == unsigned(ref[l][4]));
    mln_assertion(lines(l).d_height() == ref[l][5]);
    mln_assertion(lines(l).a_height() == ref[l][6]);
    mln_assertion(lines(l).char_space() == unsigned(ref[l][7]));
    mln_assertion(lines(l).char_width() == unsigned(ref[l][8]));
  }

}


