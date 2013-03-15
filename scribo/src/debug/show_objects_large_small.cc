// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#include <mln/data/convert.hh>

#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/save.hh>

#include <scribo/primitive/extract/components.hh>
#include <scribo/filter/objects_large.hh>
#include <scribo/filter/objects_small.hh>
#include <scribo/draw/bounding_boxes.hh>
#include <scribo/debug/usage.hh>

#include <scribo/debug/save_comp_diff.hh>

const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. True for objects and False for the "
    "background." },
  { "min_card", " Minimum cardinality in a component." },
  { "max_card", " Maximum cardinality in a component." },
  {0, 0}
};


int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;

  if (argc != 5)
    return scribo::debug::usage(argv,
				"Show components not being too small nor too"
				"large.",
				"input.pbm min_card max_card output.ppm",
				args_desc);

  mln_trace("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  value::label_16 nbboxes;
  typedef image2d<value::label_16> L;
  component_set<L> comps
    = scribo::primitive::extract::components(input, c8(), nbboxes);

  component_set<L> filter(comps);

  if (atoi(argv[2]) != 0)
    filter = scribo::filter::components_small(filter, atoi(argv[2]));

  if (atoi(argv[3]) != 0)
    filter = scribo::filter::components_large(filter, atoi(argv[3]));


  scribo::debug::save_comp_diff(comps, filter, argv[4]);
}
