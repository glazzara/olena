// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_DRAW_GROUPS_BBOXES_HH
# define SCRIBO_DRAW_GROUPS_BBOXES_HH

/// \file
///
/// Draw a list of bounding boxes

# include <mln/core/concept/image.hh>
# include <mln/draw/box.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/object_groups.hh>
# include <scribo/primitive/group/apply.hh>

namespace scribo
{

  namespace draw
  {

    using namespace mln;

    /// Draw a list of bounding boxes.
    template <typename I, typename L>
    inline
    void
    groups_bboxes(Image<I>& input_,
		  const object_groups<L>& groups,
		  const mln_value(I)& value);



# ifndef MLN_INCLUDE_ONLY



    template <typename I, typename L>
    inline
    void
    groups_bboxes(Image<I>& input_,
		  const object_groups<L>& groups,
		  const mln_value(I)& value)
    {
      mln_trace("scribo::draw::groups_bboxes");

      I& input = exact(input_);

      mln_precondition(input.is_valid());

      for_all_groups(g, groups)
	if (groups(g).is_valid())
	  mln::draw::box(input, groups(g).bbox(), value);

    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::draw

} // end of namespace scribo


#endif // ! SCRIBO_DRAW_GROUPS_BBOXES_HH
