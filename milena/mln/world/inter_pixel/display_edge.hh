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

#ifndef MLN_WORLD_INTER_PIXEL_DISPLAY_EDGE_HH
# define MLN_WORLD_INTER_PIXEL_DISPLAY_EDGE_HH

/// \file
///
/// Create an image for visualizing the values on edges of an
/// inter_pixel image.

# include <mln/core/image/image2d.hh>
# include <mln/core/image/dmorph/image_if.hh>
# include <mln/data/fill.hh>
# include <mln/world/inter_pixel/is_separator.hh>
# include <mln/opt/at.hh>

namespace mln
{

  namespace world
  {

    namespace inter_pixel
    {

      /// \brief Create an image which is a copy of the input image with
      /// a solid background color and highlighted edges. The image is
      /// stretched according to the zoom ratio.
      ///
      /// \param[in] ima The input image providing edge values.
      /// \param[in] bg The background color.
      /// \param[in] zoom The zoom ratio.
      /// \return A copy of the input image with highlighted edges.
      ///
      /// \pre \p ima has to be an unmorphed image.
      ///
      template <typename I>
      inline
      I display_edge(const I& ima, mln_value(I) bg, unsigned zoom);

# ifndef MLN_INCLUDE_ONLY


      template <typename I>
      inline
      I display_edge(const I& ima, mln_value(I) bg, unsigned zoom)
      {
	box2d b = ima.bbox();
	I output(make::box2d(((b.pmin()[0] + 1) / 2) * (zoom + 1),
			     ((b.pmin()[1] + 1) / 2) * (zoom + 1),
			     ((b.pmax()[0] + 1) / 2 + 1) * (zoom + 1) - 2,
			     ((b.pmax()[1] + 1) / 2 + 1) * (zoom + 1) - 2));
	data::fill(output, bg);
	typedef image_if<const I, is_separator> edge_t;
	edge_t edge = ima | is_separator();
	mln_piter(edge_t) p(edge.domain());
	for_all(p)
	  if (p.row() % 2) // horizontal edge
	  {
	    unsigned row = (p.row() / 2 + 1) * (zoom + 1) - 1;
	    unsigned col = (p.col() / 2) * (zoom + 1);
	    for (unsigned i = 0; i < zoom; ++i)
	      opt::at(output, row, col + i) = ima(p);
	  }
	  else // vertical edge
	  {
	    unsigned row = (p.row() / 2) * (zoom + 1);
	    unsigned col = (p.col() / 2 + 1) * (zoom + 1) - 1;
	    for (unsigned i = 0; i < zoom; ++i)
	      opt::at(output, row + i, col) = ima(p);
	  }
	return output;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::inter_pixel

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_INTER_PIXEL_DISPLAY_EDGE_HH
