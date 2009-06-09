// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MAKE_W_WINDOW1D_HH
# define MLN_MAKE_W_WINDOW1D_HH

/*! \file
 *
 * \brief Routine to create an mln::w_window in the 1D case.
 */

# include <cmath>

# include <mln/core/w_window.hh>
# include <mln/core/alias/dpoint1d.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create a 1D mln::w_window from an array of weights.
     *
     * \param[in] weights Array.
     *
     * \pre The array size, \c M, has to be a square of an odd integer.
     *
     * \return A 1D weighted window.
     */
    template <typename W, unsigned M>
    mln::w_window<mln::dpoint1d, W> w_window1d(W (&weights)[M]);


# ifndef MLN_INCLUDE_ONLY

    template <typename W, unsigned M>
    inline
    mln::w_window<mln::dpoint1d, W>
    w_window1d(W (&weights)[M])
    {
      int h = M / 2;
      mln_precondition(1 == (M % 2));
      mln::w_window<mln::dpoint1d, W> tmp;
      unsigned i = 0;
      for (int ind = - h; ind <= h; ++ind)
	  {
	    if (weights[i] != 0)
	      tmp.insert(weights[i], dpoint1d(ind));
	    i++;
	  }
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_W_WINDOW1D_HH
