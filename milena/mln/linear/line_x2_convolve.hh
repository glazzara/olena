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

#ifndef MLN_LINEAR_LINE_X2_CONVOLVE_HH
# define MLN_LINEAR_LINE_X2_CONVOLVE_HH

/*! \file mln/linear/line_x2_convolve.hh
 *
 * \brief 2D convolution by a couple of line kernels.
 */

# include <mln/linear/line_convolve.hh>
# include <mln/core/t_image.hh>



namespace mln
{

  namespace linear
  {

    /*! Convolution of an image \p input by two weighted line-shapes
     *  windows.
     *
     * \warning Computation of \p output(p) is performed with the
     * value type of \p output.
     *
     * \warning The weighted window is used as-is, considering that
     * its symmetrization is handled by the client.
     *
     * \pre output.domain = input.domain
     */
    template <typename I,
	      typename W, unsigned Nr, unsigned Nc,
	      typename O>
    void line_x2_convolve(const Image<I>& input,
			  W (&row_weights)[Nr], W (&col_weights)[Nc],
			  Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

    template <typename I,
	      typename W, unsigned Nr, unsigned Nc,
	      typename O>
    inline
    void line_x2_convolve(const Image<I>& input,
			  W (&row_weights)[Nr], W (&col_weights)[Nc],
			  Image<O>& output)
    {
      // FIXME: Check 2D.
      mln_precondition(exact(output).domain() == exact(input).domain());

      O tmp(exact(output).domain());
      linear::line_convolve(input, row_weights, tmp);

      t_image<O> swap_output = swap_coords(output, 0, 1);
      linear::line_convolve(swap_coords(tmp, 0, 1), col_weights, swap_output);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_LINE_X2_CONVOLVE_HH
