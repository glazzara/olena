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

// \file

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/value/label_16.hh>

#include <scribo/filter/objects_on_border.hh>
#include <scribo/primitive/extract/components.hh>

#include "tests/data.hh"

int main()
{
  using namespace mln;
  using namespace scribo;

  std::string img = SCRIBO_IMG_DIR "/comp_on_borders.pbm";

  image2d<bool> input;
  io::pbm::load(input, img.c_str());

  value::label_16 nlabels;
  typedef component_set<image2d<value::label_16> > O;
  O comps = scribo::primitive::extract::components(input, c8(), nlabels);
  O filtered_comps = scribo::filter::components_on_border(comps);

  unsigned valid_comps = 0;
  for_all_comps(c, filtered_comps)
    if (filtered_comps(c).is_valid())
      ++valid_comps;

  mln_assertion(valid_comps == 6u);

}
