// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_DEBUG_LINKED_BBOXES_IMAGE_HH
# define SCRIBO_DEBUG_LINKED_BBOXES_IMAGE_HH

/// \file
///
/// Compute the bounding box links image.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/graph.hh>
# include <mln/labeling/compute.hh>
# include <mln/accu/center.hh>
# include <mln/data/convert.hh>
# include <mln/value/rgb8.hh>
# include <mln/util/array.hh>
# include <mln/io/ppm/save.hh>
# include <mln/literal/colors.hh>

# include <scribo/core/object_links.hh>
# include <scribo/core/component_set.hh>
# include <scribo/draw/bounding_boxes.hh>
# include <scribo/draw/bounding_box_links.hh>


namespace scribo
{

  namespace debug
  {

    using namespace mln;


    /// Compute the line of components links image.
    ///
    /// \param[in,out] input The binary from where the components are extracted.
    /// \param[in] link_array Lines of components links.
    /// \param[in] box_value Value used to draw line bounding boxes.
    /// \param[in] link_value Value used to draw line links.
    /// \param[in] anchor Anchor from where the links are drawn.
    //
    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    linked_bboxes_image(const Image<I>& input,
			const object_links<L>& array,
			const value::rgb8& box_value,
			const value::rgb8& link_value,
			anchor::Type anchor);

    /// \overload
    /// The default anchor type is set to anchor::Center.
    //
    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    linked_bboxes_image(const Image<I>& input,
			const object_links<L>& array,
			const value::rgb8& box_value,
			const value::rgb8& link_value);

    /// Compute the line of components left and right links image.
    ///
    /// \param[in,out] input The binary from where the components are extracted.
    /// \param[in] left_link Lines of components left links.
    /// \param[in] right_link Lines of components right links.
    /// \param[in] box_value Value used to draw line bounding boxes.
    /// \param[in] link_value Value used to draw line links.
    //
    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    linked_bboxes_image(const Image<I>& input,
			const object_links<L>& left_link,
			const object_links<L>& right_link,
			const value::rgb8& box_value,
			const value::rgb8& link_value,
			anchor::Type anchor);

    /// Compute the line of components left and right links image.
    /// Draw also validated links.
    ///
    /// \param[in,out] input The binary from where the components are extracted.
    /// \param[in] left_link Lines of components left links.
    /// \param[in] right_link Lines of components right links.
    /// \param[in] box_value Value used to draw line bounding boxes.
    /// \param[in] left_link_value Value used to draw line left links.
    /// \param[in] right_link_value Value used to draw line left links.
    /// \param[in] validated_link_value Value used to draw line validated links.
    //
    template <typename I, typename L>
    inline
    mln_ch_value(I,value::rgb8)
    linked_bboxes_image(const Image<I>& input,
			const object_links<L>& left_link,
			const object_links<L>& right_link,
			const value::rgb8& box_value,
			const value::rgb8& left_link_value,
			const value::rgb8& right_link_value,
			const value::rgb8& validated_link_value,
			anchor::Type anchor);


    /// Compute the line link graph image.
    ///
    /// \param[in,out] input The binary from where the components are extracted.
    /// \param[in] g The link graph.
    /// \param[in] box_value Value used to draw line bounding boxes.
    /// \param[in] link_value Value used to draw line links.
    //
    template <typename I, typename L, typename G>
    mln_ch_value(I,value::rgb8)
    linked_bboxes_image(const Image<I>& input,
			const Graph<G>& g,
			const value::rgb8& box_value,
			const value::rgb8& link_value,
			anchor::Type anchor);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename L>
    inline
    mln_ch_value(I,value::rgb8)
    linked_bboxes_image(const Image<I>& input,
			const object_links<L>& links,
			const value::rgb8& box_value,
			const value::rgb8& link_value,
			anchor::Type anchor)
    {
      trace::entering("scribo::debug::linked_bboxes_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) tmp = data::convert(value::rgb8(), input);

      draw::bounding_boxes(tmp, links.components(), box_value);
      draw::bounding_box_links(tmp, links, link_value, anchor);

      trace::exiting("scribo::debug::linked_bboxes_image");
      return tmp;
    }


    template <typename I, typename L>
    inline
    mln_ch_value(I,value::rgb8)
    linked_bboxes_image(const Image<I>& input,
			const object_links<L>& links,
			const value::rgb8& box_value,
			const value::rgb8& link_value)
    {
      return linked_bboxes_image(input, links, box_value,
				 link_value, anchor::Center);
    }



    template <typename I, typename L>
    inline
    mln_ch_value(I,value::rgb8)
    linked_bboxes_image(const Image<I>& input,
			const object_links<L>& left_link,
			const object_links<L>& right_link,
			const value::rgb8& box_value,
			const value::rgb8& value)
    {
      return linked_bboxes_image(input,
				 left_link,
				 right_link,
				 box_value,
				 literal::yellow,
				 literal::cyan,
				 value,
				 anchor::Center);
    }


    template <typename I, typename L>
    inline
    mln_ch_value(I,value::rgb8)
    linked_bboxes_image(const Image<I>& input,
			const object_links<L>& left_link,
			const object_links<L>& right_link,
			const value::rgb8& box_value,
			const value::rgb8& left_link_value,
			const value::rgb8& right_link_value,
			const value::rgb8& validated_link_value,
			anchor::Type anchor)
    {
      trace::entering("scribo::debug::linked_bboxes_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) tmp = data::convert(value::rgb8(), input);

      draw::bounding_boxes(tmp, left_link.components(), box_value);
      draw::bounding_box_links(tmp,
			       left_link, right_link,
			       left_link_value, right_link_value,
			       validated_link_value,
			       anchor);

      trace::exiting("scribo::debug::linked_bboxes_image");
      return tmp;
    }



    template <typename I, typename L, typename G>
    inline
    mln_ch_value(I,value::rgb8)
    linked_bboxes_image(const Image<I>& input,
			const Graph<G>& g,
			const value::rgb8& box_value,
			const value::rgb8& link_value,
			anchor::Type anchor)
    {
      trace::entering("scribo::debug::linked_bboxes_image");
      mln_precondition(exact(g).is_valid());
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) tmp = data::convert(value::rgb8(), input);

      draw::bounding_boxes(tmp, exact(g).components(), box_value);
      draw::bounding_box_links(tmp, exact(g).components(), g, link_value,
			       anchor);

      trace::exiting("scribo::debug::linked_bboxes_image");
      return tmp;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_LINKED_BBOXES_IMAGE_HH
