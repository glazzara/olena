// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_PRIMITIVE_EXTRACT_LINES_PATTERN_HH
# define SCRIBO_PRIMITIVE_EXTRACT_LINES_PATTERN_HH

/// \file
///
/// Extract lines matching a specific pattern.
///
/// \todo Make a more generic canvas.

# include <mln/core/concept/image.hh>
# include <mln/core/alias/window2d.hh>
# include <mln/core/routine/duplicate.hh>

# include <mln/accu/transform_line.hh>
# include <mln/accu/count_value.hh>

# include <mln/data/paste.hh>


namespace scribo
{

  namespace primitive
  {

    namespace extract
    {

      using namespace mln;


      /// Extract lines with a specific pattern
      ///
      /// \param[in] input_ A binary image.
      /// \param[in] length The minimum line length.
      /// \param[in] dir    The direction of the lines.
      /// \param[in] win_   A window corresponding to the line pattern.
      ///
      /// \return A image with lines of direction \p dir.
      ///
      //
      template <typename I, typename W>
      mln_concrete(I)
      lines_pattern(const Image<I>& input_, unsigned length,
		    unsigned dir, const Window<W>& win_);


# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename W>
      mln_concrete(I)
      lines_pattern(const Image<I>& input_, unsigned length,
		    unsigned dir, const Window<W>& win_)
      {
	trace::entering("mln::lines_pattern");

	const I& input = exact(input_);
	const W& win = exact(win_);
	mlc_is(mln_value(I), bool)::check();
	mln_precondition(input.is_valid());

	accu::count_value<bool> accu(true);
	mln_ch_value(I,unsigned)
	  tmp = accu::transform_line(accu, input, length, dir);

	mln_concrete(I) output;
	initialize(output, input);

	mln_piter(I) p(input.domain());
	mln_qiter(window2d) q(win, p);
	bool is_foreground;
	for_all(p)
	{

	  // If the foreground part of the pattern has more than 20%
	  // of background pixels, the current pixel is considered as
	  // background pixel.
	  if (length - tmp(p) > (0.2f * length))
	  {
	    output(p) = false;
	    continue;
	  }

	  // If the background parts of the pattern have less than 95%
	  // of foreground pixels, the current pixel is considered as
	  // part of the background.
	  is_foreground = true;
	  for_all(q)
	    if ((length - tmp(q)) < (length * 0.95f))
	    {
	      is_foreground = false;
	      break;
	    }

	  output(p) = is_foreground;
	}


	trace::exiting("mln::lines_pattern");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_EXTRACT_LINES_PATTERN_HH
