// Copyright (C) 2009, 2011, 2013 EPITA Research and Development
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

#ifndef SCRIBO_DEBUG_SAVE_TABLE_IMAGE_HH
# define SCRIBO_DEBUG_SAVE_TABLE_IMAGE_HH

/// \file
///
/// \brief Save table line bounding boxes in an image.

# include <string>

# include <mln/core/concept/image.hh>
# include <mln/core/image/image2d.hh>
# include <mln/data/fill.hh>
# include <mln/data/convert.hh>
# include <mln/util/array.hh>
# include <mln/util/couple.hh>
# include <mln/value/rgb8.hh>
# include <mln/io/ppm/save.hh>

# include <scribo/draw/bounding_boxes.hh>

namespace scribo
{

  namespace debug
  {

    using namespace mln;

    /// Save lines bounding boxes in a copy of \p input_.
    /// Bounding boxes are displayed with \p bbox_color.
    ///
    /// \ingroup grpalgodebug
    //
    template <typename I>
    void
    save_table_image(const Image<I>& input_,
		     mln::util::couple<mln::util::array<box<mln_site(I)> >,
				       mln::util::array<box<mln_site(I)> > > tableboxes,
		     const value::rgb8& bbox_color,
		     const std::string& filename);

    /// Save lines bounding boxes in an image defined on \p input_domain
    /// filled with \p bg_color.
    /// Bounding boxes are displayed with \p bbox_color.
    ///
    /// \ingroup grpalgodebug
    //
    template <typename S>
    void
    save_table_image(const Site_Set<S>& input_domain,
		     mln::util::couple<mln::util::array<box<mln_site(S)> >,
				       mln::util::array<box<mln_site(S)> > > tableboxes,
		     const value::rgb8& bg_color,
		     const value::rgb8& bbox_color,
		     const std::string& filename);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    void
    save_table_image(const Image<I>& input_,
		     mln::util::couple<mln::util::array<box<mln_site(I)> >,
				       mln::util::array<box<mln_site(I)> > > tableboxes,
		     const value::rgb8& bbox_color,
		     const std::string& filename)
    {
      trace::entering("scribo::debug::save_table_image");
//      mlc_converts_to(mln_value(I), value::rgb8)::check();
      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      mln_ch_value(I, value::rgb8) out2 = data::convert(value::rgb8(), input);
      draw::bounding_boxes(out2, tableboxes.first(), bbox_color);
      draw::bounding_boxes(out2, tableboxes.second(), bbox_color);
      io::ppm::save(out2, filename);

      trace::exiting("scribo::debug::save_table_image");
    }


    template <typename S>
    void
    save_table_image(const Site_Set<S>& input_domain_,
		     mln::util::couple<mln::util::array<box<mln_site(S)> >,
				       mln::util::array<box<mln_site(S)> > > tableboxes,
		     const value::rgb8& bg_color,
		     const value::rgb8& bbox_color,
		     const std::string& filename)
    {
      trace::entering("scribo::debug::save_table_image");
      const S& input_domain = exact(input_domain_);
      mln_precondition(input_domain.is_valid());

      image2d<value::rgb8> out2(input_domain);
      data::fill(out2, bg_color);
      draw::bounding_boxes(out2, tableboxes.first(), bbox_color);
      draw::bounding_boxes(out2, tableboxes.second(), bbox_color);
      io::ppm::save(out2, filename);

      trace::exiting("scribo::debug::save_table_image");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_SAVE_TABLE_IMAGE_HH
