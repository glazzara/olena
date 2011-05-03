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

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pbm/load.hh>

#include <scribo/core/component_set.hh>
#include <scribo/core/def/lbl_type.hh>
#include <scribo/core/object_links.hh>
#include <scribo/core/object_groups.hh>
#include <scribo/primitive/extract/components.hh>
#include <scribo/primitive/link/with_single_left_link.hh>

#include "tests/data.hh"

int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;

  std::string img = SCRIBO_IMG_DIR "/the_valleys.pbm";

  static const int comp_to_group_ref[] = { 0, 1, 4, 2, 3, 2, 2, 4, 4, 2, 2, 2, 2 };
  static const int pixel_area_ref[] = { 0, 3, 973, 39, 426 };
  static const box2d bbox_ref[] = {
    box2d(),
    make::box2d(0,91, 1,92),
    make::box2d(9,95, 45,224),
    make::box2d(9,204, 20,209),
    make::box2d(9,12, 36,64)
  };

  static const int comp_ids_count_ref[] = { 0, 1, 7, 1, 3 };
  static const int comp_ids_ref[][7] = {
    { 0, 0, 0, 0, 0, 0, 0 },
    { 1, 0, 0, 0, 0, 0, 0 },
    { 3, 9, 5, 6, 10, 11, 12 },
    { 4, 0, 0, 0, 0, 0, 0 },
    { 7, 2, 8, 0, 0, 0, 0 } };

  image2d<bool> input;
  io::pbm::load(input, img.c_str());

  typedef scribo::def::lbl_type V;
  V nlabels;
  typedef image2d<V> L;
  component_set<L>
    comps = scribo::primitive::extract::components(input, c8(), nlabels);

  object_links<L> link
    = primitive::link::with_single_left_link(comps, 30);

  object_groups<L> group(link);

  mln_assertion(group.nelements() == 5);
  mln_assertion(group.comp_to_group().nelements() == 13);

  // Checking comp_to_group()
  for (int i = 0; i < group.comp_to_group().nelements(); ++i)
    mln_assertion(group.comp_to_group()[i] == comp_to_group_ref[i]);

  // Checking group info data.
  for_all_groups(g, group)
  {
    mln_assertion(group(g).id() == g);
    mln_assertion(group(g).is_valid());
    mln_assertion(group(g).pixel_area() == pixel_area_ref[g]);
    mln_assertion(group(g).bbox() == bbox_ref[g]);
    mln_assertion(group(g).component_ids().nelements() == comp_ids_count_ref[g]);

    for_all_elements(e, group(g).component_ids())
      mln_assertion(group(g).component_ids()[e] == comp_ids_ref[g][e]);
  }

  // Checking group_of()
  for_all_comps(c, comps)
    mln_assertion(group.group_of(c).id() == comp_to_group_ref[c]);
}
