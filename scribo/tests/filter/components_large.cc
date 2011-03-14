// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/util/graph.hh>
#include <mln/value/label_16.hh>
#include <mln/io/pbm/load.hh>
#include <mln/literal/colors.hh>
#include <scribo/primitive/extract/components.hh>
#include <scribo/primitive/group/apply.hh>
#include <scribo/primitive/link/with_single_left_link.hh>
#include <scribo/primitive/group/from_single_link.hh>
#include <scribo/filter/objects_large.hh>

#include "tests/data.hh"

int main()
{
  using namespace scribo;
  using namespace mln;

  std::string img = SCRIBO_IMG_DIR "/text_to_group_and_clean.pbm";

  image2d<bool> input;
  io::pbm::load(input, img.c_str());

  typedef value::label_16 V;
  typedef image2d<V> L;

  {
    V nbboxes;
    component_set<L>
      text = primitive::extract::components(input, c8(), nbboxes);

    object_links<L> links = primitive::link::with_single_left_link(text, 30);


    mln_assertion(nbboxes == 12u);

    object_groups<L> groups = primitive::group::from_single_link(links);

    component_set<L> grouped_comps = primitive::group::apply(groups);

    mln_assertion(grouped_comps.nelements() == 6u);

    component_set<L>
      filtered_comps = scribo::filter::components_large(grouped_comps, 20);

    unsigned valid_comps = 0;
    for_all_comps(c, filtered_comps)
      if (filtered_comps(c).is_valid())
	++valid_comps;

    mln_assertion(valid_comps == 4u);
  }


  {
    V nlabels;
    image2d<bool> output = scribo::filter::components_large(input, c8(),
							    nlabels, 20);

    mln_assertion(nlabels == 4u);
  }

}
