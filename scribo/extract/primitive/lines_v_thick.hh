// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef SCRIBO_EXTRACT_PRIMITIVE_LINES_V_THICK_HH
# define SCRIBO_EXTRACT_PRIMITIVE_LINES_V_THICK_HH

/// \file scribo/primitive/lines_v_thick.hh
///
/// Extract vertical thick lines.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/win/vline2d.hh>

# include <scribo/core/object_image.hh>
# include <scribo/extract/primitive/lines_thick.hh>

namespace scribo
{

  namespace extract
  {

    namespace primitive
    {

      using namespace mln;

      /// Extract thick lines in a binary image.
      /*!
       * Only non discontinued lines are correctly extracted with this routine.
       *
       * \param[in]     input_	    A binary image.
       * \param[in]     nbh_	    The neighborhood used for labeling image
       *			    components.
       * \param[in,out] nlines	    Type used for labeling.
       * \param[in]     line_length The minimum line length.
       *
       * \return An image in which lines are labeled.
       */
      template <typename I, typename N, typename V, typename W>
      object_image(mln_ch_value(I,V))
      lines_thick(const Image<I>& input_,
		  const Neighborhood<N>& nbh_, V& nlines,
		  unsigned line_length);



# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

        template <typename I, typename N, typename V>
	void
        lines_v_thick_tests(const Image<I>& input,
			    const Neighborhood<N>& nbh, V& nlines,
			    unsigned line_length)
	{
	  mlc_equal(mln_value(I),bool)::check();
	  mlc_bool(mln_site_(I)::dim == 2)::check();
	  mlc_is_a(V, mln::value::Symbolic)::check();

	  mln_precondition(exact(input).is_valid());
	  mln_precondition(exact(nbh).is_valid());
	  mln_precondition(line_length % 2);

	  (void) input;
	  (void) nbh;
	  (void) nlines;
	  (void) line_length;
	}

      } // end of namespace scribo::primitive::internal



      template <typename I, typename N, typename V>
      object_image(mln_ch_value(I,V))
      lines_v_thick(const Image<I>& input,
			   const Neighborhood<N>& nbh, V& nlines,
			   unsigned line_length)
      {
	trace::entering("scribo::primitive::lines_v_thick");

	internal::lines_v_thick_tests(input, nbh, nlines,
	    line_length);

	win::vline2d win(line_length);

	object_image(mln_ch_value(I,V))
	  output = lines_thick(input, nbh, nlines, win);

	trace::exiting("scribo::primitive::lines_v_thick");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::extract::primitive

  } // end of namespace scribo::extract

} // end of namespace scribo

#endif // ! SCRIBO_EXTRACT_PRIMITIVE_LINES_V_THICK_HH

