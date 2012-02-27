// Copyright (C) 2010, 2011, 2012 EPITA Research and Development
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

#ifndef SCRIBO_PRIMITIVE_EXTRACT_SEPARATORS_HH
# define SCRIBO_PRIMITIVE_EXTRACT_SEPARATORS_HH

/// \file
///
/// Extract vertical and horizontal separators.

# include <mln/core/concept/image.hh>
# include <mln/arith/plus.hh>

# include <scribo/primitive/extract/lines_h_pattern.hh>
# include <scribo/primitive/extract/lines_v_pattern.hh>


namespace scribo
{

  namespace primitive
  {

    namespace extract
    {

      using namespace mln;

      /*! \brief Extract vertical and horizontal separators.
       *
       *
       * \param[in]     input       A binary image.
       * \param[in]     line_length The minimum line length.
       *
       * \return A binary image were separators are set to 'True'.
       *
       * \sa primitive::remove::separators
       *
       * \ingroup extractprimitiveseps
       */
      template <typename I>
      mln_concrete(I)
      separators(const Image<I>& input, unsigned line_length);



# ifndef MLN_INCLUDE_ONLY


      template <typename I>
      mln_concrete(I)
      separators(const Image<I>& input_, unsigned line_length)
      {
	trace::entering("scribo::primitive::extract::separators");

	const I& input = exact(input_);
	mlc_is(mln_value(I), bool)::check();
	mln_precondition(input.is_valid());

	mln_concrete(I)
	  hlines = extract::lines_h_pattern(input, line_length, 3),
	  vlines = extract::lines_v_pattern(input, line_length, 3);
	hlines += vlines;

	// FIXME: UGLY! We should not do that!
	// Restore border size.
	// Previous treatments may have changed it.
	border::resize(hlines, border::thickness);
	border::resize(input, border::thickness);

	trace::exiting("scribo::primitive::extract::separators");
	return hlines;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_EXTRACT_SEPARATORS_HH
