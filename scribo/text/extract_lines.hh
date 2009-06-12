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

#ifndef SCRIBO_TEXT_EXTRACT_LINES_HH
# define SCRIBO_TEXT_EXTRACT_LINES_HH

/// \file
///
/// Extract line of text bounding boxes.


# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>

# include <mln/data/fill.hh>

# include <mln/accu/bbox.hh>

# include <mln/draw/box.hh>

# include <mln/labeling/blobs.hh>
# include <mln/labeling/compute.hh>

# include <mln/util/array.hh>
# include <mln/util/graph.hh>
# include <mln/value/label_16.hh>

# include <scribo/extract/primitive/objects.hh>
# include <scribo/text/grouping/group_with_several_left_links.hh>
# include <scribo/text/grouping/group_with_several_right_links.hh>
# include <scribo/text/grouping/group_from_double_link.hh>

# ifndef SCRIBO_NDEBUG
#  include <mln/literal/colors.hh>
#  include <scribo/make/debug_filename.hh>
#  include <scribo/debug/save_bboxes_image.hh>
#  include <scribo/debug/save_linked_bboxes_image.hh>
# endif // SCRIBO_NDEBUG

namespace scribo
{

  namespace text
  {

    using namespace mln;

    /// Extract lines of text in a binary image.
    /*!
    ** \param[in]     input_  A binary image.
    ** \param[in]     nbh_    A neighborhood used for labeling.
    ** \param[in,out] nbboxes Will hold the number of bounding boxes
    **			      at the end of the routine.
    **
    ** \return An object image with grouped potential text objects.
    */
    template <typename I, typename N, typename V>
    object_image(mln_ch_value(I,V))
    extract_lines(const Image<I>& input_,
		  const Neighborhood<N>& nbh_,
		  V& nbboxes);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename N, typename V>
    object_image(mln_ch_value(I,V))
    extract_lines(const Image<I>& input_,
		  const Neighborhood<N>& nbh_,
		  V& nbboxes)
    {
      trace::entering("scribo::text::extract_lines");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);

      mln_precondition(input.is_valid());
      mln_precondition(nbh.is_valid());

      typedef object_image(mln_ch_value(I,V)) text_t;
      text_t text = scribo::extract::primitive::objects(input, nbh, nbboxes);

# ifndef SCRIBO_NDEBUG
      debug::save_bboxes_image(input, text.bboxes(), literal::red,
			       scribo::make::debug_filename("character-bboxes.ppm"));
# endif // ! SCRIBO_NDEBUG

      //Link character bboxes to their left neighboor if possible.
      mln::util::array<unsigned> left_link
	= text::grouping::group_with_several_left_links(text, 30);
      mln::util::array<unsigned> right_link
	= text::grouping::group_with_several_right_links(text, 30);

# ifndef SCRIBO_NDEBUG
      scribo::debug::save_linked_bboxes_image(input,
					      text, left_link, right_link,
					      literal::red, literal::cyan,
					      literal::yellow, literal::green,
					      scribo::make::debug_filename("links.ppm"));
# endif // ! SCRIBO_NDEBUG

      //Merge character bboxes through a graph.
      text_t grouped_text
	  = text::grouping::group_from_double_link(text, left_link, right_link);

# ifndef SCRIBO_NDEBUG
      debug::save_bboxes_image(input, grouped_text.bboxes(),
			       literal::red,
			       scribo::make::debug_filename("multiple_links_grouped_text.ppm"));
# endif // ! SCRIBO_NDEBUG


      trace::exiting("scribo::text::extract_lines");
      return grouped_text;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_EXTRACT_LINES_HH
