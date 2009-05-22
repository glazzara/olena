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

#ifndef SCRIBO_EXTRACT_PRIMITIVE_LINES_V_DISCONTINUED_HH
# define SCRIBO_EXTRACT_PRIMITIVE_LINES_V_DISCONTINUED_HH

/// \file scribo/primitive/lines_v_discontinued.hh
///
/// Extract vertical discontinued lines.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>


namespace scribo
{

  namespace extract
  {

    namespace primitive
    {


      /// Extract vertical discontinued lines.
      /*!
       * \param[in]     input	    A binary image.
       * \param[in]     nbh	    A neighborhood used to label lines.
       * \param[in,out] nlines	    The number of lines found.
       * \param[in]     line_length The minimum line length expected. (must be
       *			    odd).
       * \param[in]     rank_k	    Rank filter parameter.
       * \param[in,out] line_bboxes Will store the line bounding boxes.
       *
       * \result An image in which lines are labeled with a value different
       * from 0.
       */
      template <typename I, typename N, typename V>
      mln_ch_value(I,V)
      lines_v_discontinued(const Image<I>& input,
			   const Neighborhood<N>& nbh, V& nlines,
			   unsigned line_length, unsigned rank_k,
			   util::array<box<mln_site(I)> >& line_bboxes);

      /// \overload
      template <typename I, typename N, typename V>
      mln_ch_value(I,V)
      lines_v_discontinued(const Image<I>& input,
			   const Neighborhood<N>& nbh, V& nlines,
			   unsigned line_length, unsigned rank_k);


# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

        template <typename I, typename N, typename V, typename W>
        void
        lines_v_discontinued_tests(const Image<I>& input_,
				   const Neighborhood<N>& nbh_, V& nlines,
				   unsigned line_length, unsigned rank_k)
	{
	  mlc_equal(mln_value(I),bool)::check();
	  mlc_equal(mln_site(I)::dim, 2)::check();
	  mlc_is_a(V, mln::value::Symbolic)::check();

	  mln_precondition(exact(input).is_valid());
	  mln_precondition(exact(nbh).is_valid());
	  mln_precondition(exact(win).is_valid());
	  mln_precondition(!(line_length % 2));

	  (void) nlines;
	  (void) rank_k;
	}

      } // end of namespace scribo::primitive::internal



      template <typename I, typename N, typename V>
      mln_ch_value(I,V)
      lines_v_discontinued(const Image<I>& input,
			   const Neighborhood<N>& nbh, V& nlines,
			   unsigned line_length, unsigned rank_k)
      {
	trace::entering("scribo::primitive::lines_v_discontinued");

	internal::line_v_discontinued_tests(input, nbh, nlines,
	    line_length, rank_k);

	win::hline2d win(line_length);

	mln_ch_value(I,V)
	  output = lines_discontinued(input, nh, nlines, win, rank_k);

	trace::exiting("scribo::primitive::lines_v_discontinued");
	return output;
      }



      template <typename I, typename N, typename V>
      mln_ch_value(I,V)
      lines_v_discontinued(const Image<I>& input,
			   const Neighborhood<N>& nbh, V& nlines,
			   unsigned line_length, unsigned rank_k,
			   util::array<box<mln_site(I)> >& line_bboxes)
      {
	trace::entering("scribo::primitive::lines_v_discontinued");

	internal::line_v_discontinued_tests(input, nbh, nlines,
	    line_length, rank_k);

	win::vline2d win(line_length);
	mln_ch_value(I,V)
	  output = internal::line_v_discontinued_tests(input, nbh, nlines,
	      line_length, rank_k,
	      line_bboxes);

	trace::exiting("scribo::primitive::lines_v_discontinued");
	return output;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::extract::primitive

  } // end of namespace scribo::extract

} // end of namespace scribo


#endif // ! SCRIBO_EXTRACT_PRIMITIVE_LINES_V_DISCONTINUED_HH
