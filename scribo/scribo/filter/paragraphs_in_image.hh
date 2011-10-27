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

#ifndef SCRIBO_FILTER_PARAGRAPHS_IN_IMAGE_HH
# define SCRIBO_FILTER_PARAGRAPHS_IN_IMAGE_HH

/// \file
///
/// \brief Remove invalid paragraphs.
/// \todo Share same test canvas as text::merging.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/function.hh>
# include <mln/core/image/dmorph/image_if.hh>

# include <mln/data/transform.hh>
# include <mln/draw/box_plain.hh>

# include <mln/util/array.hh>

# include <mln/pw/all.hh>

# include <scribo/core/document.hh>
# include <scribo/core/paragraph_set.hh>
# include <scribo/fun/v2b/objects_small_filter.hh>
# include <scribo/primitive/extract/components.hh>


namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /// \brief Remove invalid paragraphs.
    ///
    /// \param[in,out] doc    A document structure.
    ///
    /// \return A paragraph set with invalid paragraphs tag set to
    /// Paragraph::Ignored.
    ///
    /// \ingroup grpalgofilterpar
    //
    template <typename L>
    void
    paragraphs_in_image(document<L>& doc);


# ifndef MLN_INCLUDE_ONLY

    template <typename L>
    void
    paragraphs_in_image(document<L>& doc)
    {
      trace::entering("scribo::filter::paragraphs_in_image");

      mln_precondition(doc.has_elements());
      mln_precondition(doc.has_text());

      image2d<bool> billboard;
      initialize(billboard, doc.lines().components().labeled_image());
      data::fill(billboard, false);

      // Draw Image bboxes.
      for_all_elements(e, doc.elements())
	if (doc.elements()(e).is_valid()
	    && doc.elements()(e).type() == component::Image)
	  mln::draw::box_plain(billboard, doc.elements()(e).bbox(), true);

      const paragraph_set<L>& parset = doc.paragraphs();
      mln::util::array<bool> not_to_ignore(parset.nelements() + 1, true);
      not_to_ignore(0) = false;

      for_all_paragraphs(cur_id, parset)
      {
	const box2d& b_ = parset(cur_id).bbox();
	const bool
	  tl = billboard(b_.pmin()),
	  tr = billboard.at_(b_.pmin().row(), b_.pmax().col()),
	  mc = billboard.at_(b_.pcenter().row(), b_.pcenter().col()),
	  bl = billboard.at_(b_.pmax().row(), b_.pmin().col()),
	  br = billboard(b_.pmax());

	typedef mln::util::set<int> set_t;
	set_t s;
	s.insert(tl);
	s.insert(tr);
	s.insert(mc);
	s.insert(bl);
	s.insert(br);

	if (s.nelements() > 2 || (s.nelements() == 2 && !s.has(0)))
	  continue;

	// The paragraph is fully included in an image.
	for_all_elements(e, s)
	  if (s[e] != 0
	      && (mc != 0 && mc == s[e]
		  && ((tl == mc && bl == mc)
		      || (tr == mc && br == mc)
		      || (tl == mc && tr == mc)
		      || (bl == mc && br == mc))))
	  {
//	if (tl && tr && ml && mc && mr && bl && br)
	    not_to_ignore(cur_id) = false;
	    break;
	  }
      }

      paragraph_set<L> output = parset.duplicate();
      output.invalidate(not_to_ignore);
      doc.set_paragraphs(output);

      trace::exiting("scribo::filter::paragraphs_in_image");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_PARAGRAPHS_IN_IMAGE_HH
