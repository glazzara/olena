// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_WORLD_INTER_PIXEL_DISPLAY_EDGE_HH
# define MLN_WORLD_INTER_PIXEL_DISPLAY_EDGE_HH

/// \file mln/world/inter_pixel/display_edge.hh
///
/// FIXME: insert comment.

# include <mln/core/image/image2d.hh>
# include <mln/core/image/image_if.hh>
# include <mln/data/fill.hh>
# include <mln/world/inter_pixel/dim2/is_edge.hh>
# include <mln/opt/at.hh>

namespace mln
{

  namespace world
  {

    namespace inter_pixel
    {

      template <typename I>
      inline
      I display_edge(const I& ima, mln_value(I) bg, unsigned zoom)
      {
	box2d b = ima.bbox();
	I output(make::box2d((b.pmin()[0] / 2) * (zoom + 1) + 1, (b.pmin()[1] / 2) * (zoom + 1) + 1,
			     (b.pmax()[0] / 2) * (zoom + 1) - 1, (b.pmax()[1] / 2) * (zoom + 1) - 1));
	data::fill(output, bg);
	typedef image_if<const I, dim2::is_edge> edge_t;
	edge_t edge = ima | dim2::is_edge();
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

    } // end of namespace mln::world::inter_pixel

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_INTER_PIXEL_DISPLAY_EDGE
