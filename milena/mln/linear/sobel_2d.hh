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

#ifndef MLN_LINEAR_SOBEL_2D_HH
# define MLN_LINEAR_SOBEL_2D_HH

/// \file
///
/// Sobel_2d filter.
/// \todo Extends to the case of other dimensions (only the 2-d case
/// is handled here).

# include <mln/trait/ch_value.hh>
# include <mln/trait/value/nature.hh>

# include <mln/metal/int.hh>

# include <mln/arith/plus.hh>
# include <mln/data/abs.hh>
# include <mln/fun/x2v/l1_norm.hh>
# include <mln/fun/vv2v/vec.hh>
# include <mln/linear/convolve_2x1d.hh>



namespace mln
{

  namespace linear
  {

    /*! \brief Sobel_2d gradient components.
      \ingroup mlnlinear
    */
    /// \{
    /// \brief Compute the horizontal component of the 2D Sobel gradient.
    template <typename I>
    mln_ch_convolve(I, int)
    sobel_2d_h(const Image<I>& input);

    /// \brief Compute the vertical component of the 2D Sobel gradient.
    template <typename I>
    mln_ch_convolve(I, int)
    sobel_2d_v(const Image<I>& input);

    /// \brief Compute the vertical component of the 2D Sobel gradient.
    template <typename I>
    mln_ch_convolve_grad(I, int)
    sobel_2d(const Image<I>& input);

    /// \brief Compute the L1 norm of the 2D Sobel gradient.
    template <typename I>
    mln_ch_convolve(I, int)
    sobel_2d_l1_norm(const Image<I>& input);
    /// \}


# ifndef MLN_INCLUDE_ONLY


    // Facades.

    template <typename I>
    inline
    mln_ch_convolve(I, int)
    sobel_2d_h(const Image<I>& input)
    {
      mln_trace("linear::sobel_2d_h");
      mln_precondition(exact(input).is_valid());

      int wh[] = { -1, 0, 1 };
      int wv[] = { 1,
		   2,
		   1 };
      mln_ch_convolve(I, int) output = convolve_2x1d(input, wh, wv);

      return output;
    }


    template <typename I>
    inline
    mln_ch_convolve(I, int)
    sobel_2d_v(const Image<I>& input)
    {
      mln_trace("linear::sobel_2d_v");
      mln_precondition(exact(input).is_valid());

      int wh[] = { 1, 2, 1 };
      int wv[] = { -1,
		    0,
		   +1 };
      mln_ch_convolve(I, int) output = convolve_2x1d(input, wh, wv);

      return output;
    }


    template <typename I>
    mln_ch_convolve_grad(I, int)
    sobel_2d(const Image<I>& input)
    {
      mln_trace("linear::sobel_2d");
      mln_precondition(exact(input).is_valid());

      typedef mln_ch_convolve(I, int) J;
      J h = sobel_2d_h(input),
	v = sobel_2d_v(input);
      fun::vv2v::vec<mln_value(J)> f;
      mln_ch_convolve_grad(I, int) output = data::transform(h, v, f);

      return output;
    }


    template <typename I>
    mln_ch_convolve(I, int)
    sobel_2d_l1_norm(const Image<I>& input)
    {
      mln_trace("linear::sobel_2d_norm_l1");
      mln_precondition(exact(input).is_valid());

      typedef mln_ch_convolve_grad(I, int) G;
      G grad = sobel_2d(input);
      fun::x2v::l1_norm<mln_value(G)> f;
      mln_ch_convolve(I, int) output = data::transform(grad, f);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_SOBEL_2D_HH
