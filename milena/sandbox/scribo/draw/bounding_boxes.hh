// Copyright (C) 2009 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.


#ifndef SCRIBO_DRAW_BOUNDING_BOXES_HH
# define SCRIBO_DRAW_BOUNDING_BOXES_HH

/// \file scribo/draw/bounding_boxes.hh
///
/// Draw a list of bounding boxes and their associated mass center.

# include <mln/core/concept/image.hh>
# include <mln/draw/box.hh>
# include <mln/util/array.hh>

# include <scribo/core/macros.hh>

namespace scribo
{

  namespace draw
  {

    using namespace mln;

    /// Draw a list of bounding boxes and their associated mass center.
    template <typename I>
    void
    bounding_boxes(Image<I>& input_,
		   const util::array< box<mln_site(I)> >& boxes,
		   const mln_value(I)& value);



# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    inline
    void
    bounding_boxes(Image<I>& input_,
		   const util::array< box<mln_site(I)> >& boxes,
		   const mln_value(I)& value)
    {
      trace::entering("scribo::draw::bounding_boxes");

      I& input = exact(input_);

      mln_precondition(input.is_valid());

      for_all_components(i, boxes)
        if (boxes[i].is_valid())
	{
	  input(boxes[i].center()) = value;
	  mln::draw::box(input, boxes[i], value);
	}

      trace::exiting("scribo::draw::bounding_boxes");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::draw

} // end of namespace scribo


#endif // ! SCRIBO_DRAW_BOUNDING_BOXES_HH
