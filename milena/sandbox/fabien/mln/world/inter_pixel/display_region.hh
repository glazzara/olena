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

#ifndef MLN_WORLD_INTER_PIXEL_DISPLAY_REGION_HH
# define MLN_WORLD_INTER_PIXEL_DISPLAY_REGION_HH

/// \file
///
/// Create an image for visualizing the different regions of a labeled
/// inter_pixel image.

# include <mln/core/image/image2d.hh>
# include <mln/core/image/dmorph/image_if.hh>
# include <mln/data/paste.hh>
# include <mln/world/inter_pixel/is_pixel.hh>
# include <mln/world/inter_pixel/is_separator.hh>
# include <mln/opt/at.hh>
# include <mln/value/rgb8.hh>

# include "is_zero_face.hh"


namespace mln
{

  namespace world
  {

    namespace inter_pixel
    {

      /// \brief FIXME
      ///
      /// \param[in] input The input image providing pixels values.
      /// \param[in] wst The labeling image providing the watershed lines.
      /// \param[in] edge_color The color (in rgb8) for the watershed line.
      /// \return FIXME
      ///
      /// \pre \p wst has to be an unmorphed image.
      /// \warning This implementation only works for 2D images.
      ///
      template <typename I, typename L>
      inline
      mln_ch_value(I, value::rgb8)
      display_region(const I& input, const mln_ch_value(I, L)& wst, value::rbg8 edge_color);


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      mln_ch_value(I, value::rgb8)
      display_region(const I& input, const mln_ch_value(I, L)& wst, value::rbg8 edge_color)
      {
	mln_precondition((2 * input.bbox()) == wst.unmorph_().bbox());

	mln_ch_value(I, value::rgb8) output;
	initialize(output, wst);

	// Pixel.
	mln_piter(I) p_input(input.domain());
	for_all(p)
	  opt::at(output, p.to_vec[0] * 2, p.to_vec[1] * 2) = input(p);

	// Separator.
	typedef image_if<const mln_ch_value(I, L)&, is_separator> separator_t;
	separator_t edge = wst | is_separator();
	mln_piter(edge_t) p(edge.domain());
	for_all(p)
	{
	  if (p.row() % 2) // horizontal edge
	  {
	    unsigned row = (p.row() / 2 + 1) * 2 - 1;
	    unsigned col = p.col();
	      opt::at(output, row, col) = input(p);
	  }
	  else // vertical edge
	  {
	    unsigned row = p.row();
	    unsigned col = (p.col() / 2 + 1) * 2 - 1;
	      opt::at(output, row, col) = input(p);
	  }
	}

	// Point (dimension 0).
	typedef image_if<const mln_ch_value(I, L)&, is_zero_face> zero_face_t;
	zero_face_t edge = wst | is_zero_face();
	mln_piter(edge_t) p(edge.domain());
	for_all(p)
	{
	  if (p.row() % 2) // horizontal edge
	  {
	    unsigned row = (p.row() / 2 + 1) * 2 - 1;
	    unsigned col = p.col();
	      opt::at(output, row, col) = input(p);
	  }
	  else // vertical edge
	  {
	    unsigned row = p.row();
	    unsigned col = (p.col() / 2 + 1) * 2 - 1;
	      opt::at(output, row, col) = input(p);
	  }
	}

	return output;

# endif // ! MLN_INCLUDE_ONLY

      }

    } // end of namespace mln::world::inter_pixel

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_INTER_PIXEL_DISPLAY_REGION_HH

