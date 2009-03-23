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

#ifndef MLN_WORLD_BINARY_2D_PROJECTED_HISTO_HH
# define MLN_WORLD_BINARY_2D_PROJECTED_HISTO_HH

/// \file mln/world/binary_2d/projected_histo.hh
///
/// FIXME: insert comment.

# include <mln/core/image/image1d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/alias/dpoint2d.hh>
# include <mln/value/int_u12.hh>
# include <mln/util/couple.hh>

namespace mln
{

  namespace world
  {

    namespace binary_2d
    {

      util::couple<image1d<float>, image1d<float> >
      projected_histo(const image2d<bool>& input, bool value = true)
      {
	const unsigned nrows = input.nrows();
	image1d<float> row_histo(nrows);
	data::fill(row_histo, 0);

	const unsigned ncols = input.ncols();
	image1d<float> col_histo(ncols);
	data::fill(col_histo, 0);

	mln_precondition(row_histo.nelements() == nrows);
	mln_precondition(col_histo.nelements() == ncols);

	for (unsigned row = 0; row < nrows; ++row)
	{
	  for (unsigned col = 0; col < ncols; ++col)
	  {
	    if (input.at_(row, col) == value)
	    {
	      ++row_histo.at_(row);
	      ++col_histo.at_(col);
	    }
	  }
	}

	for (unsigned i = 0; i < row_histo.ninds(); ++i)
	  row_histo.at_(i) = row_histo.at_(i) * 100 / ncols;

	for (unsigned i = 0; i < col_histo.ninds(); ++i)
	  col_histo.at_(i) = col_histo.at_(i) * 100 / nrows;

	return util::couple<image1d<float>, image1d<float> > (row_histo, col_histo);
      }

    } // end of namespace mln::world::binary_2d

  } // end of namspace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_BINARY_2D_PROJECTED_HISTO_HH
