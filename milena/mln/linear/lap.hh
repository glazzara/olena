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

#ifndef MLN_LINEAR_LAP_HH
# define MLN_LINEAR_LAP_HH

/*! \file
 *
 * \brief Laplacian.
 */

/// \todo Improve doc!

# include <mln/linear/convolve.hh>
# include <mln/make/w_window2d.hh>



namespace mln
{

  namespace linear
  {

    /*! \brief Laplacian
      \ingroup mlnlinear
    */
    /// \{
    template <typename I>
    mln_ch_convolve(I, int)
    lap_4(const Image<I>& input);

    template <typename I>
    mln_ch_convolve(I, int)
    lap_8(const Image<I>& input);

    template <typename I>
    mln_ch_convolve(I, int)
    lap_x(const Image<I>& input);

    template <typename I>
    mln_ch_convolve(I, int)
    lap_o(const Image<I>& input);
    /// \}

# ifndef MLN_INCLUDE_ONLY

    // Laplacian operators (Cf. Sonka et al., p. 81)

    template <typename I>
    inline
    mln_ch_convolve(I, int)
    lap_4(const Image<I>& input)
    {
      trace::entering("linear::lap_4");
      mln_precondition(exact(input).is_valid());
      int ws[] = { 0,  1,  0,
		   1, -4,  1,
		   0,  1,  0 };
      mln_ch_convolve(I, int) output = convolve(input, make::w_window2d(ws));
      trace::exiting("linear::lap_4");
      return output;
    }

    template <typename I>
    inline
    mln_ch_convolve(I, int)
    lap_8(const Image<I>& input)
    {
      trace::entering("linear::lap_8");
      mln_precondition(exact(input).is_valid());
      int ws[] = { 1,  1,  1,
		   1, -8,  1,
		   1,  1,  1 };
      mln_ch_convolve(I, int) output = convolve(input, make::w_window2d(ws));
      trace::exiting("linear::lap_8");
      return output;
    }

    template <typename I>
    inline
    mln_ch_convolve(I, int)
    lap_x(const Image<I>& input)
    {
      trace::entering("linear::lap_x");
      mln_precondition(exact(input).is_valid());
      int ws[] = { +2, -1, +2,
		   -1, -4, -1,
		   +2, -1, +2 };
      mln_ch_convolve(I, int) output = convolve(input, make::w_window2d(ws));
      trace::exiting("linear::lap_x");
      return output;
    }

    template <typename I>
    inline
    mln_ch_convolve(I, int)
    lap_o(const Image<I>& input)
    {
      trace::entering("linear::lap_o");
      mln_precondition(exact(input).is_valid());
      int ws[] = { -1, +2, -1,
		   +2, -4, +2,
		   -1, +2, -1 };
      mln_ch_convolve(I, int) output = convolve(input, make::w_window2d(ws));
      trace::exiting("linear::lap_o");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_LAP_HH
