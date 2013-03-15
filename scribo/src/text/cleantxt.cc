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

#include <mln/io/pbm/all.hh>

#include <mln/logical/not.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u16.hh>

#include <scribo/text/merging.hh>
#include <scribo/primitive/extract/components.hh>
#include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
#include <scribo/primitive/group/from_single_link.hh>
#include <scribo/text/clean.hh>
#include <scribo/debug/usage.hh>

#include <scribo/make/debug_filename.hh>

const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'True' for objects, 'False' for the "
    "background." },
  { "out.pbm",   "A cleaned up binary image." },
  {0, 0}
};



int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 3)
    return scribo::debug::usage(argv,
				"Cleanup text areas.",
				"input.pbm out.pbm",
				args_desc);

  mln_trace("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  scribo::make::internal::debug_filename_prefix = argv[2];

  typedef image2d<value::int_u16> L;
  value::int_u16 ncomps;
  component_set<L>
    comps = primitive::extract::components(input, c8(), ncomps);

  object_links<L>
    links = primitive::link::with_single_left_link_dmax_ratio(comps, 2);

  object_groups<L>
    groups = primitive::group::from_single_link(links);

  line_set<L> lines(groups);
  lines = text::merging(lines);

  logical::not_inplace(input);

  for_all_lines(l, lines)
    io::pbm::save(scribo::text::clean(lines(l), input),
		  scribo::make::debug_filename("text.pbm"));

}
