// Copyright (C) 2009, 2010, 2012 EPITA Research and Development
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

#ifndef SCRIBO_PRIMITIVE_EXTRACT_LINES_H_DISCONTINUED_HH
# define SCRIBO_PRIMITIVE_EXTRACT_LINES_H_DISCONTINUED_HH

/// \file
///
/// Extract horizontal discontinued lines.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/win/hline2d.hh>

# include <scribo/primitive/extract/lines_discontinued.hh>



namespace scribo
{

  namespace primitive
  {

    namespace extract
    {

      using namespace mln;

      /// Extract horizontal discontinued lines.
      /*!
       * \param[in]     input   A binary image.
       * \param[in]     nbh     A neighborhood used to label lines.
       * \param[in,out] nlines  The number of lines found.
       * \param[in]     line_length The minimum line length expected. (must be
       *		      odd).
       * \param[in]     rank_k  Rank filter parameter.
       *
       * \result An image in which lines are labeled with a value different
       * from 0.
       *
       * \ingroup extractprimitiveseps
       */
      template <typename I, typename N, typename V>
      component_set<mln_ch_value(I,V)>
      lines_h_discontinued(const Image<I>& input,
			   const Neighborhood<N>& nbh, V& nlines,
			   unsigned line_length, unsigned rank_k);


# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

        template <typename I, typename N, typename V>
	void
        lines_h_discontinued_tests(const Image<I>& input,
				   const Neighborhood<N>& nbh, V& nlines,
				   unsigned line_length, unsigned rank_k)
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
	  (void) rank_k;
	}

      } // end of namespace scribo::primitive::internal



      template <typename I, typename N, typename V>
      component_set<mln_ch_value(I,V)>
      lines_h_discontinued(const Image<I>& input,
			   const Neighborhood<N>& nbh, V& nlines,
			   unsigned line_length, unsigned rank_k)
      {
	mln_trace("scribo::primitive::lines_h_discontinued");

	internal::lines_h_discontinued_tests(input, nbh, nlines,
	    line_length, rank_k);

	win::hline2d win(line_length);

	component_set<mln_ch_value(I,V)>
	  output = lines_discontinued(input, nbh, nlines, win, rank_k);

	return output;
      }



# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_EXTRACT_LINES_H_DISCONTINUED_HH
