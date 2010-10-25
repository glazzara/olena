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

// \file

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/value/label_8.hh>
#include <mln/value/label_32.hh>
#include <mln/data/convert.hh>
#include <mln/data/wrap.hh>

#include <scribo/filter/objects_with_holes.hh>
#include <scribo/primitive/extract/components.hh>
#include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
#include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>
#include <scribo/primitive/link/merge_double_link.hh>
#include <scribo/primitive/link/internal/dmax_width_and_height.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using namespace scribo;

  std::string img = SCRIBO_IMG_DIR "/pixels.pbm";

  unsigned ref_left[] = {0, 1, 1, 3, 4, 5, 6, 6, 8};
  unsigned ref_right[] = {0, 2, 2, 3, 4, 5, 6, 7, 8};
  unsigned ref_merged[] = {0, 1, 1, 3, 4, 5, 6, 7, 8};


  image2d<bool> input;
  io::pbm::load(input, img.c_str());

  value::label_16 nlabels;
  typedef image2d<value::label_16> L;
  typedef component_set<L> O;
  O comps = scribo::primitive::extract::components(input, c8(), nlabels);

  typedef primitive::link::internal::dmax_width_and_height dmax_fun_t;

  object_links<L> left_link
    = primitive::link::with_single_left_link_dmax_ratio(comps,
							dmax_fun_t(1),
							anchor::MassCenter);
  object_links<L> right_link
    = primitive::link::with_single_right_link_dmax_ratio(comps,
							 dmax_fun_t(1),
							 anchor::MassCenter);

  // Validating left and right links.
  object_links<L>
    merged_links = primitive::link::merge_double_link(left_link,
						      right_link);


  for_all_links(i, left_link)
    mln_assertion(ref_left[i] == left_link(i));

  for_all_links(i, right_link)
    mln_assertion(ref_right[i] == right_link(i));

  for_all_links(i, merged_links)
    mln_assertion(ref_merged[i] == merged_links(i));


}
