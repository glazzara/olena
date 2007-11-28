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

#ifndef MLN_MAKE_BOX2D_H_HH
# define MLN_MAKE_BOX2D_H_HH

/*! \file mln/make/box2d_h.hh
 *
 * \brief Routines to construct an mln::box2d_h.
 */

# include <mln/core/box2d_h.hh>
# include <mln/make/point2d_h.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create an mln::box2d_h.
     *
     * \param[in] nrows Number of rows.
     * \param[in] ncols Number of columns.
     *
     * \pre \p nrows != 0 and \p ncols != 0.
     *
     * \return A 2D_H box.
     */
    mln::box2d_h box2d_h(unsigned nrows, unsigned ncols);


    /*! \brief Create an mln::box2d_h.
     *
     * \overload
     *
     * \param[in] min_row Index of the top most row.
     * \param[in] min_col Index of the left most column.
     * \param[in] max_row Index of the botton most row.
     * \param[in] max_col Index of the right most column.
     *
     * \pre \p max_row >= \p min_row and \p max_col >= \p min_col.
     *
     * \return A 2D_H box.
     */
    mln::box2d_h box2d_h(int min_row, int min_col,
			 int max_row, int max_col);


# ifndef MLN_INCLUDE_ONLY

    inline
    mln::box2d_h box2d_h(unsigned nrows, unsigned ncols)
    {
      mln_precondition(nrows != 0 && ncols != 0);
      mln::box2d_h tmp(make::point2d_h(0, 0),
		       make::point2d_h(nrows - 1, ncols - 1));
      return tmp;
    }

    inline
    mln::box2d_h box2d_h(int min_row, int min_col,
			 int max_row, int max_col)
    {
      mln_precondition(max_row >= min_row && max_col >= min_col);
      mln::box2d_h tmp(make::point2d_h(min_row, min_col),
		       make::point2d_h(max_row, max_col));
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_BOX2D_H_HH
