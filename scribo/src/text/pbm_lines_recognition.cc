// Copyright (C) 2010, 2013 EPITA Research and Development Laboratory (LRDE)
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

#include <scribo/text/recognition.hh>

#include <scribo/debug/usage.hh>

#include <scribo/core/component_set.hh>
#include <scribo/core/object_links.hh>
#include <scribo/core/object_groups.hh>

#include <scribo/text/merging.hh>
#include <scribo/primitive/extract/components.hh>
#include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
#include <scribo/primitive/group/from_single_link.hh>

#include <scribo/io/text_boxes/save.hh>


const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'True' for objects, 'False'\
for the background." },
  { "lang", "Must be set to \"eng\", \"fra\", \"deu\", \"spa\", \"ita\" (Default \"fra\")" },
  {0, 0}
};



int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 2 && argc != 3 && argc != 4)
    return scribo::debug::usage(argv,
				"Text recognition",
				"input.pbm [lang] [file]",
				args_desc);

  mln_trace("main");

  image2d<bool> input;
  mln::io::pbm::load(input, argv[1]);

  typedef image2d<value::int_u16> L;
  value::int_u16 ncomps;
  component_set<L>
    comps = scribo::primitive::extract::components(input, c8(), ncomps);
  std::cout << "ncomps = " << ncomps << std::endl;

  object_links<L>
    links = scribo::primitive::link::with_single_left_link_dmax_ratio(comps, 2);

  object_groups<L>
    groups = scribo::primitive::group::from_single_link(links);

  line_set<L> lines(groups);
  lines = text::merging(lines);

  std::string str = argv[2];
  const char *lang;
  if (argc < 3 || (str != "eng" && str != "fra"
		   && str != "deu" && str != "spa"
		   && str != "ita"))
    lang = "fra";
  else
    lang = argv[2];

  char *output = 0;
  if (argc >= 4)
    output = argv[3];

  scribo::text::recognition(lines, lang);

  scribo::io::text_boxes::save(lines, output);

}
