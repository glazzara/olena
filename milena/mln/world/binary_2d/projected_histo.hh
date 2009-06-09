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

#ifndef MLN_WORLD_BINARY_2D_PROJECTED_HISTO_HH
# define MLN_WORLD_BINARY_2D_PROJECTED_HISTO_HH

/// \file
///
/// FIXME: insert comment.

# include <mln/core/concept/image.hh>
# include <mln/core/image/image1d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/alias/dpoint2d.hh>
# include <mln/geom/all.hh>
# include <mln/opt/at.hh>
# include <mln/value/int_u12.hh>
# include <mln/util/couple.hh>

namespace mln
{

  namespace world
  {

    namespace binary_2d
    {

      template <typename I>
      util::couple<image1d<float>, image1d<float> >
      projected_histo(const Image<I>& input_, bool value = true)
      {
	mln_ch_value(I, bool) input = exact(input_);

	const unsigned nrows = geom::nrows(input);
	image1d<float> row_histo(nrows);
	data::fill(row_histo, 0);

	const unsigned ncols = geom::ncols(input);
	image1d<float> col_histo(ncols);
	data::fill(col_histo, 0);

	for (unsigned row = 0; row < nrows; ++row)
	{
	  for (unsigned col = 0; col < ncols; ++col)
	  {
	    if (opt::at(input, geom::bbox(input).min_row() + row, geom::bbox(input).min_col() + col) == value)
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
