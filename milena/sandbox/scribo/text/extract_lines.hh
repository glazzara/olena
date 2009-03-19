// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_TEXT_EXTRACT_LINES_HH
# define SCRIBO_TEXT_EXTRACT_LINES_HH

/// \file scribo/text/extract_lines.hh
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

# include <scribo/text/grouping/group_with_multiple_links.hh>
# include <scribo/text/grouping/group_from_multiple_links.hh>
# include <scribo/filter/small_components.hh>
# include <scribo/util/text.hh>

# include <scribo/debug/save_textbboxes_image.hh>
# include <scribo/debug/save_linked_textbboxes_image.hh>

namespace scribo
{

  namespace text
  {

    using namespace mln;


    template <typename I, typename N, typename V>
    scribo::util::text<mln_ch_value(I,V)>
    extract_lines(const Image<I>& input_,
		  const Neighborhood<N>& nbh_,
		  V& nbboxes);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename N, typename V>
    scribo::util::text<mln_ch_value(I,V)>
    extract_lines(const Image<I>& input_,
		  const Neighborhood<N>& nbh_,
		  V& nbboxes)
    {
      trace::entering("scribo::text::extract_lines");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);

      mln_precondition(input.is_valid());
      mln_precondition(nbh.is_valid());

      scribo::util::text<mln_ch_value(I,V)> text
	    = scribo::make::text(input, nbh, nbboxes);

# ifndef SCRIBO_NDEBUG
      debug::save_textbboxes_image(input, text.bboxes(), literal::red,
				   scrib::make::debug_filename("character-bboxes.ppm"));
# endif // ! SCRIBO_NDEBUG

      //Link character bboxes to their left neighboor if possible.
      mln::util::graph g
	= text::grouping::group_with_multiple_links(text, 30);
# ifndef SCRIBO_NDEBUG
      debug::save_linked_textbboxes_image(input,
					  text, g,
					  literal::red, literal::cyan,
					  scribo::make::debug_filename("multiple_links_left_linked.ppm"));
# endif // ! SCRIBO_NDEBUG

      //Merge character bboxes through a graph.
      scribo::util::text<mln_ch_value(I,V)> grouped_text
	  = text::grouping::group_from_multiple_links(text, g);

# ifndef SCRIBO_NDEBUG
      debug::save_textbboxes_image(input, grouped_text.bboxes(),
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

