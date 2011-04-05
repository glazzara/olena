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

#ifndef SCRIBO_TEXT_EXTRACT_LINES_HH
# define SCRIBO_TEXT_EXTRACT_LINES_HH

/// \file
///
/// Extract line of text bounding boxes.


# include <mln/core/concept/image.hh>

# include <scribo/core/def/lbl_type.hh>
# include <scribo/core/line_set.hh>

# include <scribo/primitive/extract/components.hh>

# include <scribo/primitive/link/merge_double_link.hh>
# include <scribo/primitive/link/internal/dmax_width_and_height.hh>
# include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
# include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>

# include <scribo/filter/objects_small.hh>

# include <scribo/primitive/group/apply.hh>
# include <scribo/primitive/group/from_single_link.hh>

# include <scribo/filter/object_links_bbox_h_ratio.hh>

# include <scribo/text/merging.hh>


namespace scribo
{

  namespace text
  {

    using namespace mln;

    /// Extract lines of text in a binary image.
    /*!
    ** \param[in]     input  A binary image.
    ** \param[in]     nbh    A neighborhood used for labeling.
    ** \param[in]     seps   A binary image with separator information.
    **
    ** \return A set of lines.
    */
    template <typename I, typename N>
    line_set<mln_ch_value(I,scribo::def::lbl_type)>
    extract_lines(const Image<I>& input_, const Neighborhood<N>& nbh_,
		  const mln_ch_value(I,bool)& separators);

    /// \overload
    template <typename I, typename N>
    line_set<mln_ch_value(I,scribo::def::lbl_type)>
    extract_lines(const Image<I>& input, const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename N>
    line_set<mln_ch_value(I,scribo::def::lbl_type)>
    extract_lines(const Image<I>& input, const Neighborhood<N>& nbh)
    {
      mln_ch_value(I,bool) seps;
      return extract_lines(input, nbh, seps);
    }


    template <typename I, typename N>
    line_set<mln_ch_value(I,scribo::def::lbl_type)>
    extract_lines(const Image<I>& input_, const Neighborhood<N>& nbh_,
		  const mln_ch_value(I,bool)& separators)
    {
      trace::entering("scribo::text::extract_lines");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);

      mln_precondition(input.is_valid());
      mln_precondition(nbh.is_valid());

      /// Finding comps.
      typedef mln_ch_value(I,scribo::def::lbl_type) L;
      scribo::def::lbl_type ncomps;
      component_set<L>
	comps = scribo::primitive::extract::components(input, nbh, ncomps);

      /// First filtering.
      comps = scribo::filter::components_small(comps, 3);

      /// Use separators.
      if (exact(separators).is_valid())
	comps.add_separators(separators);

      /// Linking potential comps
      object_links<L>
	left_link = primitive::link::with_single_left_link_dmax_ratio(
	  comps,
//	  primitive::link::internal::dmax_width_and_height(1),
	  primitive::link::internal::dmax_default(1),
	  anchor::MassCenter);

      object_links<L>
	right_link = primitive::link::with_single_right_link_dmax_ratio(
	  comps,
//	  primitive::link::internal::dmax_width_and_height(1),
	  primitive::link::internal::dmax_default(1),
	  anchor::MassCenter);

      // Validating left and right links.
      object_links<L>
	merged_links = primitive::link::merge_double_link(left_link,
						      right_link);


      object_links<L> hratio_filtered_links
	= filter::object_links_bbox_h_ratio(merged_links, 2.5f);

      object_groups<L>
	groups = primitive::group::from_single_link(hratio_filtered_links);

      line_set<L> lines(groups);
      lines = text::merging(lines);

      trace::exiting("scribo::text::extract_lines");
      return lines;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_EXTRACT_LINES_HH
