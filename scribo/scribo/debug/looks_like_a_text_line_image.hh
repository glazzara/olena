// Copyright (C) 2010, 2011, 2013 EPITA Research and Development
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

#ifndef SCRIBO_DEBUG_LOOKS_LIKE_A_TEXT_LINE_IMAGE_HH
# define SCRIBO_DEBUG_LOOKS_LIKE_A_TEXT_LINE_IMAGE_HH

/// \file
///
/// \brief Compute an image where components are drawn differently
/// whether they look like a line or not.


# include <mln/core/concept/image.hh>
# include <mln/data/convert.hh>
# include <mln/value/rgb8.hh>
# include <mln/literal/colors.hh>
# include <mln/draw/box.hh>

# include <scribo/core/line_set.hh>

# include <scribo/text/look_like_text_lines.hh>


namespace scribo
{

  namespace debug
  {

    using namespace mln;


    /// \brief Compute an image where components are drawn differently
    /// whether they look like a line or not.
    ///
    /// \param[in] input An image convertible towards a color image.
    /// \param[in] lines A line set.
    /// \param[in] text_value The color used to draw bounding boxes
    ///                       of components looking like a text line.
    /// \param[in] non_text_value The color used to draw bounding
    ///                           boxes of components NOT looking like
    ///                           a text line.
    ///
    /// \return A color image.
    ///
    /// \ingroup grpalgodebug
    //
    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    looks_like_a_text_line_image(const Image<I>& input,
				 const line_set<L>& lines,
				 const value::rgb8& text_value,
				 const value::rgb8& non_text_value);

    /// \overload
    /// text_value is set to literal::green.
    /// non_text_value is set to literal::red.
    ///
    /// \ingroup grpalgodebug
    //
    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    looks_like_a_text_line_image(const Image<I>& input,
				 const line_set<L>& lines);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    looks_like_a_text_line_image(const Image<I>& input,
				 const line_set<L>& lines,
				 const value::rgb8& text_value,
				 const value::rgb8& non_text_value)
    {
      mln_trace("scribo::debug::looks_like_a_text_line_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) output = data::convert(value::rgb8(), input);

      for_all_lines(l, lines)
	if (! lines(l).is_hidden())
	{
	  if (text::internal::looks_like_a_text_line(lines(l)))
	    mln::draw::box(output, lines(l).bbox(), text_value);
	  else
	    mln::draw::box(output, lines(l).bbox(), non_text_value);
	}

      return output;
    }


    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    looks_like_a_text_line_image(const Image<I>& input,
				 const line_set<L>& lines)
    {
      return looks_like_a_text_line_image(input, lines,
					  literal::green, literal::red);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_LOOKS_LIKE_A_TEXT_LINE_IMAGE_HH
