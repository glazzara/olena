// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_FILTER_OBJECTS_IN_BORDERS_HH
# define SCRIBO_FILTER_OBJECTS_IN_BORDERS_HH

/// \file
///
/// \brief Invalidate false positive separators.
/// \todo Share same test canvas as text::merging.


# include <mln/core/concept/image.hh>
# include <mln/geom/all.hh>
# include <scribo/core/component_set.hh>
# include <scribo/util/box_is_included.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /*! \brief Invalidate components located close to the image
      borders.

       \param[in,out] components A component set.
       \param[in] vratio Ratio to be used for evaluating the inner
                         border size in which vertical separators
                         will be invalidated.
       \param[in] hratio Ratio to be used for evaluating the inner
                         border size in which horizontal separators
                         will be invalidated.

       \warning It only invalidates components in the given
       component_set.

       \verbatim

        -----------
        |_!____!__|
        | !    ! <--------- Components located in this area are
        | !    !  |         invalidated.
        | !    !  |
        |_!____!__|
        | !    !  |
        -----------

	\endverbatim

       \ingroup grpalgofilterelt
    */
    template <typename L>
    void
    objects_in_borders(component_set<L>& components, float vratio, float hratio);


# ifndef MLN_INCLUDE_ONLY

    template <typename L>
    void
    objects_in_borders(component_set<L>& components, float vratio, float hratio)
    {
      mln_trace("scribo::filter::objects_in_borders");

      mln_precondition(components.is_valid());

      const L& ima = components.labeled_image();

      unsigned
	hborder_size = hratio * std::min(ima.domain().width(), ima.domain().height()),
	vborder_size = vratio * std::min(ima.domain().width(), ima.domain().height());

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
	pt(geom::min_row(ima), geom::max_col(ima) - vborder_size),
	ptr(hborder_size, geom::max_col(ima)),
	pbr = ima.domain().pmax(),
	pb(geom::max_row(ima), vborder_size),
	pbl(geom::max_row(ima) - hborder_size, geom::min_col(ima));

      box2d
	bt(ptl, ptr),
	br(pt, pbr),
	bb(pbl, pbr),
	bl(ptl, pb);

      for_all_comps(c, components)
	if (components(c).is_valid())
	  if (util::box_is_included(components(c).bbox(), bt)
	      || util::box_is_included(components(c).bbox(), br)
	      || util::box_is_included(components(c).bbox(), bb)
	      || util::box_is_included(components(c).bbox(), bl))
	  {
	    components(c).update_tag(component::Ignored);
	  }

    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_OBJECTS_IN_BORDERS_HH
