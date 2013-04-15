// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_LINEAR_CONVOLVE_2X1D_HH
# define MLN_LINEAR_CONVOLVE_2X1D_HH

/// \file
///
/// 2D convolution by a couple of line kernels.

# include <mln/linear/convolve_directional.hh>
# include <mln/extension/adjust.hh>
# include <mln/util/max.hh>



namespace mln
{

  namespace linear
  {

    /*! \brief Convolution of an image \p input by two weighted line-shapes
        windows.

       \warning The weighted window is used as-is, considering that
       its symmetrization is handled by the client.

       \pre input.is_valid

       \ingroup mlnlinear
     */
    template <typename I,
	      typename W, unsigned Sh, unsigned Sv>
    mln_ch_convolve(I, W)
    convolve_2x1d(const Image<I>& input,
		  W (&horizontal_weights)[Sh],
		  W (&  vertical_weights)[Sv]);


# ifndef MLN_INCLUDE_ONLY

    template <typename I,
	      typename W, unsigned Sh, unsigned Sv>
    mln_ch_convolve(I, W)
    convolve_2x1d(const Image<I>& input,
		  W (&horizontal_weights)[Sh],
		  W (&  vertical_weights)[Sv])
    {
      mln_trace("linear::convolve_2x1d");

      mlc_bool(Sh % 2 == 1)::check();
      mlc_bool(Sv % 2 == 1)::check();

      mln_precondition(exact(input).is_valid());

      extension::adjust(input, util::max(Sh / 2, Sv / 2));

      mln_ch_convolve(I, W) tmp, output;
      // Horizontal kernel so columns (index 1) are varying.
      tmp    = linear::convolve_directional(input, 1, horizontal_weights);
      output = linear::convolve_directional(  tmp, 0,   vertical_weights);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_CONVOLVE_2X1D_HH
