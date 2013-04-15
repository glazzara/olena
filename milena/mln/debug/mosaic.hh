// Copyright (C) 2010, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_DEBUG_MOSAIC_HH
# define MLN_DEBUG_MOSAIC_HH

/// \file
///
/// Create a single image from an array of image.

# include <cmath>

# include <mln/core/image/image2d.hh>

# include <mln/core/image/image3d.hh>
# include <mln/core/image/dmorph/slice_image.hh>

# include <mln/core/image/dmorph/p2p_image.hh>
# include <mln/fun/p2p/translation.hh>

# include <mln/data/paste.hh>
# include <mln/data/fill.hh>



namespace mln
{

  namespace debug
  {

    /*! \brief Create a single image from an array of image.

      The size of the output image is defined by:

      width = n_horizontal * max(input[i].ncols())
      height = (input.size() / n_horizontal) * max(input[i].nrows())

      \return a single image where all the input images are displayed
      as a mosaic.

      \ingroup mlndebug
     */
    template <typename I>
    mln_concrete(I)
    mosaic(const util::array<I>& input,
	   unsigned n_horizontal,
	   const mln_value(I)& bg);



# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    mln_concrete(I)
    mosaic(const util::array<I>& input,
	   unsigned n_horizontal,
	   const mln_value(I)& bg)
    {
      mln_trace("debug::mosaic");

      mlc_is_a(I, Image)::check();
      mlc_equal(mln_domain(I), box2d)::check();
      mln_precondition(n_horizontal > 0);

      // Find largest dimensions
      unsigned max_ncols = 0, max_nrows = 0;
      for (unsigned i = 0; i < input.nelements(); ++i)
      {
	const I& ima = exact(input[i]);
	mln_precondition(ima.is_valid());

	if (ima.ncols() > max_ncols)
	  max_ncols = ima.ncols();

	if (ima.nrows() > max_nrows)
	  max_nrows = ima.nrows();
      }


      mln_concrete(I) output(max_nrows
			     * (input.nelements() / (float)n_horizontal
				+ 0.51f),
			     max_ncols * n_horizontal);
      data::fill(output, bg);

      unsigned brow = 0, bcol = 0;
      for (unsigned i = 0; i < input.nelements(); ++i)
      {
	const I& ima = exact(input[i]);
	mln_precondition(ima.is_valid());

	dpoint2d dp(brow * max_nrows
		    + ((max_nrows - ima.domain().height()) / 2),
		    bcol * max_ncols
		    + ((max_ncols - ima.domain().width()) / 2));
	data::paste(apply_p2p(ima, fun::p2p::translation(dp)),
		    output);

	++bcol;
	if (! (bcol < n_horizontal))
	{
	  ++brow;
	  bcol = 0;
	}
      }

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_MOSAIC_HH
