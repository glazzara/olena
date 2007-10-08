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

#ifndef MLN_MAKE_POINT2D_H_HH
# define MLN_MAKE_POINT2D_H_HH

/*! \file mln/make/point2d_h.hh
 *
 * \brief Routine to construct an mln::point2d_h.
 */

# include <mln/core/point2d_h.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create an mln::point2d_h.
     *
     * \param[in] row Row coordinate.
     * \param[in] col Column coordinate.
     *
     * \return A 2D point.
     */
    mln::point2d_h point2d_h(int row, int col);


# ifndef MLN_INCLUDE_ONLY

    mln::point2d_h point2d_h(int row, int col)
    {
      mln::point2d_h tmp;
      tmp[0] = row;
      tmp[1] = col;
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_POINT2D_H_HH
