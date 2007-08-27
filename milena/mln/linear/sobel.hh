// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_LINEAR_SOBEL_HH
# define MLN_LINEAR_SOBEL_HH

/*! \file mln/linear/sobel.hh
 *
 * \brief Sobel filter.
 */

# include <mln/arith/plus.hh>
# include <mln/level/abs.hh>
# include <mln/linear/line_x2_convolve.hh>



namespace mln
{

  namespace linear
  {

    /*! Convolution of an image \p input by the weighted window \p w_win.
     *
     * \warning Computation of \p output(p) is performed with the
     * value type of \p output.
     *
     * \warning The weighted window is used as-is, considering that
     * its symmetrization is handled by the client.
     *
     * \pre output.domain = input.domain
     *
     * \todo Only for 2D so check + generalize.
     *
     * \todo Suboptimal since it is not point-wise and it costs temp
     * images.
     */
    template <typename I, typename O>
    void sobel_v(const Image<I>& input, Image<O>& output);

    template <typename I, typename O>
    void sobel_h(const Image<I>& input, Image<O>& output);

    template <typename I, typename O>
    void sobel(const Image<I>& input, Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename O>
      void sobel_h_(const Image<I>& input, Image<O>& output)
      {
	int wrow[] = { -1, 0, 1 }, wcol[] = { 1,
					      2,
					      1 };
	linear::line_x2_convolve(input, wrow, wcol, output);
      }

      template <typename I, typename O>
      void sobel_v_(const Image<I>& input, Image<O>& output)
      {
	int wrow[] = { 1, 2, 1 }, wcol[] = { -1,
					      0,
					     +1 };
	linear::line_x2_convolve(input, wrow, wcol, output);
      }

      template <typename I, typename O>
      void sobel_(const Image<I>& input, Image<O>& output)
      {
	// h
	sobel_h(input, output);
	// v
	O temp(exact(input).domain());
	sobel_v(input, temp);
	// output
	arith::plus_inplace(output, temp);
	level::abs_inplace(output);
      }

    } // end of namespace mln::linear::impl


    // Facades.

    template <typename I, typename O>
    void sobel_h(const Image<I>& input, Image<O>& output)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      impl::sobel_h_(exact(input), exact(output));
    }

    template <typename I, typename O>
    void sobel_v(const Image<I>& input, Image<O>& output)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      impl::sobel_v_(exact(input), exact(output));
    }

    template <typename I, typename O>
    void sobel(const Image<I>& input, Image<O>& output)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      impl::sobel_(exact(input), exact(output));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_SOBEL_HH
