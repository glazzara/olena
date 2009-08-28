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

#ifndef SCRIBO_DEBUG_SAVE_LINKED_BBOXES_IMAGE_HH
# define SCRIBO_DEBUG_SAVE_LINKED_BBOXES_IMAGE_HH

/// \file
///
/// Save the bounding box links image.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/graph.hh>
# include <mln/labeling/compute.hh>
# include <mln/accu/center.hh>
# include <mln/data/convert.hh>
# include <mln/value/rgb8.hh>
# include <mln/util/array.hh>
# include <mln/io/ppm/save.hh>

# include <scribo/core/object_image.hh>
# include <scribo/draw/bounding_boxes.hh>
# include <scribo/draw/bounding_box_links.hh>


namespace scribo
{

  namespace debug
  {

    using namespace mln;


    /// Save the line of objects links image.
    ///
    /// \param[in,out] input The binary from where the objects are extracted.
    /// \param[in] objects An object image.
    /// \param[in] link_array Lines of objects links.
    /// \param[in] box_value Value used to draw line bounding boxes.
    /// \param[in] link_value Value used to draw line links.
    /// \param[in] filename The target file name.
    template <typename I, typename L>
    void
    save_linked_bboxes_image(const Image<I>& input,
			     const object_image(L)& objects,
			     const object_links<L>& array,
			     const value::rgb8& box_value,
			     const value::rgb8& link_value,
			     const std::string& filename);

    /// Save the line of objects left and right links image.
    ///
    /// \param[in,out] input The binary from where the objects are extracted.
    /// \param[in] objects An object image.
    /// \param[in] left_link Lines of objects left links.
    /// \param[in] right_link Lines of objects right links.
    /// \param[in] box_value Value used to draw line bounding boxes.
    /// \param[in] link_value Value used to draw line links.
    /// \param[in] filename The target file name.
    template <typename I, typename L>
    void
    save_linked_bboxes_image(const Image<I>& input,
			     const object_image(L)& objects,
			     const object_links<L>& left_link,
			     const object_links<L>& right_link,
			     const value::rgb8& box_value,
			     const value::rgb8& link_value,
			     const std::string& filename);

    /// Save the line of objects left and right links image.
    /// Draw also validated links.
    ///
    /// \param[in,out] input The binary from where the objects are extracted.
    /// \param[in] objects An object image.
    /// \param[in] left_link Lines of objects left links.
    /// \param[in] right_link Lines of objects right links.
    /// \param[in] box_value Value used to draw line bounding boxes.
    /// \param[in] left_link_value Value used to draw line left links.
    /// \param[in] right_link_value Value used to draw line left links.
    /// \param[in] validated_link_value Value used to draw line validated links.
    /// \param[in] filename The target file name.
    template <typename I, typename L>
    inline
    void
    save_linked_bboxes_image(const Image<I>& input,
			     const object_image(L)& objects,
			     const object_links<L>& left_link,
			     const object_links<L>& right_link,
			     const value::rgb8& box_value,
			     const value::rgb8& left_link_value,
			     const value::rgb8& right_link_value,
			     const value::rgb8& validated_link_value,
			     const std::string& filename);


    /// Save the line link graph image.
    ///
    /// \param[in,out] input The binary from where the objects are extracted.
    /// \param[in] objects An object image.
    /// \param[in] g The link graph.
    /// \param[in] box_value Value used to draw line bounding boxes.
    /// \param[in] link_value Value used to draw line links.
    /// \param[in] filename The target file name.
    template <typename I, typename L, typename G>
    void
    save_linked_bboxes_image(const Image<I>& input,
			     const object_image(L)& objects,
			     const Graph<G>& g,
			     const value::rgb8& box_value,
			     const value::rgb8& link_value,
			     const std::string& filename);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename L>
    inline
    void
    save_linked_bboxes_image(const Image<I>& input,
			     const object_image(L)& objects,
			     const object_links<L>& array,
			     const value::rgb8& box_value,
			     const value::rgb8& link_value,
			     const std::string& filename)
    {
      trace::entering("scribo::debug::save_linked_bboxes_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) tmp = data::convert(value::rgb8(), input);

      mln::util::array<mln_result(accu::center<mln_psite(L)>)>
	    mass_center = labeling::compute(accu::meta::center(),
					    objects,
					    objects.nlabels());

      draw::bounding_boxes(tmp, objects.bboxes(), box_value);
      draw::bounding_box_links(tmp, mass_center, array, link_value);

      io::ppm::save(tmp, filename);

      trace::exiting("scribo::debug::save_linked_bboxes_image");
    }


    template <typename I, typename L>
    inline
    void
    save_linked_bboxes_image(const Image<I>& input,
			     const object_image(L)& objects,
			     const object_links<L>& left_link,
			     const object_links<L>& right_link,
			     const value::rgb8& box_value,
			     const value::rgb8& value,
			     const std::string& filename)
    {
      trace::entering("scribo::debug::save_linked_bboxes_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) tmp = data::convert(value::rgb8(), input);

      mln::util::array<mln_result(accu::center<mln_psite(L)>)>
	    mass_center = labeling::compute(accu::meta::center(),
					    objects,
					    objects.nlabels());

      draw::bounding_boxes(tmp, objects.bboxes(), box_value);
      draw::bounding_box_links(tmp, mass_center,
			       left_link, right_link,
			       value);

      io::ppm::save(tmp, filename);

      trace::exiting("scribo::debug::save_linked_bboxes_image");
    }


    template <typename I, typename L>
    inline
    void
    save_linked_bboxes_image(const Image<I>& input,
			     const object_image(L)& objects,
			     const object_links<L>& left_link,
			     const object_links<L>& right_link,
			     const value::rgb8& box_value,
			     const value::rgb8& left_link_value,
			     const value::rgb8& right_link_value,
			     const value::rgb8& validated_link_value,
			     const std::string& filename)
    {
      trace::entering("scribo::debug::save_linked_bboxes_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) tmp = data::convert(value::rgb8(), input);

      mln::util::array<mln_result(accu::center<mln_psite(L)>)>
	mass_center = labeling::compute(accu::meta::center(),
					objects,
					objects.nlabels());

      draw::bounding_boxes(tmp, objects.bboxes(), box_value);
      draw::bounding_box_links(tmp, mass_center,
			       left_link, right_link,
			       left_link_value, right_link_value,
			       validated_link_value);

      io::ppm::save(tmp, filename);

      trace::exiting("scribo::debug::save_linked_bboxes_image");
    }



    template <typename I, typename L, typename G>
    inline
    void
    save_linked_bboxes_image(const Image<I>& input,
			     const object_image(L)& objects,
			     const Graph<G>& g,
			     const value::rgb8& box_value,
			     const value::rgb8& link_value,
			     const std::string& filename)
    {
      trace::entering("scribo::debug::save_linked_bboxes_image");
      mln_precondition(exact(g).is_valid());
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) tmp = data::convert(value::rgb8(), input);

      draw::bounding_boxes(tmp, objects.bboxes(), box_value);
      draw::bounding_box_links(tmp, objects.bboxes(), g, link_value);

      io::ppm::save(tmp, filename);

      trace::exiting("scribo::debug::save_linked_bboxes_image");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_SAVE_LINKED_BBOXES_IMAGE_HH
