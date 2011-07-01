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

#ifndef SCRIBO_FILTER_PARAGRAPHS_IN_BORDERS_HH
# define SCRIBO_FILTER_PARAGRAPHS_IN_BORDERS_HH

/// \file
///
/// Invalidate false positive paragraphs.
/// \fixme Share same test canvas as text::merging.


# include <mln/core/concept/image.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/document.hh>
# include <scribo/util/box_is_included.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /// Invalidate paragraphs located close to the image borders.
    ///
    /// \param[in,out] doc A document structure.
    ///
    /// Warning: it does not remove paragraphs from separator
    /// image. It only invalidate separator components in their
    /// respective component_set.
    ///
    /// \verbatim
    ///
    ///  -----------
    ///  |_!____!__|
    ///  | !    ! <--------- Paragraphs located in this area are
    ///  | !    !  |         invalidated.
    ///  | !    !  |
    ///  |_!____!__|
    ///  | !    !  |
    ///  -----------
    ///
    /// \endverbatim
    //
    template <typename L>
    void
    paragraphs_in_borders(document<L>& doc);


# ifndef MLN_INCLUDE_ONLY

    template <typename L>
    void
    paragraphs_in_borders(document<L>& doc)
    {
      trace::entering("scribo::filter::paragraphs_in_borders");

      mln_precondition(doc.is_valid());

      const mln::image2d<mln::value::rgb8>& ima = doc.image();

      unsigned border_size = std::min(43., 0.02 * ima.domain().width());

      ///            pt
      /// ptl X------X---
      ///     |_!____!__X ptr
      ///     | !    !  |
      ///     | !    !  |
      ///     | !    !  |
      /// pbl X_!____!__|
      ///     | !    !  |
      ///     --X-------X
      ///       pb       pbr
      ///
      point2d
	ptl = ima.domain().pmin(),
	pt(geom::min_row(ima), geom::max_col(ima) - border_size),
	ptr(border_size, geom::max_col(ima)),
	pbr = ima.domain().pmax(),
	pb(geom::max_row(ima), border_size),
	pbl(geom::max_row(ima) - border_size, geom::min_col(ima));

      box2d
	bt(ptl, ptr),
	br(pt, pbr),
	bb(pbl, pbr),
	bl(ptl, pb);

      // Horizontal paragraphs
      if (doc.has_text())
      {
	paragraph_set<L> parset = doc.paragraphs();
	for_all_paragraphs(p, parset)
	  if (parset(p).is_valid())
	    if (util::box_is_included(parset(p).bbox(), bt)
		|| util::box_is_included(parset(p).bbox(), br)
		|| util::box_is_included(parset(p).bbox(), bb)
		|| util::box_is_included(parset(p).bbox(), bl))
	    {
	      parset(p).invalidate();
	    }

	doc.set_paragraphs(parset);
      }

      trace::exiting("scribo::filter::paragraphs_in_borders");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_PARAGRAPHS_IN_BORDERS_HH
