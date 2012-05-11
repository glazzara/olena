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

#ifndef SCRIBO_DRAW_LINE_COMPONENTS_HH
# define SCRIBO_DRAW_LINE_COMPONENTS_HH

/// \file
///
/// Draw the components of a line
# include <mln/util/object_id.hh>

# include <scribo/core/line_set.hh>
# include <scribo/core/component_set.hh>


namespace scribo
{
  namespace draw
  {

    using namespace mln;

    template <typename L, typename I>
    void
    line_components(Image<I>& input_,
		    const line_set<L>& lines,
		    const line_info<L>& line,
		    const mln_value(I)& value);


# ifndef MLN_INCLUDE_ONLY


    template <typename L, typename I>
    void
    line_components(Image<I>& input_,
		    const line_set<L>& lines,
		    const line_info<L>& line,
		    const mln_value(I)& value)
    {
      trace::entering("scribo::draw::line_components");

      I& input = exact(input_);

      mln_precondition(input.is_valid());

      const component_set<L>& comp_set = lines.components();
      const L& labeled_image = lines.components().labeled_image();

      const mln::util::array<component_id_t>& component_ids = line.component_ids();

      for_all_elements(i, component_ids)
      {
	const unsigned c = component_ids(i);
	const mln_value(L) v = c;
	const box2d& bbox = comp_set(c).bbox();

	const unsigned index = labeled_image.offset_of_point(bbox.pmin());
	const unsigned index2 = input.offset_of_point(bbox.pmin());
	const mln_value(L)* ptr_in = &(labeled_image.element(index));
	mln_value(I)* ptr_out = &(input.element(index2));
	const unsigned nrows = bbox.height();
	const unsigned ncols = bbox.width();
	const int delta = labeled_image.delta_offset(dpoint2d(+1, -ncols));
	const int delta2 = input.delta_offset(dpoint2d(+1, -ncols));

	for (unsigned k = 0; k < nrows; ++k)
	{
	  for (unsigned j = 0; j < ncols; ++j)
	  {
	    if (*ptr_in == v)
	      *ptr_out = value;
	    ++ptr_out;
	    ++ptr_in;
	  }

	  ptr_out += delta2;
	  ptr_in += delta;
	}
      }

      trace::exiting("scribo::draw::line_components");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::draw
} // end of namespace scribo

#endif // ! SCRIBO_DRAW_LINE_COMPONENTS_HH
