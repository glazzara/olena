// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_MAKE_W_WINDOW2D_HH
# define MLN_MAKE_W_WINDOW2D_HH

/*! \file mln/make/w_window2d.hh
 *
 * \brief Routine to create an mln::w_window in the 2D case.
 */

# include <mln/core/w_window.hh>
# include <mln/core/alias/dpoint2d.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create a 2D mln::w_window from an array of weights.
     *
     * \param[in] weights Array.
     *
     * \pre The array size, \c S, has to be a square of an odd integer.
     *
     * \return A 2D weighted window.
     */
    template <typename W, unsigned S>
    mln::w_window<mln::dpoint2d, W> w_window2d(W (&weights)[S]);


# ifndef MLN_INCLUDE_ONLY

    template <typename W, unsigned S>
    inline
    mln::w_window<mln::dpoint2d, W>
    w_window2d(W (&weights)[S])
    {
      enum { s = mlc_sqrt_int(S) / 2 };
      metal::bool_<(((2 * s + 1) * (2 * s + 1)) == S)>::check();
      mln::w_window<mln::dpoint2d, W> tmp;
      convert::from_to(weights, tmp);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_W_WINDOW2D_HH
