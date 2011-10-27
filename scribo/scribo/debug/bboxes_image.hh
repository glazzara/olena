// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef SCRIBO_DEBUG_BBOXES_IMAGE_HH
# define SCRIBO_DEBUG_BBOXES_IMAGE_HH

/// \file
///
/// Draw a list of bounding boxes and their associated mass center.

# include <mln/core/concept/image.hh>
# include <mln/data/convert.hh>
# include <mln/value/rgb8.hh>
# include <mln/util/array.hh>
# include <mln/io/ppm/save.hh>
# include <mln/literal/colors.hh>

# include <scribo/core/line_set.hh>
# include <scribo/draw/bounding_boxes.hh>


namespace scribo
{

  namespace debug
  {

    using namespace mln;


    /// \brief Draw a list of bounding boxes
    ///
    /// \ingroup grpalgodebug
    //
    template <typename I>
    mln_ch_value(I, value::rgb8)
    bboxes_image(const Image<I>& input,
		 const mln::util::array< box<mln_site(I)> >& bboxes,
		 const value::rgb8& value);

    /// \overload
    ///
    /// \ingroup grpalgodebug
    //
    template <typename I, typename L>
    mln_ch_value(I, value::rgb8)
    bboxes_image(const Image<I>& input,
		 const line_set<L>& lines,
		 const value::rgb8& value);

    /// \overload
    /// value is set to literal::red.
    ///
    /// \ingroup grpalgodebug
    //
    template <typename I, typename L>
    inline
    mln_ch_value(I, value::rgb8)
    bboxes_image(const Image<I>& input,
		 const line_set<L>& lines);

    /// \overload
    ///
    /// \ingroup grpalgodebug
    //
    template <typename I, typename L>
    inline
    mln_ch_value(I, value::rgb8)
    bboxes_image(const Image<I>& input,
		 const component_set<L>& comps,
		 const value::rgb8& value);

# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    mln_ch_value(I, value::rgb8)
    bboxes_image(const Image<I>& input,
		 const mln::util::array< box<mln_site(I)> >& bboxes,
		 const value::rgb8& value)
    {
      trace::entering("scribo::debug::bboxes_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) tmp = data::convert(value::rgb8(), input);
      draw::bounding_boxes(tmp, bboxes, value);

      trace::exiting("scribo::debug::bboxes_image");
      return tmp;
    }


    template <typename I, typename L>
    inline
    mln_ch_value(I, value::rgb8)
    bboxes_image(const Image<I>& input,
		 const line_set<L>& lines,
		 const value::rgb8& value)
    {
      trace::entering("scribo::debug::bboxes_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I, value::rgb8)
	output = data::convert(value::rgb8(), input);

      for_all_lines(l, lines)
	if (! lines(l).is_hidden())
	  mln::draw::box(output, lines(l).bbox(), value);

      trace::exiting("scribo::debug::bboxes_image");
      return output;
    }


    template <typename I, typename L>
    inline
    mln_ch_value(I, value::rgb8)
    bboxes_image(const Image<I>& input,
		 const line_set<L>& lines)
    {
      return bboxes_image(input, lines, literal::red);
    }


    template <typename I, typename L>
    inline
    mln_ch_value(I, value::rgb8)
    bboxes_image(const Image<I>& input,
		 const component_set<L>& comps,
		 const value::rgb8& value)
    {
      trace::entering("scribo::debug::bboxes_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I, value::rgb8)
	output = data::convert(value::rgb8(), input);

      for_all_comps(c, comps)
	if (comps(c).is_valid())
	  mln::draw::box(output, comps(c).bbox(), value);

      trace::exiting("scribo::debug::bboxes_image");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_BBOXES_IMAGE_HH
