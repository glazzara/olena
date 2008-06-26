// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_MAKE_NEIGHB2D_HH
# define MLN_MAKE_NEIGHB2D_HH

/*! \file mln/make/neighb2d.hh
 *
 * \brief Routine to create an mln::neighb2d.
 */

# include <mln/make/window2d.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create a mln::neighb2d.
     *
     * \param[in] values Array of Booleans.
     *
     * \pre The array size, \c S, has to be a square of an odd integer.
     *
     * \return A 2D neighborhood.
     */
    template <unsigned S>
    mln::neighb2d neighb2d(bool (&values)[S]);


    /*! \brief Create a mln::neighb2d.
     *
     * \param[in] values Double-array of Booleans.
     *
     * \pre \c R and \c C, defining the array size, have to be odd.
     *
     * \return A 2D neighborhood.
     */
    template <unsigned R, unsigned C>
    mln::neighb2d neighb2d(bool (&values)[R][C]);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned S>
    inline
    mln::neighb2d
    neighb2d(bool (&values)[S])
    {
      enum { h = mlc_sqrt_int(S) / 2 };
      mlc_bool((2 * h + 1) * (2 * h + 1) == S)::check();
      mln::window2d win = make::window2d(values);
      mln::neighb2d tmp(win);
      return tmp;
    }

    template <unsigned R, unsigned C>
    inline
    mln::neighb2d
    neighb2d(bool (&values)[R][C])
    {
      mlc_bool(R % 2 == 1)::check();
      mlc_bool(C % 2 == 1)::check();
      mln::window2d win = make::window2d(values);
      mln::neighb2d tmp(win);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_NEIGHB2D_HH
