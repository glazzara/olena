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
#include <mln/io/pbm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/draw/box_plain.hh>

#include <mln/debug/filename.hh>

#include <mln/util/timer.hh>

#include <scribo/core/def/lbl_type.hh>
#include <scribo/debug/usage.hh>

#include <scribo/core/component_set.hh>
#include <scribo/core/paragraph_set.hh>
#include <scribo/core/line_info.hh>

#include <scribo/text/link_lines.hh>
#include <scribo/filter/line_links_x_height.hh>

#include <scribo/io/xml/load.hh>

// int i = 0;

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
				"Show paragraph blocks",
				"lines.xml out_blocks.pbm",
				args_desc);

  trace::entering("main");

  typedef image2d<scribo::def::lbl_type> L;
  document<L> doc;
  scribo::io::xml::load(doc, argv[1]);

  if (! doc.has_text())
  {
    std::cout << "ERROR: this XML file does not contain any text information!"
	      << std::endl;
    return 1;
  }


  // Link text lines
  line_links<L> llinks = scribo::text::link_lines(doc.lines());
  llinks = scribo::filter::line_links_x_height(llinks);

  scribo::paragraph_set<L> parset = scribo::make::paragraph(llinks);
  doc.set_paragraphs(parset);

  util::timer t;
  t.start();

  image2d<bool> blocks;
  initialize(blocks, doc.lines().components().labeled_image());
  data::fill(blocks, false);

  // image2d<value::int_u8> log;
  // initialize(log, blocks);
  // data::fill(log, 0);

  for_all_paragraphs(p, parset)
    if (parset(p).nlines() >= 3)
    {
      box2d last_tbox, last_box;

      // For each line in this paragraph.
      for_all_elements(l, parset(p).line_ids())
      {
	const line_info<L>& line = parset.lines()(parset(p).line_ids()(l));

	if (last_box.is_valid())
	  if (last_box.pmax().row() < line.bbox().pmin().row())
	  {
	    last_tbox = last_box;

	    point2d
	      pmin(std::min(last_tbox.pmax().row(), line.bbox().pmin().row()),
	    	   std::max(last_tbox.pmin().col(), line.bbox().pmin().col())),
	      pmax(std::max(last_tbox.pmax().row(), line.bbox().pmin().row()),
	    	   std::min(last_tbox.pmax().col(), line.bbox().pmax().col()));

	    // invalid case:
	    //
	    //         =======
	    //  ======

	    if (pmax.col() > pmin.col())
	    {
	      box2d new_box(pmin, pmax);
	      mln::draw::box_plain(blocks, new_box, true);
	      // mln::draw::box_plain(log, new_box, 1);

	      // mln::io::pgm::save(log, mln::debug::filename("log.pgm", i++));
	    }
	  }
	  else  // Handle the case when there are several text boxes on the same line.
	  {
	    if (last_tbox.is_valid() && last_tbox.pmax().row() < line.bbox().pmin().row())
	    {
	      // Top box
	      point2d
		pmin(std::min(last_tbox.pmax().row(), line.bbox().pmin().row()),
		     std::max(last_tbox.pmin().col(), line.bbox().pmin().col())),
		pmax(std::max(last_tbox.pmax().row(), line.bbox().pmin().row()),
		     std::min(last_tbox.pmax().col(), line.bbox().pmax().col()));

	      box2d new_box(pmin, pmax);

	      mln::draw::box_plain(blocks, new_box, true);
	      // mln::draw::box_plain(log, new_box, 2);
	      // mln::io::pgm::save(log, mln::debug::filename("log.pgm", i++));
	    }

	    if (last_box.pmax().col() < line.bbox().pmax().col()
		&& last_box.pmin().col() < line.bbox().pmin().col())
	    {
	      // Left box
	      point2d
		pmin(std::min(last_tbox.pmax().row(), line.bbox().pmin().row()),
		     std::min(last_box.pmax().col(), line.bbox().pmin().col())),
		pmax(std::min(last_box.pmax().row(), line.bbox().pmax().row()),
		     std::max(last_box.pmax().col(), line.bbox().pmin().col()));

	      box2d new_box(pmin, pmax);
	      mln::draw::box_plain(blocks, new_box, true);
	      // mln::draw::box_plain(log, new_box, 3);
	      // mln::io::pgm::save(log, mln::debug::filename("log.pgm", i++));
	    }
	  }

	mln::draw::box_plain(blocks, line.bbox(), true);
	// mln::draw::box_plain(log, line.bbox(), 255);
	// mln::io::pgm::save(log, mln::debug::filename("log.pgm", i++));
	last_box = line.bbox();
      }
    }

  t.stop();
  std::cout << t << std::endl;

  mln::io::pbm::save(blocks, argv[2]);
  // mln::io::pgm::save(log, "log.pgm");

  trace::exiting("main");
}
