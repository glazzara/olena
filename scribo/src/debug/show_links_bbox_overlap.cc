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

#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>
#include <mln/literal/colors.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/save.hh>

#include <scribo/primitive/extract/components.hh>
#include <scribo/primitive/link/with_single_right_link.hh>
#include <scribo/primitive/link/with_single_left_link.hh>
#include <scribo/filter/object_links_bbox_overlap.hh>

#include <scribo/debug/decision_image.hh>
#include <scribo/debug/links_decision_image.hh>
#include <scribo/debug/usage.hh>



const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. True for objects and False for the background." },
  { "ratio", "Maximum bounding boxes overlap ratio. (common value : 0.7)" },
  {0, 0}
};


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace scribo::primitive::internal;
  using namespace mln;

  if (argc != 4)
    return scribo::debug::usage(argv,
				"Show valid or invalid links according"
				" the bboxes overlap.",
				"input.pbm ratio output.ppm",
				args_desc);

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  // Finding objects.
  value::label_16 nbboxes;
  typedef image2d<value::label_16> L;
  component_set<L> comps
    = scribo::primitive::extract::components(input, c8(), nbboxes);


  // Finding right links.
  object_links<L> right_links
    = primitive::link::with_single_right_link(comps);

  // Filtering.
  object_links<L> hratio_filtered_links
    = filter::object_links_bbox_overlap(right_links, atof(argv[2]));


  // Debug image.
  image2d<value::rgb8> hratio_decision_image
    = scribo::debug::links_decision_image(input,
					  right_links,
					  hratio_filtered_links);
  io::ppm::save(hratio_decision_image, argv[3]);

}
