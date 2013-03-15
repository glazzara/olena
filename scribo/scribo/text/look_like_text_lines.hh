// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_TEXT_LOOK_LIKE_TEXT_LINES_HH
# define SCRIBO_TEXT_LOOK_LIKE_TEXT_LINES_HH

/// \file
///
/// \brief Set line type to line::Text according to criterion.

# include <scribo/core/line_info.hh>

namespace scribo
{

  namespace text
  {

    using namespace mln;

    /// \brief Set line type to line::Text according to criterion.
    //
    template <typename L>
    line_set<L>
    look_like_text_lines(const scribo::line_set<L>& l);

    /// \overload
    /// Inplace version.
    //
    template <typename L>
    void
    look_like_text_lines_inplace(scribo::line_set<L>& line);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      template <typename L>
      inline
      bool looks_like_a_text_line(const scribo::line_info<L>& l)
      {
	// Special case for two-letter words
	if (l.card() == 2)
	{
	  const float ratio = (float) l.bbox().width() / l.bbox().height();

	  if (// Minimal width / height ratio
	    ratio > 0.4f && ratio < 2.0f
	      // Minimal height
	      && l.bbox().height() >= 15
	      // Characters must have approximately the same width
	      && l.chars_same_width())
	    {
	      return true;
	    }
	 }

	return
	  l.card() >= 3                  // at least 3 components
	  && l.bbox().height() > 10      // and minimal height
	  && l.bbox().width() > l.bbox().height(); // and more horizontal-like than vertical
	// FIXME: Later on, add a criterion based on the number
	// of alignments (on top and bot).
      }

    } // end of namespace scribo::text::namespace



    // Facades

    template <typename L>
    inline
    void
    look_like_text_lines_inplace(scribo::line_set<L>& line)
    {
      mln_trace("scribo::text::look_like_text_lines_inplace");

      for_all_lines(l, line)
	if (internal::looks_like_a_text_line(line(l)))
	    line(l).update_type(line::Text);

    }

    template <typename L>
    inline
    line_set<L>
    look_like_text_lines(const scribo::line_set<L>& l)
    {
      mln_trace("scribo::text::look_like_text_lines");
      line_set<L> output = l.duplicate();

      look_like_text_lines_inplace(output);

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_LOOK_LIKE_TEXT_LINES_HH
