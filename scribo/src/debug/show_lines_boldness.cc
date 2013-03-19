// Copyright (C) 2011, 2012 EPITA Research and Development Laboratory
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
#include <mln/io/pbm/load.hh>
#include <mln/value/int_u16.hh>
#include <mln/draw/box_plain.hh>
#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/data/convert.hh>
#include <mln/literal/colors.hh>
#include <mln/math/round.hh>

#include <scribo/debug/usage.hh>

#include <scribo/core/component_set.hh>
#include <scribo/core/object_links.hh>
#include <scribo/core/object_groups.hh>

#include <scribo/text/extract_lines_with_features.hh>
#include <scribo/estim/components_features.hh>

#include <scribo/io/xml/save.hh>


const char *args_desc[][2] =
{
  {0, 0}
};



int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 3)
    return scribo::debug::usage(argv,
				"Show text lines",
				"input.pbm out_prefix",
				args_desc);

  mln_trace("main");

  image2d<bool> input;
  mln::io::pbm::load(input, argv[1]);

  std::string prefix = argv[2];

  typedef image2d<scribo::def::lbl_type> L;
  typedef value::int_u8 V;

  // Extract lines
  line_set<L>
    lines = text::extract_lines_with_features(
      data::convert(value::rgb8(), input), input, c8());
  const component_set<L>& comp_set = lines.components();

  // Min boldness / line
  {
    image2d<V> min_boldness;
    initialize(min_boldness, input);

    data::fill(min_boldness, 0);
    for_all_lines(l, lines)
      if (lines(l).is_textline())
      {
	float min = 65000;

	// Compute min
	for_all_line_comps(cid, lines(l).component_ids())
	{
	  unsigned id = lines(l).component_ids()(cid);
	  if (comp_set(id).features().boldness < min)
	    min = comp_set(id).features().boldness;
	}

	// Draw
	for_all_line_comps(cid, lines(l).component_ids())
	{
	  unsigned id = lines(l).component_ids()(cid);
	  data::fill(((min_boldness | comp_set(id).bbox()).rw()
		      | (pw::value(comp_set.labeled_image()) == pw::cst(id))).rw(),
		     mln::math::round<V>(min));
	}

      }

    mln::io::pgm::save(min_boldness, prefix + "_min_boldness.pgm");
  }

  // Max boldness / line
  {
    image2d<V> max_boldness;
    initialize(max_boldness, input);

    data::fill(max_boldness, 0);
    for_all_lines(l, lines)
      if (lines(l).is_textline())
      {
	float max = 65000;

	// Compute max
	for_all_line_comps(cid, lines(l).component_ids())
	{
	  unsigned id = lines(l).component_ids()(cid);
	  if (comp_set(id).features().boldness < max)
	    max = comp_set(id).features().boldness;
	}

	// Draw
	for_all_line_comps(cid, lines(l).component_ids())
	{
	  unsigned id = lines(l).component_ids()(cid);
	  data::fill(((max_boldness | comp_set(id).bbox()).rw()
		      | (pw::value(comp_set.labeled_image()) == pw::cst(id))).rw(),
		     mln::math::round<V>(max));
	}

      }

    mln::io::pgm::save(max_boldness, prefix + "_max_boldness.pgm");
  }


  // Mean boldness / line
  {
    image2d<V> min_boldness;
    initialize(min_boldness, input);

    data::fill(min_boldness, 0);
    for_all_lines(l, lines)
      if (lines(l).is_textline())
      {
	// Draw
	for_all_line_comps(cid, lines(l).component_ids())
	{
	  unsigned id = lines(l).component_ids()(cid);
	  data::fill(((min_boldness | comp_set(id).bbox()).rw()
		      | (pw::value(comp_set.labeled_image()) == pw::cst(id))).rw(),
		     mln::math::round<V>(lines(l).boldness()));
	}

      }

    mln::io::pgm::save(min_boldness, prefix + "_mean_boldness.pgm");
  }

  // Stddev boldness / line
  {
    image2d<V> min_boldness;
    initialize(min_boldness, input);

    data::fill(min_boldness, 0);
    for_all_lines(l, lines)
      if (lines(l).is_textline())
      {
	// Draw
	for_all_line_comps(cid, lines(l).component_ids())
	{
	  unsigned id = lines(l).component_ids()(cid);
	  data::fill(((min_boldness | comp_set(id).bbox()).rw()
		      | (pw::value(comp_set.labeled_image()) == pw::cst(id))).rw(),
		     mln::math::round<V>(lines(l).boldness_reliability()));
	}

      }

    mln::io::pgm::save(min_boldness, prefix + "_stddev_boldness.pgm");
  }


}

