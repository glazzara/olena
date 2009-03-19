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


#ifndef SCRIBO_DEBUG_SAVE_LINKED_TEXTBBOXES_IMAGE_HH
# define SCRIBO_DEBUG_SAVE_LINKED_TEXTBBOXES_IMAGE_HH

/// \file scribo/draw/bounding_boxes.hh
///
/// Save the bounding box links image.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/graph.hh>
# include <mln/level/convert.hh>
# include <mln/value/rgb8.hh>
# include <mln/util/array.hh>
# include <mln/io/ppm/save.hh>

# include <scribo/draw/bounding_boxes.hh>
# include <scribo/draw/bounding_box_links.hh>
# include <scribo/util/text.hh>


namespace scribo
{

  namespace debug
  {

    using namespace mln;


    /// Save the bounding box links image.
    template <typename I, typename L>
    void
    save_linked_textbboxes_image(const Image<I>& input,
				 const scribo::util::text<L>& text,
				 const mln::util::array<unsigned>& link_array,
				 const value::rgb8& box_value,
				 const value::rgb8& link_value,
				 const std::string& filename);


    template <typename I, typename L>
    void
    save_linked_textbboxes_image(const Image<I>& input,
				 const scribo::util::text<L>& text,
				 const mln::util::array<unsigned>& left_link,
				 const mln::util::array<unsigned>& right_array,
				 const value::rgb8& box_value,
				 const value::rgb8& left_link_value,
				 const value::rgb8& right_link_value,
				 const std::string& filename);


    template <typename I, typename L, typename G>
    void
    save_linked_textbboxes_image(const Image<I>& input,
				 const scribo::util::text<L>& text,
				 const Graph<G>& g,
				 const value::rgb8& box_value,
				 const value::rgb8& link_value,
				 const std::string& filename);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename L>
    inline
    void
    save_linked_textbboxes_image(const Image<I>& input,
				 const scribo::util::text<L>& text,
				 const mln::util::array<unsigned>& link_array,
				 const value::rgb8& box_value,
				 const value::rgb8& link_value,
				 const std::string& filename)
    {
      trace::entering("scribo::debug::save_linked_textbboxes_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) tmp = level::convert(value::rgb8(), input);
      draw::bounding_boxes(tmp, text.bboxes(), box_value);
      draw::bounding_box_links(tmp, text.bboxes(), link_array, link_value);
      io::ppm::save(tmp, filename);

      trace::exiting("scribo::debug::save_linked_textbboxes_image");
    }


    template <typename I, typename L>
    inline
    void
    save_linked_textbboxes_image(const Image<I>& input,
				 const scribo::util::text<L>& text,
				 const mln::util::array<unsigned>& left_link,
				 const mln::util::array<unsigned>& right_link,
				 const value::rgb8& box_value,
				 const value::rgb8& left_value,
				 const value::rgb8& right_value,
				 const std::string& filename)
    {
      trace::entering("scribo::debug::save_linked_textbboxes_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) tmp = level::convert(value::rgb8(), input);
      draw::bounding_boxes(tmp, text.bboxes(), box_value);
      draw::bounding_box_links(tmp, text.bboxes(),
			       left_link, right_link,
			       left_value, right_value);
      io::ppm::save(tmp, filename);

      trace::exiting("scribo::debug::save_linked_textbboxes_image");
    }


    template <typename I, typename L, typename G>
    inline
    void
    save_linked_textbboxes_image(const Image<I>& input,
				 const scribo::util::text<L>& text,
				 const Graph<G>& g,
				 const value::rgb8& box_value,
				 const value::rgb8& link_value,
				 const std::string& filename)
    {
      trace::entering("scribo::debug::save_linked_textbboxes_image");
      mln_precondition(exact(g).is_valid());
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) tmp = level::convert(value::rgb8(), input);
      draw::bounding_boxes(tmp, text.bboxes(), box_value);
      draw::bounding_box_links(tmp, text.bboxes(), g, link_value);
      io::ppm::save(tmp, filename);

      trace::exiting("scribo::debug::save_linked_textbboxes_image");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_SAVE_LINKED_TEXTBBOXES_IMAGE_HH
