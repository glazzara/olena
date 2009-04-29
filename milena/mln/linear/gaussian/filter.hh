// Copyright (C) 2001, 2002, 2003, 2004, 2008, 2009 EPITA Research and
// Laboratory (LRDE)
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef MLN_LINEAR_GAUSSIAN_FILTER_HH
# define MLN_LINEAR_GAUSSIAN_FILTER_HH

/// \file mln/linear/gaussian/filter.hh
///
/// Gaussian filter.
///
/// \todo Add a clean reference Rachid Deriche
///	 Recursively implementing the gaussian and its derivatives (1993)

# include <mln/core/concept/image.hh>
# include <mln/linear/gaussian/impl.hh>
# include <mln/linear/gaussian/internal/coefficients.hh>


namespace mln
{

  namespace linear
  {

    namespace gaussian
    {

      /// Gaussian filter of an image \p input
      ///
      /// \pre output.domain = input.domain
      ///
      template <typename I>
      mln_concrete(I)
      filter(const Image<I>& input, double sigma);


      template <typename I>
      mln_concrete(I)
      filter(const Image<I>& input, double sigma, int dir);



# ifndef MLN_INCLUDE_ONLY


      // Facades.

      /*! Apply an approximated gaussian filter of \p sigma on \p input.
       * on a specific direction \p dir
       * if \p dir = 0, the filter is applied on the first image dimension.
       * if \p dir = 1, the filter is applied on the second image dimension.
       * And so on...
       *
       * \pre input.is_valid
       * \pre dir < dimension(input)
       */
      template <typename I>
      inline
      mln_concrete(I)
      filter(const Image<I>& input, double sigma, int dir)
      {
	trace::entering("linear::gaussian::filter");

	mln_precondition(exact(input).is_valid());
	mln_precondition(dir < I::site::dim);

	mln_concrete(I) output;
	initialize(output, input);

	internal::coefficients coef = internal::coefficients_not_derivative(sigma);

	impl::generic_filter_common_(mln_trait_value_nature(mln_value(I))(),
				     input, coef, sigma, output, dir);

	trace::exiting("linear::gaussian::filter");
	return output;
      }




      /*! Apply an approximated gaussian filter of \p sigma on \p input.
       * This filter is applied in all the input image direction.
       *
       * \pre input.is_valid
       */
      template <typename I>
      inline
      mln_concrete(I)
      filter(const Image<I>& input, double sigma)
      {
	trace::entering("linear::gaussian::filter");

	mln_precondition(exact(input).is_valid());

	mln_concrete(I) output;
	initialize(output, input);

	internal::coefficients coef = internal::coefficients_not_derivative(sigma);

	impl::generic_filter_common_(mln_trait_value_nature(mln_value(I))(),
				     input, coef, sigma, output);

	trace::exiting("linear::gaussian::filter");
	return output;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::linear::gaussian

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_GAUSSIAN_FILTER_HH
