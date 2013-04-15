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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/value/int_u16.hh>
#include <mln/draw/box_plain.hh>
#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/data/convert.hh>
#include <mln/literal/colors.hh>

#include <scribo/debug/usage.hh>

#include <scribo/core/document.hh>
#include <scribo/core/component_set.hh>
#include <scribo/core/object_links.hh>
#include <scribo/core/object_groups.hh>

#include <scribo/text/extract_lines.hh>

#include <scribo/io/xml/save.hh>


const char *args_desc[][2] =
{
  {0, 0}
};



int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 7)
    return scribo::debug::usage(argv,
				"Show text lines",
				"input.pbm input_seps.pbm out_text_boxes.ppm out_text_boxes.pbm out_text_comps.pbm out_lines.xml",
				args_desc);

  mln_trace("main");

  image2d<bool> input;
  mln::io::pbm::load(input, argv[1]);

  image2d<bool> input_seps;
  mln::io::pbm::load(input_seps, argv[2]);

  mln::util::timer t;
  t.start();

  typedef image2d<scribo::def::lbl_type> L;
  line_set<L> lines = text::extract_lines(input, c8(), input_seps);

  t.stop();
  std::cout << t << "s" << std::endl;

  // Color image with line bboxes.
  {
    image2d<value::rgb8> output = data::convert(value::rgb8(), input);
    for_all_lines(l, lines)
      if (lines(l).is_textline())
	mln::draw::box(output, lines(l).bbox(), literal::red);
    mln::io::ppm::save(output, argv[3]);
  }

  // Binary image with line bboxes.
  {
    image2d<bool> output;
    initialize(output, input);
    data::fill(output, false);
    for_all_lines(l, lines)
      if (lines(l).is_textline())
	mln::draw::box_plain(output, lines(l).bbox(), true);
    mln::io::pbm::save(output, argv[4]);
  }

  {
    image2d<bool> output;
    initialize(output, input);
    data::fill(output, false);
    for_all_lines(l, lines)
      if (lines(l).is_textline())
      {
	const mln::util::array<component_id_t>& comps = lines(l).component_ids();
	const L& lbl = lines.components().labeled_image();
	for_all_elements(c, comps)
	  data::fill((output | lines.components()(comps(c)).bbox()).rw(),
		     ((input | lines.components()(comps(c)).bbox())  | (pw::value(lbl) == comps(c))));
      }
    mln::io::pbm::save(output, argv[5]);
  }

  // Saving stoppers data to XML
  document<L> doc(argv[1]);
  doc.open();
  doc.set_paragraphs(scribo::make::paragraph(lines));
  scribo::io::xml::save(doc, argv[6], scribo::io::xml::Full);


}
