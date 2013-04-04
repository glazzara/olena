// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_LINEAR_LOG_HH
# define MLN_LINEAR_LOG_HH

/// \file
///
/// Laplacian of Gaussian.
/// \todo use doxygen to make reference to articles.


# include <mln/linear/convolve.hh>
# include <mln/make/w_window2d.hh>



namespace mln
{

  namespace linear
  {

    /*! \brief Laplacian of Gaussian.

      LoG_5x5  (Cf. Sonka et al., pages 85-86).
      This is also a "mexican hat".

      \ingroup mlnlinear
    */
    template <typename I>
    mln_ch_convolve(I, int)
    LoG_5x5(const Image<I>& input);

    /*! \brief Laplacian of Gaussian.

      LoG 7x7  (Cf. Russ, p. 250).

      \ingroup mlnlinear
    */
    template <typename I>
    mln_ch_convolve(I, int)
    LoG_7x7(const Image<I>& input);

    /*! \brief Laplacian of Gaussian.

      LoG 13x13  (Cf. Russ, p. 250).

      \ingroup mlnlinear
    */
    template <typename I>
    mln_ch_convolve(I, int)
    LoG_13x13(const Image<I>& input);

    /*! \brief Laplacian of Gaussian.

      LoG 17x17  (Cf. Sonka et al., pages 85-86).

      \ingroup mlnlinear
    */
    template <typename I>
    mln_ch_convolve(I, int)
    LoG_17x17(const Image<I>& input);

# ifndef MLN_INCLUDE_ONLY

    // LoG_5x5  (Cf. Sonka et al., pages 85-86)
    // This is also a "mexican hat".

    template <typename I>
    inline
    mln_ch_convolve(I, int)
    LoG_5x5(const Image<I>& input)
    {
      mln_trace("linear::LoG_5x5");
      mln_precondition(exact(input).is_valid());
      int ws[] = { +0,  0, -1,  0,  0,
		   +0, -1, -2, -1,  0,
		   -1, -2, 16, -2, -1,
		   +0, -1, -2, -1,  0,
		   +0,  0, -1,  0,  0 };
      mln_ch_convolve(I, int) output = convolve(input, make::w_window2d(ws));
      return output;
    }

    // LoG 7x7  (Cf. Russ, p. 250)

    template <typename I>
    inline
    mln_ch_convolve(I, int)
    LoG_7x7(const Image<I>& input)
    {
      mln_trace("linear::LoG_7x7");
      mln_precondition(exact(input).is_valid());
      int ws[] = { +0,  0, -1, -1, -1,  0,  0,
		   +0, -1, -3, -3, -3, -1,  0,
		   -1, -3,  0,  7,  0, -3, -1,
		   -1, -3,  7, 24,  7, -3, -1,
		   -1, -3,  0,  7,  0, -3, -1,
		   +0, -1, -3, -3, -3, -1,  0,
		   +0,  0, -1, -1, -1,  0,  0 };
      mln_ch_convolve(I, int) output = convolve(input, make::w_window2d(ws));
      return output;
    }

    // LoG 13x13  (Cf. Russ, p. 250)

    template <typename I>
    inline
    mln_ch_convolve(I, int)
    LoG_13x13(const Image<I>& input)
    {
      mln_trace("linear::LoG_13x13");
      mln_precondition(exact(input).is_valid());
      int ws[] = { +0,  0,  0,  0,  0, -1, -1, -1,  0,  0,  0,  0,  0,
		   +0,  0,  0, -1, -1, -2, -2, -2, -1, -1,  0,  0,  0,
		   +0,  0, -2, -2, -3, -3, -4, -3, -3, -2, -2,  0,  0,
		   +0, -1, -2, -3, -3, -3, -2, -3, -3, -3, -2, -1,  0,
		   +0, -1, -3, -3, -1,  4,  6,  4, -1, -3, -3, -1,  0,
		   -1, -2, -3, -3,  4, 14, 19, 14,  4, -3, -3, -2, -1,
		   -1, -2, -4, -2,  6, 19, 24, 19,  6, -2, -4, -2, -1,
		   -1, -2, -3, -3,  4, 14, 19, 14,  4, -3, -3, -2, -1,
		   +0, -1, -3, -3, -1,  4,  6,  4, -1, -3, -3, -1,  0,
		   +0, -1, -2, -3, -3, -3, -2, -3, -3, -3, -2, -1,  0,
		   +0,  0, -2, -2, -3, -3, -4, -3, -3, -2, -2,  0,  0,
		   +0,  0,  0, -1, -1, -2, -2, -2, -1, -1,  0,  0,  0,
		   +0,  0,  0,  0,  0, -1, -1, -1,  0,  0,  0,  0,  0 };
      mln_ch_convolve(I, int) output = convolve(input, make::w_window2d(ws));
      return output;
    }


    // LoG 17x17  (Cf. Sonka et al., pages 85-86)

    template <typename I>
    inline
    mln_ch_convolve(I, int)
    LoG_17x17(const Image<I>& input)
    {
      mln_trace("linear::LoG_17x17");
      mln_precondition(exact(input).is_valid());
      int ws[] = { +0, 0, 0, 0, 0, 0,-1,-1,-1,-1,-1, 0, 0, 0, 0, 0, 0,
		   +0, 0, 0, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0, 0, 0,
		   +0, 0,-1,-1,-1,-2,-3,-3,-3,-3,-3,-2,-1,-1,-1, 0, 0,
		   +0, 0,-1,-1,-2,-3,-3,-3,-3,-3,-3,-3,-2,-1,-1, 0, 0,
		   +0,-1,-1,-2,-3,-3,-3,-2,-3,-2,-3,-3,-3,-2,-1,-1, 0,
		   +0,-1,-2,-3,-3,-3, 0, 2, 4, 2, 0,-3,-3,-3,-2,-1, 0,
		   -1,-1,-3,-3,-3, 0, 4,10,12,10, 4, 0,-3,-3,-3,-1,-1,
		   -1,-1,-3,-3,-2, 2,10,18,21,18,10, 2,-2,-3,-3,-1,-1,
		   -1,-1,-3,-3,-3, 4,12,21,24,21,12, 4,-3,-3,-3,-1,-1,
		   -1,-1,-3,-3,-2, 2,10,18,21,18,10, 2,-2,-3,-3,-1,-1,
		   -1,-1,-3,-3,-3, 0, 4,10,12,10, 4, 0,-3,-3,-3,-1,-1,
		   +0,-1,-2,-3,-3,-3, 0, 2, 4, 2, 0,-3,-3,-3,-2,-1, 0,
		   +0,-1,-1,-2,-3,-3,-3,-2,-3,-2,-3,-3,-3,-2,-1,-1, 0,
		   +0, 0,-1,-1,-2,-3,-3,-3,-3,-3,-3,-3,-2,-1,-1, 0, 0,
		   +0, 0,-1,-1,-1,-2,-3,-3,-3,-3,-3,-2,-1,-1,-1, 0, 0,
		   +0, 0, 0, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0, 0, 0,
		   +0, 0, 0, 0, 0, 0,-1,-1,-1,-1,-1, 0, 0, 0, 0, 0, 0 };
      mln_ch_convolve(I, int) output = convolve(input, make::w_window2d(ws));
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_LOG_HH
