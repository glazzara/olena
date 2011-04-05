// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_TEXT_EXTRACT_LINES_WITH_FEATURES_HH
# define SCRIBO_TEXT_EXTRACT_LINES_WITH_FEATURES_HH

/// \file
///
/// Extract line of text bounding boxes.


# include <mln/core/concept/image.hh>

# include <scribo/text/extract_lines.hh>


namespace scribo
{

  namespace text
  {

    using namespace mln;

    /// Extract lines of text in a binary image.
    /*!
    ** \param[in]     input  Original color image.
    ** \param[in]     input_binary  A binary image.
    ** \param[in]     nbh    A neighborhood used for labeling.
    ** \param[in]     separators   A binary image with separator information.
    **
    ** \return A set of lines.
    */
    template <typename I, typename J, typename N>
    line_set<mln_ch_value(I,scribo::def::lbl_type)>
    extract_lines_with_features(const Image<I>& input,
				const Image<J>& input_binary,
				const Neighborhood<N>& nbh,
				const mln_ch_value(I,bool)& separators);


    /// \overload
    template <typename I, typename J, typename N>
    line_set<mln_ch_value(I,scribo::def::lbl_type)>
    extract_lines_with_features(const Image<I>& input,
				const Image<J>& input_binary,
				const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename J, typename N>
    line_set<mln_ch_value(I,scribo::def::lbl_type)>
    extract_lines_with_features(const Image<I>& input,
				const Image<J>& input_binary,
				const Neighborhood<N>& nbh)
    {
      mln_ch_value(I,bool) seps;
      return extract_lines_with_features(input, input_binary, nbh, seps);
    }


    template <typename I, typename J, typename N>
    line_set<mln_ch_value(I,scribo::def::lbl_type)>
    extract_lines_with_features(const Image<I>& input_,
				const Image<J>& input_binary_,
				const Neighborhood<N>& nbh_,
				const mln_ch_value(I,bool)& separators)
    {
      trace::entering("scribo::text::extract_lines_with_features");

      const I& input = exact(input_);
      const J& input_binary = exact(input_binary_);
      const N& nbh = exact(nbh_);

      mln_precondition(input.is_valid());
      mln_precondition(input_binary.is_valid());
      mln_precondition(nbh.is_valid());

      /// Finding comps.
      typedef mln_ch_value(I,scribo::def::lbl_type) L;
      scribo::def::lbl_type ncomps;
      component_set<L>
	comps = scribo::primitive::extract::components(input, input_binary,
						       nbh, ncomps);

      /// First filtering.
      comps = scribo::filter::components_small(comps, 3);

      /// Use separators.
      if (exact(separators).is_valid())
	comps.add_separators(separators);

      line_set<mln_ch_value(I,scribo::def::lbl_type)>
	lines = extract_lines(comps);

      trace::exiting("scribo::text::extract_lines_with_features");
      return lines;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_EXTRACT_LINES_WITH_FEATURES_HH
