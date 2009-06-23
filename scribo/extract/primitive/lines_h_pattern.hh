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

#ifndef SCRIBO_EXTRACT_PRIMITIVE_LINES_H_PATTERN_HH
# define SCRIBO_EXTRACT_PRIMITIVE_LINES_H_PATTERN_HH

/// \file
///
/// Extract horizontal lines matching a specific pattern.

#include <mln/core/concept/image.hh>
#include <mln/core/alias/window2d.hh>

# include <mln/win/hline2d.hh>

# include <mln/morpho/dilation.hh>

# include <scribo/extract/primitive/lines_pattern.hh>

namespace scribo
{

  namespace extract
  {

    namespace primitive
    {
      using namespace mln;

      /// Extract horizontal lines matching a specific pattern.
      ///
      /// \param[in] input  A binary image.
      /// \param[in] length The minimum line length.
      ///
      /// \result An image of horizontal lines.
      //
      template <typename I>
      mln_concrete(I)
      lines_h_pattern(const Image<I>& input, unsigned length);


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      mln_concrete(I)
      lines_h_pattern(const Image<I>& input, unsigned length)
      {
	mlc_is(mln_value(I), bool)::check();
	mln_precondition(exact(input).is_valid());
	mln_precondition(length % 2 == 1);

	bool win_def[7][1] = { {1},
			       {0},
			       {0},
			       {0},
			       {0},
			       {0},
			       {1} };

	window2d win;
	convert::from_to(win_def, win);

	win::hline2d hwin(length/2 + 2);
	return morpho::dilation(lines_pattern(input, length, 1, win), hwin);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::impl::generic

  } // end of namespace mln::impl

} // end of namespace mln


#endif // ! SCRIBO_EXTRACT_PRIMITIVE_LINES_H_PATTERN_HH

