// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
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

#ifndef MLN_LINEAR_GAUSSIAN_1D_HH
# define MLN_LINEAR_GAUSSIAN_1D_HH

/// \file
///
/// Gaussian filter for fastest 1D images.
///
/// \todo This is experimental code to be merged with regular code...

#include <mln/core/image/image1d.hh>
#include <mln/linear/gaussian_directional_2d.hh>



namespace mln
{

  namespace linear
  {

    /*! \brief Gaussian filter for fastest 1D images.
      \ingroup mlnlinear
     */
    template <typename I>
    mln_concrete(I)
    gaussian_1d(const Image<I>& input,
		double sigma,
		const mln_value(I)& bdr);



# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    mln_concrete(I)
    gaussian_1d(const Image<I>& input_,
		double sigma,
		const mln_value(I)& bdr)
    {
      mln_trace("linear::gaussian_1d");

      typedef mln_site(I) P;
      mlc_bool(P::dim == 1)::check();

      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      my::recursivefilter_coef_ coef(1.68f, 3.735f,
				     1.783f, 1.723f,
				     -0.6803f, -0.2598f,
				     0.6318f, 1.997f,
				     sigma,
				     my::recursivefilter_coef_::DericheGaussian);

      extension::adjust_fill(input, 5 * int(sigma + .50001) + 1, bdr);
      mln_concrete(I) output = duplicate(input);

      if (sigma < 0.006)
	return output;

      int
	ninds = geom::ninds(input),
	b     = input.border();

      recursivefilter_directional_fastest(output, coef,
					  point1d(- b),
					  point1d(ninds - 1 + b),
					  ninds + 2 * b,
					  dpoint1d(1),
					  bdr);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_GAUSSIAN_1D_HH
