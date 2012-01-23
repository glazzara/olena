// Copyright (C) 2008, 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_DEBUG_SLICES_2D_HH
# define MLN_DEBUG_SLICES_2D_HH

/// \file
///
/// Create a 2D image of the slices of the 3D image \p input.

/// \todo Does it do the same job as debug::mosaic ?

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

    /*! \brief Create a 2D image of the slices of the 3D image \p
      input.
      \ingroup mlndebug
     */
    template <typename I>
    image2d<mln_value(I)>
    slices_2d(const Image<I>& input,
	      unsigned n_horizontal, unsigned n_vertical,
	      const mln_value(I)& bg);


    /*! \brief Create a 2D image of the slices of the 3D image \p
        input.
	\ingroup mlndebug
     */
    template <typename I>
    image2d<mln_value(I)>
    slices_2d(const Image<I>& input,
	      float ratio_hv,           // horizontal / vertical
	      const mln_value(I)& bg);



# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    image2d<mln_value(I)>
    slices_2d(const Image<I>& input_,
	      unsigned n_horizontal, unsigned n_vertical,
	      const mln_value(I)& bg)
    {
      trace::entering("debug::slices_2d");
      mlc_equal(mln_domain(I), box3d)::check();

      const I& input = exact(input_);

      mln_precondition(input.is_valid());
      mln_precondition(n_horizontal > 0 && n_vertical > 0);
      mln_precondition(input.nslis() <= n_horizontal * n_vertical);

      image2d<mln_value(I)> output(input.nrows() * n_vertical,
				   input.ncols() * n_horizontal);
      if (input.nslis() != n_horizontal * n_vertical)
	data::fill(output, bg);

      const point3d& p_min = input.domain().pmin();
      def::coord
	sli = p_min.sli(),
	last_sli = input.domain().pmax().sli();
      for (unsigned i = 0; i < n_vertical; ++i)
	for (unsigned j = 0; j < n_horizontal; ++j)
	  {
	    dpoint2d dp(i * input.nrows() - p_min.row(),
			j * input.ncols() - p_min.col());
	    data::paste(apply_p2p(slice(input, sli),
				  fun::p2p::translation(dp)),
			output);
	    if (++sli > last_sli)
	      {
		// Go out of loops.
		i = n_vertical;
		j = n_horizontal;
		break;
	      }
	  }

      trace::exiting("debug::slices_2d");
      return output;
    }


    namespace internal
    {

      inline
      unsigned round_up(float f)
      {
	unsigned n = static_cast<unsigned>(f + 0.499999f);
	if (n == 0u)
	  ++n;
	if (float(n) < f)
	  ++n;
	return n;
      }

      inline
      void slices2d_helper(float nslis, float nrows, float ncols,
			   float ratio_hv,
			   unsigned& n_horizontal,
			   unsigned& n_vertical)
      {
	if (ratio_hv > 1.f)
	  {
	    float n_v = std::sqrt(nslis * ncols / ratio_hv / nrows);
	    n_vertical = internal::round_up(n_v);
	    float n_h = nslis / float(n_vertical);
	    n_horizontal = internal::round_up(n_h);
	  }
	else
	  {
	    float n_h = std::sqrt(nrows * nslis * ratio_hv / ncols);
	    n_horizontal = internal::round_up(n_h);
	    float n_v = nslis / float(n_horizontal);
	    n_vertical = internal::round_up(n_v);
	  }
      }

    } // end of namespace mln::debug::internal


    template <typename I>
    image2d<mln_value(I)>
    slices_2d(const Image<I>& input_,
	      float ratio_hv,           // horizontal / vertical
	      const mln_value(I)& bg)
    {
      trace::entering("debug::slices_2d");
      mlc_equal(mln_domain(I), box3d)::check();

      const I& input = exact(input_);
      mln_precondition(input.is_valid());
      mln_precondition(ratio_hv > 0.f);

      unsigned n_horizontal, n_vertical;
      internal::slices2d_helper(input.nslis(), input.nrows(), input.ncols(),
				ratio_hv,
				n_horizontal, n_vertical);
      mln_assertion(n_horizontal * n_vertical >= input.nslis());

      image2d<mln_value(I)> output = slices_2d(input, n_horizontal, n_vertical, bg);

      trace::exiting("debug::slices_2d");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_SLICES_2D_HH
