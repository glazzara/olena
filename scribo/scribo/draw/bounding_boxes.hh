// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_DRAW_BOUNDING_BOXES_HH
# define SCRIBO_DRAW_BOUNDING_BOXES_HH

/// \file
///
/// Draw a list of bounding boxes

# include <mln/core/concept/image.hh>
# include <mln/draw/box.hh>
# include <mln/util/array.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/component_set.hh>

namespace scribo
{

  namespace draw
  {

    using namespace mln;

    /// Draw a list of bounding boxes.
    template <typename I>
    void
    bounding_boxes(Image<I>& input_,
		   const mln::util::array< box<mln_site(I)> >& boxes,
		   const mln_value(I)& value);


    /// Draw object bounding boxes.
    template <typename I, typename L>
    void
    bounding_boxes(Image<I>& input_,
		   const component_set<L>& components,
		   const mln_value(I)& value);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    inline
    void
    bounding_boxes(Image<I>& input_,
		   const mln::util::array< box<mln_site(I)> >& boxes,
		   const mln_value(I)& value)
    {
      mln_trace("scribo::draw::bounding_boxes");

      I& input = exact(input_);

      mln_precondition(input.is_valid());

      for_all_comp_data(i, boxes)
        if (boxes[i].is_valid())
	  mln::draw::box(input, boxes[i], value);

    }


    template <typename I, typename L>
    inline
    void
    bounding_boxes(Image<I>& input_,
		   const component_set<L>& components,
		   const mln_value(I)& value)
    {
      mln_trace("scribo::draw::bounding_boxes");

      I& input = exact(input_);

      mln_precondition(input.is_valid());

      for_all_comps(i, components)
	if (components(i).is_valid())
	  mln::draw::box(input, components(i).bbox(), value);

    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::draw

} // end of namespace scribo


#endif // ! SCRIBO_DRAW_BOUNDING_BOXES_HH
