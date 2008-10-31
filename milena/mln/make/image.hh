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

#ifndef MLN_MAKE_IMAGE_HH
# define MLN_MAKE_IMAGE_HH

/*! \file mln/make/image.hh
 *
 * \brief Routine to create an image from various input.
 */

# include <mln/core/image/image1d.hh>
# include <mln/core/image/image2d.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create an image1d from an 1d array of values.
     *
     * \param[in] values 1d array.
     *
     * \return A 1D image.
     */
    template <typename V, unsigned L>
    mln::image1d<V> image1d(V (&values)[L]);


    /*! \brief Create an image2d from an 2d array of values.
     *
     * \param[in] values 2d array.
     *
     * \return A 2D image.
     */
    template <typename V, unsigned R, unsigned C>
    mln::image2d<V> image(V (&values)[R][C]);


# ifndef MLN_INCLUDE_ONLY

    template <typename V, unsigned L>
    mln::image1d<V>
    image1d(V (&values)[L])
    {
      mlc_bool(L != 0)::check();
      mln::image1d<V> tmp(L);
      for (unsigned ind = 0; ind < L; ++ind)
	tmp(point1d(ind)) = values[ind];
      return tmp;
    }

    template <typename V, unsigned R, unsigned C>
    mln::image2d<V>
    image(V (&values)[R][C])
    {
      mln::image2d<V> tmp(R, C);
      for (unsigned row = 0; row < R; ++row)
	for (unsigned col = 0; col < C; ++col)
	    tmp(point2d(row, col)) = values[row][col];
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_IMAGE_HH
