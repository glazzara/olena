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

#ifndef SCRIBO_PRIMITIVE_EXTRACT_LINES_V_PATTERN_HH
# define SCRIBO_PRIMITIVE_EXTRACT_LINES_V_PATTERN_HH

/// \file
///
/// Extract vertical lines matching a specific pattern.

#include <mln/core/concept/image.hh>
#include <mln/core/alias/window2d.hh>

# include <mln/win/hline2d.hh>

# include <mln/morpho/dilation.hh>

# include <scribo/primitive/extract/lines_pattern.hh>

namespace scribo
{

  namespace primitive
  {

    namespace extract
    {

      using namespace mln;


      /// Extract vertical lines matching a specific pattern.
      ///
      /// \param[in] input  A binary image.
      /// \param[in] length The minimum line length.
      ///
      /// \result An image of vertical lines.
      //
      template <typename I>
      mln_concrete(I)
      lines_v_pattern(const Image<I>& input, unsigned length, unsigned delta);


# ifndef MLN_INCLUDE_ONLY


      template <typename I>
      mln_concrete(I)
      lines_v_pattern(const Image<I>& input, unsigned length, unsigned delta)
      {
	mln_precondition(length % 2 == 1);

// 	bool win_def[1][7] = { { 1, 0, 0, 0, 0, 0, 1 } };

// 	window2d win;
// 	convert::from_to(win_def, win);

	// FIXME: not generic.
 	window2d win;
	mln_deduce(I, site, dpsite)
	  dp1(0, -delta),
	  dp2(0,  delta);
	win.insert(dp1);
	win.insert(dp2);

	//FIXME: Add reconstruction instead of this arbitrary dilation.
 	win::vline2d vwin(length/2 + 2);
// 	win::vline2d vwin(length);
	return morpho::dilation(lines_pattern(input, length, 0, win), vwin);
//	return lines_pattern(input, length, 0, win);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_EXTRACT_LINES_V_PATTERN_HH

