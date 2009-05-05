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

#ifndef MLN_WORLD_INTER_PIXEL_FULL_HH
# define MLN_WORLD_INTER_PIXEL_FULL_HH

/// \file mln/world/inter_pixel/full.hh
///
/// Convert a classical 2D image to an inter-pixel image.
///
/// FIXME: will NOT work if the image has an origin different from (0,0).

# include <mln/core/image/image2d.hh>
# include <mln/geom/max_col.hh>
# include <mln/geom/max_row.hh>
# include <mln/geom/min_col.hh>
# include <mln/geom/min_row.hh>


namespace mln
{

  namespace world
  {

    namespace inter_pixel
    {

      /// Convert a classical 2D image to an inter-pixel image.
      ///
      /// \param[in] input A 2d image.
      ///
      /// \return An inter-pixel image.
      //
      template <typename T>
      image2d<T>
      image2full(const image2d<T>& input);


# ifndef MLN_INCLUDE_ONLY


      template <typename T>
      image2d<T>
      image2full(const image2d<T>& input)
      {
	trace::entering("world::inter_pixel::image2full");
	mln_precondition(input.is_valid());

	image2d<T> output(2 * input.nrows() - 1, 2 * input.ncols() - 1);
	for (int row = geom::min_row(input); row <= geom::max_row(input); ++row)
	  for (int col = geom::min_col(input); col <= geom::max_col(input); ++col)
	    opt::at(output, 2 * row, 2 * col) = opt::at(input, row, col);

	trace::exiting("world::inter_pixel::image2full");
	return output;
      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::world::inter_pixel

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_INTER_PIXEL_FULL
