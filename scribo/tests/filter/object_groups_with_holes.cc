// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pbm/load.hh>
#include <scribo/core/def/lbl_type.hh>
#include <scribo/primitive/extract/components.hh>
#include <scribo/primitive/link/with_single_left_link.hh>
#include <scribo/primitive/group/from_single_link.hh>
#include <scribo/filter/object_groups_with_holes.hh>

#include "tests/data.hh"

int main()
{
  using namespace scribo;
  using namespace mln;

  std::string img = SCRIBO_IMG_DIR "/the_valleys.pbm";

  const unsigned ref[] = { 0, 1, 7, 3, 4, 3, 3, 7, 7, 3, 3, 3, 3 };
  const unsigned filtered_ref[] = { 0, 0, 7, 3, 0, 3, 3, 7, 7, 3, 3, 3, 3 };

  image2d<bool> input;
  io::pbm::load(input, img.c_str());

  typedef scribo::def::lbl_type V;
  typedef image2d<V> L;

  V nbboxes;
  component_set<L>
    text = primitive::extract::components(input, c8(), nbboxes);
  object_links<L> links = primitive::link::with_single_left_link(text, 30);

  object_groups<L> groups = primitive::group::from_single_link(links);

  for_all_groups(g, groups)
    mln_assertion(groups(g) == ref[g]);

  groups = filter::object_groups_with_holes(groups, 3);

  for_all_groups(g, groups)
    mln_assertion(groups(g) == filtered_ref[g]);
}
