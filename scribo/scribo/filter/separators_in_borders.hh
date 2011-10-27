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

#ifndef SCRIBO_FILTER_SEPARATORS_IN_BORDERS_HH
# define SCRIBO_FILTER_SEPARATORS_IN_BORDERS_HH

/// \file
///
/// \brief Invalidate false positive separators.
/// \todo Share same test canvas as text::merging.


# include <mln/core/concept/image.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/document.hh>
# include <scribo/util/box_is_included.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /// \brief Invalidate separators located close to the image
    /// borders.
    ///
    /// \param[in,out] doc A document structure.
    /// \param[in] vratio Ratio to be used for evaluating the inner
    ///                   border size in which vertical separators
    ///                   will be invalidated.
    /// \param[in] hratio Ratio to be used for evaluating the inner
    ///                   border size in which horizontal separators
    ///                   will be invalidated.
    ///
    /// Warning: it does not remove separators from separator
    /// image. It only invalidate separator components in their
    /// respective component_set.
    ///
    /// \verbatim
    ///
    ///  -----------
    ///  |_!____!__|
    ///  | !    ! <--------- Separators located in this area are
    ///  | !    !  |         invalidated.
    ///  | !    !  |
    ///  |_!____!__|
    ///  | !    !  |
    ///  -----------
    ///
    /// \endverbatim
    ///
    /// \ingroup grpalgofilterelt
    //
    template <typename L>
    void
    separators_in_borders(document<L>& doc, float vratio, float hratio);


# ifndef MLN_INCLUDE_ONLY

    template <typename L>
    void
    separators_in_borders(document<L>& doc, float vratio, float hratio)
    {
      trace::entering("scribo::filter::separators_in_borders");

      mln_precondition(doc.is_valid());

      const mln::image2d<mln::value::rgb8>& ima = doc.image();

      // Horizontal separators
      if (doc.has_hline_seps())
      {
	unsigned border_size = hratio * std::min(ima.domain().width(), ima.domain().height());

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


	component_set<L> hline = doc.hline_seps_comps().duplicate();
	for_all_comps(c, hline)
	  if (hline(c).is_valid())
	    if (util::box_is_included(hline(c).bbox(), bt)
		|| util::box_is_included(hline(c).bbox(), br)
		|| util::box_is_included(hline(c).bbox(), bb)
		|| util::box_is_included(hline(c).bbox(), bl))
	    {
	      hline(c).update_tag(component::Ignored);
	    }

	// FIXME: warning this call may produce inconsistent data
	// Ignored components are still in the separator image...
	doc.set_hline_separators(doc.hline_seps(), hline);
      }


      // Vertical separators
      if (doc.has_vline_seps())
      {
	unsigned border_size = vratio * std::min(ima.domain().width(), ima.domain().height());

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


	component_set<L> vline = doc.vline_seps_comps().duplicate();
	for_all_comps(c, vline)
	  if (vline(c).is_valid())
	  {
	    if (util::box_is_included(vline(c).bbox(), bt)
		|| util::box_is_included(vline(c).bbox(), br)
		|| util::box_is_included(vline(c).bbox(), bb)
		|| util::box_is_included(vline(c).bbox(), bl))
	    {
	      // std::cout << vline(c).bbox() << " is included in ";
	      // if (util::box_is_included(vline(c).bbox(), bt))
	      // 	std::cout << bt << std::endl;
	      // if (util::box_is_included(vline(c).bbox(), br))
	      // 	std::cout << br << std::endl;
	      // if (util::box_is_included(vline(c).bbox(), bb))
	      // 	std::cout << bb << std::endl;
	      // if (util::box_is_included(vline(c).bbox(), bl))
	      // 	std::cout << bl << std::endl;

	      vline(c).update_tag(component::Ignored);
	    }
	    // else
	    // {
	    //   std::cout << vline(c).bbox() << " is not included in " << bt << " - " << br << " - " << bb << " - " << bl << std::endl;
	    // }
	  }
	// FIXME: warning this call may produce inconsistent data
	// Ignored components are still in the separator image...
	doc.set_vline_separators(doc.vline_seps(), vline);
      }

      trace::exiting("scribo::filter::separators_in_borders");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_SEPARATORS_IN_BORDERS_HH
