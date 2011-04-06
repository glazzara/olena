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
#include <scribo/filter/object_links_bbox_h_ratio.hh>

#include "tests/data.hh"

int main()
{
  using namespace scribo;
  using namespace mln;

  std::string img = SCRIBO_IMG_DIR "/phillip.pbm";

  const unsigned ref[] = { 0, 2, 6, 8, 3, 4, 6, 9, 2, 4 };
  const unsigned filtered_ref[] = { 0, 1, 6, 3, 3, 5, 6, 7, 8, 9 };

  image2d<bool> input;
  io::pbm::load(input, img.c_str());

  typedef scribo::def::lbl_type V;
  typedef image2d<V> L;

  V nbboxes;
  component_set<L>
    text = primitive::extract::components(input, c8(), nbboxes);

  object_links<L> links = primitive::link::with_single_left_link(text, 30);

  for_all_links(l, links)
    mln_assertion(links(l) == ref[l]);

  links = scribo::filter::object_links_bbox_h_ratio(links, 1.2f);

  for_all_links(l, links)
    mln_assertion(links(l) == filtered_ref[l]);
}
