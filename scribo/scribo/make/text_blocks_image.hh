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

#ifndef SCRIBO_MAKE_TEXT_BLOCKS_IMAGE_HH
# define SCRIBO_MAKE_TEXT_BLOCKS_IMAGE_HH

/// \file
///
/// \brief Create a mask of paragraph blocks.

# include <mln/core/image/image2d.hh>
# include <mln/draw/box_plain.hh>

# include <scribo/core/document.hh>
# include <scribo/core/paragraph_set.hh>
# include <scribo/core/line_info.hh>


namespace scribo
{

  namespace make
  {
    using namespace mln;


    /// \brief Create a mask of paragraph blocks.
    /*!
      \pre \p doc has_text() methods MUST return True.
     */
    template <typename L>
    mln_ch_value(L,bool)
    text_blocks_image(const document<L>& doc, unsigned min_nlines);


 # ifndef MLN_INCLUDE_ONLY

    template <typename L>
    mln_ch_value(L,bool)
      text_blocks_image(const document<L>& doc, unsigned min_nlines)
    {
      mln_trace("scribo::make::text_blocks_image");

      mln_precondition(doc.is_open());

      image2d<bool> output;
      initialize(output, doc.image());
      data::fill(output, false);

      // image2d<value::int_u8> log;
      // initialize(log, output);
      // data::fill(log, 0);

      if (doc.has_text())
      {
	const paragraph_set<L>& parset = doc.paragraphs();
	for_all_paragraphs(p, parset)
	  if (parset(p).nlines() >= min_nlines)
	  {
	    box2d last_box;

	    // For each line in this paragraph.
	    for_all_elements(l, parset(p).line_ids())
	    {
	      const line_info<L>&
		line = parset.lines()(parset(p).line_ids()(l));

	      // Avoid invalid case:
	      //
	      //         =======
	      //  ======
	      if (last_box.is_valid()
		  && last_box.pmax().row() < line.bbox().pmin().row()
		  && last_box.pmin().col() < line.bbox().pmax().col())
	      {
		point2d
		  pmin(std::min(last_box.pmax().row(),
				line.bbox().pmin().row()),
		       std::max(last_box.pmin().col(),
				line.bbox().pmin().col())),
		  pmax(std::max(last_box.pmax().row(),
				line.bbox().pmin().row()),
		       std::min(last_box.pmax().col(),
				line.bbox().pmax().col()));

		box2d new_box(pmin, pmax);
		mln::draw::box_plain(output, new_box, true);
		// mln::draw::box_plain(log, new_box, 1);

		// mln::io::pgm::save(log, mln::debug::filename("log.pgm", i++));
	      }

	      mln::draw::box_plain(output, line.bbox(), true);
	      // mln::draw::box_plain(log, line.bbox(), 255);
	      // mln::io::pgm::save(log, mln::debug::filename("log.pgm", i++));
	      last_box = line.bbox();
	    }
	  }
      }


      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::make

} // end of namespace scribo



#endif // ! SCRIBO_MAKE_TEXT_BLOCKS_IMAGE_HH
