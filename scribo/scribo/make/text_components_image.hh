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

#ifndef SCRIBO_MAKE_TEXT_COMPONENTS_IMAGE_HH
# define SCRIBO_MAKE_TEXT_COMPONENTS_IMAGE_HH

/// \file
///
/// Create a binary image with text components only.

#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/util/array.hh>
#include <mln/data/fill.hh>

#include <scribo/core/document.hh>
#include <scribo/core/line_set.hh>


namespace scribo
{

  namespace make
  {
    using namespace mln;


    /// \brief Create a binary image with text components only.
    /*!
      \pre \p doc has_text() methods MUST return True.
     */
    template <typename L>
    mln_ch_value(L,bool)
    text_components_image(const document<L>& doc);


# ifndef MLN_INCLUDE_ONLY

    template <typename L>
    mln_ch_value(L,bool)
    text_components_image(const document<L>& doc)
    {
      trace::entering("scribo::make::text_components_image");

      mln_precondition(doc.is_open());
      mln_precondition(doc.has_text());

      mln_ch_value(L,bool) output;
      initialize(output, doc.image());
      data::fill(output, false);

      if (doc.has_text())
      {
	const scribo::line_set<L>& lines = doc.lines();
	for_all_lines(l, doc.lines())
	  if (lines(l).is_textline())
	  {
	    const util::array<component_id_t>&
	      comp_ids = lines(l).component_ids();
	    const L& lbl = lines.components().labeled_image();
	    for_all_elements(c, comp_ids)
	    {
	      data::fill(((output | lines.components()(comp_ids(c)).bbox()).rw()
			  | (pw::value(lbl) == comp_ids(c))).rw(),
			 (doc.binary_image() | lines.components()(comp_ids(c)).bbox()));
	    }
	  }
      }

      trace::exiting("scribo::make::text_components_image");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::make

} // end of namespace scribo



#endif // ! SCRIBO_MAKE_TEXT_COMPONENTS_IMAGE_HH
