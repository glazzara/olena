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

#ifndef SCRIBO_DEBUG_MEAN_AND_BASE_LINES_IMAGE_HH
# define SCRIBO_DEBUG_MEAN_AND_BASE_LINES_IMAGE_HH

/// \file
///
/// \brief Compute a color image showing the mean and the base lines
/// of the text lines.

# include <mln/core/concept/image.hh>
# include <mln/data/convert.hh>
# include <mln/value/rgb8.hh>
# include <mln/literal/colors.hh>

# include <mln/draw/box.hh>
# include <mln/draw/line.hh>
# include <mln/draw/dashed_line.hh>

# include <scribo/core/line_set.hh>


namespace scribo
{

  namespace debug
  {

    using namespace mln;


    /// \brief Compute a color image showing the mean and the base lines
    /// of the text lines.
    ///
    /// The mean line is drawn with a dashed line.
    /// The base line is drawn with a plain line.
    ///
    /// \param[in] input An image convertible towards a color image.
    /// \param[in] lines A line set.
    /// \param[in] bbox_value Value used to draw lines bounding boxes.
    /// \param[in] meanline_value Value used to draw mean lines.
    /// \param[in] baseline_value Value used to draw base lines.
    ///
    /// \return A color image.
    ///
    /// \ingroup grpalgodebug
    //
    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    mean_and_base_lines_image(const Image<I>& input,
			      const line_set<L>& lines,
			      const value::rgb8& bbox_value,
			      const value::rgb8& meanline_value,
			      const value::rgb8& baseline_value);

    /// \overload
    /// text_value is set to literal::green.
    /// non_text_value is set to literal::red.
    ///
    /// \ingroup grpalgodebug
    //
    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    mean_and_base_lines_image(const Image<I>& input,
			      const line_set<L>& lines);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    mean_and_base_lines_image(const Image<I>& input,
			      const line_set<L>& lines,
			      const value::rgb8& bbox_value,
			      const value::rgb8& meanline_value,
			      const value::rgb8& baseline_value)
    {
      trace::entering("scribo::debug::mean_and_base_lines_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I, value::rgb8)
	output = data::convert(value::rgb8(), input);

      for_all_lines(l, lines)
      {
	if (! lines(l).is_hidden())
	{
	  mln::draw::box(output, lines(l).bbox(), bbox_value);

	  if (! (lines(l).type() == line::Text
		|| (lines(l).type() == line::Undefined
		    && lines(l).tag() == line::None
		    && lines(l).card() > 2)))
	    continue;

	  point2d
	    b_top(lines(l).meanline(), lines(l).bbox().pmin().col()),
	    e_top(lines(l).meanline(), lines(l).bbox().pmax().col()),
	    b_bot(lines(l).baseline(), lines(l).bbox().pmin().col()),
	    e_bot(lines(l).baseline(), lines(l).bbox().pmax().col());

	  mln::draw::line(output, b_bot, e_bot, baseline_value);
	  mln::draw::dashed_line(output, b_top, e_top, meanline_value);

	}
      }

      trace::exiting("scribo::debug::mean_and_base_lines_image");
      return output;
    }


    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    mean_and_base_lines_image(const Image<I>& input,
			      const line_set<L>& lines)
    {
      return mean_and_base_lines_image(input, lines,
				       literal::purple, literal::blue,
				       literal::cyan);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_MEAN_AND_BASE_LINES_IMAGE_HH
