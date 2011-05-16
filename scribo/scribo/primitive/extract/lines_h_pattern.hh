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

#ifndef SCRIBO_PRIMITIVE_EXTRACT_LINES_H_PATTERN_HH
# define SCRIBO_PRIMITIVE_EXTRACT_LINES_H_PATTERN_HH

/// \file
///
/// Extract horizontal lines matching a specific pattern.

# include <cmath>

# include <mln/core/concept/image.hh>
# include <mln/core/alias/window2d.hh>
# include <mln/win/rectangle2d.hh>
# include <mln/morpho/dilation.hh>

# include <mln/arith/times.hh>

# include <scribo/primitive/extract/lines_pattern.hh>

# include <scribo/primitive/internal/rd.hh>


namespace scribo
{

  namespace primitive
  {

    namespace extract
    {

      using namespace mln;

      /*! \brief Extract horizontal lines matching a specific pattern.

	\param[in] input  A binary image.
	\param[in] length The minimum line length.
	\param[in] delta Distance between the object pixel and the
	background pixel.

	\result An image of horizontal lines.


	     o
	     |     ^
	     |     |  Delta
	     |     v
	     X
	     |     ^
	     |     |  Delta
	     |     v
	     o

	Using a delta of 0 is equivalent to the use of a c2_row
	neighborhood.

      */
      template <typename I>
      mln_concrete(I)
      lines_h_pattern(const Image<I>& input, unsigned length, unsigned delta);


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      mln_concrete(I)
      lines_h_pattern(const Image<I>& input, unsigned length, unsigned delta)
      {
	trace::entering("scribo::primitive::extract::lines_h_pattern");

	mlc_is(mln_value(I), bool)::check();
	mln_precondition(exact(input).is_valid());

	// FIXME: not generic.
 	window2d win;
	mln_deduce(I, site, dpsite)
	  dp1(-delta - 1, 0),
	  dp2( delta + 1, 0);
	win.insert(dp1);
	win.insert(dp2);

	mln_concrete(I) output = lines_pattern(input, length, 1, win);

	mln_concrete(I)
	  output_dil = morpho::dilation(output,
					win::rectangle2d(2 * delta + 1,
							 length + 2));

	output = scribo::primitive::internal::rd(output, input * output_dil);

	trace::exiting("scribo::primitive::extract::lines_h_pattern");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_EXTRACT_LINES_H_PATTERN_HH

