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

#ifndef SCRIBO_DEBUG_TEXT_COLOR_IMAGE_HH
# define SCRIBO_DEBUG_TEXT_COLOR_IMAGE_HH

/// \file
///
/// Draw text components with their respective colors.

# include <mln/core/image/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/data/fill.hh>
# include <mln/literal/white.hh>
# include <mln/util/array.hh>

# include <mln/pw/all.hh>
# include <mln/core/image/dmorph/image_if.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/document.hh>

namespace scribo
{

  namespace debug
  {
    using namespace mln;

    template <typename L>
    image2d<value::rgb8>
    text_color_image(const document<L>& doc);


# ifndef MLN_INCLUDE_ONLY

    template <typename L>
    image2d<mln::value::rgb8>
    text_color_image(const document<L>& doc)
    {
      image2d<value::rgb8> debug;
      initialize(debug, doc.binary_image());
      data::fill(debug, literal::white);

      const paragraph_set<L>& parset = doc.paragraphs();
      const line_set<L>& lines = doc.lines();
      const component_set<L>& comp_set = lines.components();
      const L& lbl = comp_set.labeled_image();

      for_all_paragraphs(p, parset)
      {
	const mln::util::array<line_id_t>& line_ids = parset(p).line_ids();

	for_all_paragraph_lines(lid, line_ids)
	{
	  line_id_t l = line_ids(lid);
	  const mln::util::array<component_id_t>& comps = lines(l).component_ids();

	  for_all_elements(e, comps)
	  {
	    unsigned comp_id = comps(e);
	    data::fill(((debug | comp_set(comp_id).bbox()).rw() | (pw::value(lbl) == pw::cst(comp_id))).rw(),
		       parset(p).color());
	  }

	}
      }

      return debug;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo

#endif // ! SCRIBO_DEBUG_TEXT_COLOR_IMAGE_HH
