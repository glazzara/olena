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

#ifndef SCRIBO_DEBUG_SAVE_BBOXES_IMAGE_HH
# define SCRIBO_DEBUG_SAVE_BBOXES_IMAGE_HH

/// \file
///
/// Draw a list of bounding boxes and their associated mass center.

# include <mln/core/concept/image.hh>
# include <mln/data/convert.hh>
# include <mln/value/rgb8.hh>
# include <mln/util/array.hh>
# include <mln/io/ppm/save.hh>
# include <mln/literal/colors.hh>

# include <scribo/core/line_set.hh>
# include <scribo/draw/bounding_boxes.hh>


namespace scribo
{

  namespace debug
  {

    using namespace mln;


    /// Draw a list of bounding boxes
    template <typename I>
    void
    save_bboxes_image(const Image<I>& input,
		      const mln::util::array< box<mln_site(I)> >& bboxes,
		      const std::string& filename,
		      const value::rgb8& value);


    template <typename I, typename L>
    void
    save_bboxes_image(const Image<I>& input,
		      const line_set<L>& lines,
		      const std::string& filename,
      		      const value::rgb8& value);

    /// \overload
    /// value is set to literal::red.
    //
    template <typename I, typename L>
    inline
    void
    save_bboxes_image(const Image<I>& input,
		      const line_set<L>& lines,
		      const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    void
    save_bboxes_image(const Image<I>& input,
		      const mln::util::array< box<mln_site(I)> >& bboxes,
		      const std::string& filename,
		      const value::rgb8& value)
    {
      trace::entering("scribo::debug::save_bboxes_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) tmp = data::convert(value::rgb8(), input);
      draw::bounding_boxes(tmp, bboxes, value);
      mln::io::ppm::save(tmp, filename);

      trace::exiting("scribo::debug::save_bboxes_image");
    }


    template <typename I, typename L>
    inline
    void
    save_bboxes_image(const Image<I>& input,
		      const line_set<L>& lines,
		      const std::string& filename,
      		      const value::rgb8& value)
    {
      trace::entering("scribo::debug::save_bboxes_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I, value::rgb8)
	output = data::convert(value::rgb8(), input);

      for_all_lines(l, lines)
	if (! lines(l).hidden())
	  mln::draw::box(output, lines(l).bbox(), value);

      mln::io::ppm::save(output, filename);
      trace::exiting("scribo::debug::save_bboxes_image");
    }


    template <typename I, typename L>
    inline
    void
    save_bboxes_image(const Image<I>& input,
		      const line_set<L>& lines,
		      const std::string& filename)
    {
      return save_bboxes_image(input, lines, filename, literal::red);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_SAVE_BBOXES_IMAGE_HH
