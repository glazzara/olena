// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_DEBUG_SAVE_TABLE_IMAGE_HH
# define SCRIBO_DEBUG_SAVE_TABLE_IMAGE_HH

/// \file scribo/debug/save_table_image.hh
///
/// Save table line bounding boxes in an image.

# include <string>

# include <mln/core/concept/image.hh>
# include <mln/data/fill.hh>
# include <mln/util/array.hh>
# include <mln/util/couple.hh>
# include <mln/value/rgb8.hh>
# include <mln/io/ppm/save.hh>

# include <scribo/draw/bounding_boxes.hh>

namespace scribo
{

  namespace debug
  {

    /// Save lines bounding boxes in an image filled with \p bg_color.
    /// Bounding boxes are displayed with \p bbox_color.
    template <typename I>
    void
    save_table(const Image<I>& input_,
	       util::couple<util::array<box<mln_site(I)> >,
			    util::array<box<mln_site(I)> > > tableboxes,
	       const value::rgb8& bg_color,
	       const value::rgb8& bbox_color,
	       const std::string& filename);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    void
    save_table(const Image<I>& input_,
	       util::couple<util::array<box<mln_site(I)> >,
			    util::array<box<mln_site(I)> > > tableboxes,
	       const value::rgb8& bg_color,
	       const value::rgb8& bbox_color,
	       const std::string& filename)
    {
      trace::entering("scribo::debug::save_table_image");
      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      mln_ch_value(I,value::rgb8) out2(exact(input).domain());
      data::fill(out2, bg_color);
      draw::bounding_boxes(out2, tableboxes.first(), bbox_color);
      draw::bounding_boxes(out2, tableboxes.second(), bbox_color);
      io::ppm::save(out2, filename);

      trace::exiting("scribo::internal::save_table");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_SAVE_TABLE_IMAGE_HH
