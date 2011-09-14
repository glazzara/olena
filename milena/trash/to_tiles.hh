// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CONVERT_TO_TILES_HH
# define MLN_CONVERT_TO_TILES_HH

/*! \file mln/convert/to_tiles.hh
 *
 * \brief Conversion of a vector of image2d into a tiled image with ratio.
 */

# include <mln/core/image/translate_image.hh>
# include <mln/data/paste.hh>
# include <mln/geom/nrows.hh>
# include <mln/geom/ncols.hh>

# include <vector>

namespace mln
{

  namespace convert
  {

    /// FIXME : Run only for image2d
    /// Convert a vector of image2d into a tiled image with ratio.
    template <typename I>
    I to_tiles(std::vector<I>& v_ima, float ratio);

# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    I
    to_tiles(std::vector<I>& v_ima, float ratio)
    {
      // Test if v_ima is not empty.
      mln_precondition(! v_ima.empty());

      unsigned size  = v_ima.size();
      unsigned nrows = geom::nrows(v_ima[0]);
      unsigned ncols = geom::ncols(v_ima[0]);

      // Compute output size.
      int size_c = (int) ceil(sqrt(size * ratio));
      int size_r = (int) ceil(sqrt(size / ratio));

      I output(size_r * nrows, size_c * ncols);

      for (unsigned i = 0; i < size; ++i)
	{
	  /// Check if current image has good dimension.
	  mln_precondition(geom::nrows(v_ima[i]) == nrows);
	  mln_precondition(geom::ncols(v_ima[i]) == ncols);

	  /// Compute the delta point of translation.
	  dpoint2d dp = dpoint2d(i / size_c * nrows, i % size_c * ncols);

	  /// Convert current image in translate image with its delta point.
	  translate_image<I> tr_ima(v_ima[i], dp);

	  /// Paste translated image into output.
	  data::paste(tr_ima, output);
	}

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_TO_TILES_HH
