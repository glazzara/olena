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
/// FIXME: insert comment.

# include <mln/core/image/image2d.hh>

namespace mln
{

  namespace world
  {

    namespace inter_pixel
    {

      template <typename T>
      image2d<T>
      full2image(const image2d<T>& input);


# ifndef MLN_INCLUDE_ONLY

      template <typename T>
      image2d<T>
      full2image(const image2d<T>& input)
      {
	image2d<T> output((input.nrows() + 1) / 2,
	    (input.ncols() + 1) / 2);
	for (int row = 0; row < input.nrows(); row += 2)
	  for (int col = 0; col < input.ncols(); col += 2)
	    opt::at(output, row / 2, col / 2) =
	      opt::at(input, row, col);
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::world::inter_pixel

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_INTER_PIXEL_FULL
