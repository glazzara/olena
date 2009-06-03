// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_MAKE_W_WINDOW2D_INT_HH
# define MLN_MAKE_W_WINDOW2D_INT_HH

/*! \file mln/make/w_window2d_int.hh
 *
 * \brief Routine to create a mln::w_window2d_int.
 */

# include <mln/core/alias/w_window2d_int.hh>
# include <mln/make/w_window2d.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create a mln::w_window2d_int.
     *
     * \param[in] weights Array of integers.
     *
     * \pre The array size, \c M, has to be a square of an odd integer.
     *
     * \return A 2D int-weighted window.
     */
    template <unsigned M>
    mln::w_window2d_int w_window2d_int(int (&weights)[M]);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned M>
    inline
    mln::w_window2d_int
    w_window2d_int(int (&weights)[M])
    {
      mln_precondition((2 * (unsigned(std::sqrt(float(M))) / 2) + 1) *
		       (2 * (unsigned(std::sqrt(float(M))) / 2) + 1) == M);
      return make::w_window2d(weights);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_W_WINDOW2D_INT_HH
