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

#ifndef SCRIBO_DEBUG_SAVE_LINKED_TEXTBBOXES_IMAGE_HH
# define SCRIBO_DEBUG_SAVE_LINKED_TEXTBBOXES_IMAGE_HH

/// \file scribo/draw/bounding_boxes.hh
///
/// Save the bounding box links image.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/graph.hh>
# include <mln/labeling/compute.hh>
# include <mln/accu/center.hh>
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


    /// Save the line of text links image.
    ///
    /// \param[in,out] input The binary from where the text is extracted.
    /// \param[in] text The lines of text.
    /// \param[in] link_array Lines of text links.
    /// \param[in] box_value Value used to draw line bounding boxes.
    /// \param[in] link_value Value used to draw line links.
    /// \param[in] filename The target file name.
    template <typename I, typename L>
    void
    save_linked_textbboxes_image(const Image<I>& input,
				 const scribo::util::text<L>& text,
				 const mln::util::array<unsigned>& link_array,
				 const value::rgb8& box_value,
				 const value::rgb8& link_value,
				 const std::string& filename);

    /// Save the line of text left and right links image.
    ///
    /// \param[in,out] input The binary from where the text is extracted.
    /// \param[in] text The lines of text.
    /// \param[in] left_link Lines of text left links.
    /// \param[in] right_link Lines of text right links.
    /// \param[in] box_value Value used to draw line bounding boxes.
    /// \param[in] link_value Value used to draw line links.
    /// \param[in] filename The target file name.
    template <typename I, typename L>
    void
    save_linked_textbboxes_image(const Image<I>& input,
				 const scribo::util::text<L>& text,
				 const mln::util::array<unsigned>& left_link,
				 const mln::util::array<unsigned>& right_link,
				 const value::rgb8& box_value,
				 const value::rgb8& link_value,
				 const std::string& filename);

    /// Save the line of text left and right links image.
    /// Draw also validated links.
    ///
    /// \param[in,out] input The binary from where the text is extracted.
    /// \param[in] text The lines of text.
    /// \param[in] left_link Lines of text left links.
    /// \param[in] right_link Lines of text right links.
    /// \param[in] box_value Value used to draw line bounding boxes.
    /// \param[in] left_link_value Value used to draw line left links.
    /// \param[in] right_link_value Value used to draw line left links.
    /// \param[in] validated_link_value Value used to draw line validated links.
    /// \param[in] filename The target file name.
    template <typename I, typename L>
    inline
    void
    save_linked_textbboxes_image(const Image<I>& input,
				 const scribo::util::text<L>& text,
				 const mln::util::array<unsigned>& left_link,
				 const mln::util::array<unsigned>& right_link,
				 const value::rgb8& box_value,
				 const value::rgb8& left_link_value,
				 const value::rgb8& right_link_value,
				 const value::rgb8& validated_link_value,
				 const std::string& filename);


    /// Save the line link graph image.
    ///
    /// \param[in,out] input The binary from where the text is extracted.
    /// \param[in] text The lines of text.
    /// \param[in] g The link graph.
    /// \param[in] box_value Value used to draw line bounding boxes.
    /// \param[in] link_value Value used to draw line links.
    /// \param[in] filename The target file name.
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
      draw::bounding_box_links(tmp, text.mass_centers(), link_array, link_value);

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
				 const value::rgb8& value,
				 const std::string& filename)
    {
      trace::entering("scribo::debug::save_linked_textbboxes_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) tmp = level::convert(value::rgb8(), input);

      draw::bounding_boxes(tmp, text.bboxes(), box_value);
      draw::bounding_box_links(tmp, text.mass_centers(),
			       left_link, right_link,
			       value);

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
				 const value::rgb8& left_link_value,
				 const value::rgb8& right_link_value,
				 const value::rgb8& validated_link_value,
				 const std::string& filename)
    {
      trace::entering("scribo::debug::save_linked_textbboxes_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) tmp = level::convert(value::rgb8(), input);

      draw::bounding_boxes(tmp, text.bboxes(), box_value);
      draw::bounding_box_links(tmp, text.mass_centers(),
			       left_link, right_link,
			       left_link_value, right_link_value,
			       validated_link_value);

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
