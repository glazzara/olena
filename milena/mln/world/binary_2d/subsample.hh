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

#ifndef MLN_WORLD_BINARY_2D_SUBSAMPLE_HH
# define MLN_WORLD_BINARY_2D_SUBSAMPLE_HH

/// \file mln/world/binary_2d/subsample.hh
///
/// FIXME: insert comment.

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/dpoint2d.hh>
# include <mln/value/int_u8.hh>

namespace mln
{

  namespace world
  {

    namespace binary_2d
    {

      image2d<value::int_u8> subsample(image2d<bool>& input, unsigned n)
      {
	using value::int_u8;

	const bool** ptr = new const bool*[n];
	const unsigned nrows = input.nrows() / n;
	const unsigned ncols = input.ncols() / n;
	algebra::vec<2, unsigned int> vmin;
	algebra::vec<2, unsigned int> vmax;
	vmin[0] = 0;
	vmin[1] = 0;
	vmax[0] = nrows - 1;
	vmax[1] = ncols - 1;
	point2d pmin(vmin);
	point2d pmax(vmax);
	image2d<int_u8> output(box<point2d>(pmin, pmax));

	dpoint2d dp_row(1, 0);
	const unsigned delta_row = input.delta_index(dp_row);
	unsigned count = 0;

	for (unsigned row = 0; row < nrows; ++row)
	{
	  ptr[0] = & input(point2d(n * row, 0));
	  for (unsigned i = 1; i < n; ++i)
	    ptr[i] = ptr[i - 1] + delta_row;
	  for (unsigned col = 0; col < ncols; ++col)
	  {
	    count = 0;
	    for (unsigned i = 0; i < n; ++i)
	    {
	      for (unsigned j = 0; j < n; ++j, ++(ptr[i]))
	      {
		if (*(ptr[i]))
		  ++count;
	      }
	    }
	    output(point2d(row, col)) = count * 255 / n / n;
	  }
	}

	return output;
      }

    } // end of namespace mln::world::binary_2d

  } // end of namspace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_BINARY_2D_SUBSAMPLE_HH
