// Copyright (C) 2009, 2010, 2011, 2012 EPITA Research and Development
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

#ifndef SCRIBO_PRIMITIVE_EXTRACT_LINES_H_THICK_AND_SINGLE_HH
# define SCRIBO_PRIMITIVE_EXTRACT_LINES_H_THICK_AND_SINGLE_HH

/// \file
///
/// \brief Extract horizontal thick lines in a binary image.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/morpho/opening/structural.hh>

# include <scribo/core/macros.hh>

# include <scribo/primitive/extract/lines_h_thick.hh>
# include <scribo/primitive/extract/lines_h_single.hh>


namespace scribo
{

  namespace primitive
  {

    namespace extract
    {


      using namespace mln;


      /*! \brief Extract horizontal thick lines in a binary image.
       *
       * Only non discontinued lines are correctly extracted with this
       * routine.  Only lines matching the given criterions are kept
       * in the result.
       *
       * \param[in]     input	  A binary image.
       * \param[in]     nbh	  The neighborhood used for labeling image
       *				  components.
       * \param[in,out] nlines	  Type used for labeling.
       * \param[in]     min_line_length The minimum line length.
       * \param[in]     h_w_ratio   The minimum ratio width/height object
       *                            bounding boxes to consider an
       *                            object as a single line.
       *
       * \return An image in which lines are labeled.
       *
       * \ingroup extractprimitiveseps
       */
      template <typename I, typename N, typename V>
      component_set<mln_ch_value(I,V)>
      lines_h_thick_and_single(const Image<I>& input,
			       const Neighborhood<N>& nbh,
			       V& nlines,
			       unsigned min_line_length,
			       float h_w_ratio);



# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

        template <typename I, typename N, typename V>
        void
	lines_h_thick_and_single_tests(const Image<I>& input,
				       const Neighborhood<N>& nbh,
				       V& nlines,
				       unsigned min_line_length,
				       float h_w_ratio)
	{
	  mlc_equal(mln_value(I),bool)::check();
	  mlc_bool(mln_site_(I)::dim == 2)::check();
	  mlc_is_a(V, mln::value::Symbolic)::check();

	  mln_precondition(exact(input).is_valid());
	  mln_precondition(exact(nbh).is_valid());

	  (void) input;
	  (void) nbh;
	  (void) min_line_length;
	  (void) h_w_ratio;
	  (void) nlines;
	}

      } // end of namespace scribo::primitive::internal



      template <typename I, typename N, typename V>
      inline
      component_set<mln_ch_value(I,V)>
      lines_h_thick_and_single(const Image<I>& input,
			       const Neighborhood<N>& nbh,
			       V& nlines,
			       unsigned min_line_length,
			       float h_w_ratio)
      {
	mln_trace("scribo::primitive::lines_h_thick_and_single");

	internal::lines_h_thick_and_single_tests(input, nbh,
						 nlines,
						 min_line_length,
						 h_w_ratio);
	component_set<mln_ch_value(I,V)>
	  output = lines_h_thick(input, nbh, nlines, min_line_length);

	output = lines_h_single(output, min_line_length, h_w_ratio);

	return output;
      }


# endif // !MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_EXTRACT_LINES_H_THICK_AND_SINGLE_HH
