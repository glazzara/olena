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

# include <mln/core/concept/image.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/image/dmorph/image_if.hh>
# include <mln/core/routine/extend.hh>
# include <mln/convert/from_to.hh>
# include <mln/data/paste.hh>
# include <mln/world/inter_pixel/is_pixel.hh>
# include <mln/world/inter_pixel/is_separator.hh>
# include <mln/world/inter_pixel/is_zero_face.hh>
# include <mln/opt/at.hh>
# include <mln/value/rgb8.hh>


namespace mln
{

  namespace world
  {

    namespace inter_pixel
    {

      /// \brief Create an image which is a copy of the input image (but
      /// twice the size) with highlighted watershed lines.
      ///
      /// \param[in] input The input image providing pixels values.
      /// \param[in] wst The labeling image providing the watershed lines.
      /// \param[in] edge_color The color (in rgb8) for the watershed line.
      /// \return A copy of the input image (but twice the size) with
      /// highlighted watershed lines.
      ///
      /// \pre \p input has to be an 8 bits image.
      /// \pre \p wst has to be an unmorphed image.
      /// \warning This implementation only works for 2D images.
      ///
      template <typename I, typename L>
      inline
      mln_ch_value(I, value::rgb8)
      display_region(const Image<I>& input, const Image<L>& wst, value::rgb8 edge_color);


# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename L>
      inline
      mln_ch_value(I, value::rgb8)
      display_region(const Image<I>& input_, const Image<L>& wst_, value::rgb8 edge_color)
      {
	trace::entering("world::inter_pixel::display_region");
	// TODO: We should check that wst.bbox() == 2 * input.bbox() - 1.
	// TODO: We must check that dim(I) == dim(L).
	//mln_precondition((2 * input.bbox()) == wst.bbox());

	const I& input = exact(input_);
	const L& wst = exact(wst_);

	typedef mln_ch_value(I, value::rgb8) output_t;
	output_t output;
	initialize(output, wst);

	// Pixel.
	mln_piter(I) p_input(input.domain());
	for_all(p_input)
	  convert::from_to(input(p_input), opt::at(output, p_input.row() * 2, p_input.col() * 2));

	// Separator.
	typedef image_if<output_t, is_separator> separator_t;
	separator_t sep = output | is_separator();
	mln_piter(separator_t) p(sep.domain());
	for_all(p)
	{
	  unsigned row = p.row();
	  unsigned col = p.col();
	  if (p.row() % 2) // horizontal edge
	    output(p) = (opt::at(output, row + 1, col) +
			 opt::at(output, row - 1, col)) / 2;
	  else // vertical edge
	    output(p) = (opt::at(output, row, col + 1) +
			 opt::at(output, row, col - 1)) / 2;
	}

	// Zero-face (dimension 0).
	typedef image_if<output_t, is_zero_face> zero_face_t;
	zero_face_t zero = output | is_zero_face();
	mln_piter(zero_face_t) q(zero.domain());
	mln_niter(neighb2d) n(c4(), q);
	for_all(q)
	{
	  bool has_wst = false;
	  for_all(n)
	  {
	    if (!has_wst && wst(n) == 0)
	    {
	      has_wst = true;
	      output(q) = edge_color;
	    }
	  }

	  if (!has_wst)
	  {
	    unsigned row = q.row();
	    unsigned col = q.col();
	    output(q) = (opt::at(output, row + 1, col) +
			 opt::at(output, row, col + 1) +
			 opt::at(output, row - 1, col) +
			 opt::at(output, row, col - 1)) / 4;
	  }
	}

	// Watershed lines.
	for_all(p)
	  if (wst(p) == 0)
	    output(p) = edge_color;

	trace::exiting("world::inter_pixel::display_region");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::world::inter_pixel

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_INTER_PIXEL_DISPLAY_REGION_HH

