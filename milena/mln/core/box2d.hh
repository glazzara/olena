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

#ifndef MLN_CORE_BOX2D_HH
# define MLN_CORE_BOX2D_HH

/*! \file mln/core/box2d.hh
 *
 * \brief Definition of the mln::box2d alias and of construction
 * routines.
 */

# include <mln/core/box.hh>
# include <mln/core/point2d.hh>


namespace mln
{

  /*! \brief Type alias for a box defined on the 2D square grid with
   * integer coordinates.
   *
   * \see mln::rectangle2d.
   */
  typedef box_<point2d> box2d;


  /*! \brief Create an mln::box2d.
   *
   * \param[in] nrows Number of rows.
   * \param[in] ncols Number of columns.
   *
   * \pre \p nrows != 0 and \p ncols != 0.
   *
   * \return A 2D box.
   */
  box2d mk_box2d(unsigned nrows, unsigned ncols)
  {
    mln_precondition(nrows != 0 && ncols != 0);
    box2d tmp(mk_point2d(0, 0),
	      mk_point2d(nrows - 1, ncols - 1));
    return tmp;
  }


  /*! \brief Create an mln::box2d.
   *
   * \overload
   *
   * \param[in] min_row Index of the top most row.
   * \param[in] max_row Index of the botton most row.
   * \param[in] min_col Index of the left most column.
   * \param[in] max_col Index of the right most column.
   *
   * \pre \p max_row >= \p min_row and \p max_col >= \p min_col.
   *
   * \return A 2D box.
   */
  box2d mk_box2d(int min_row, int max_row,
		 int min_col, int max_col)
  {
    mln_precondition(max_row >= min_row && max_col >= min_col);
    box2d tmp(mk_point2d(min_row, min_col),
	      mk_point2d(max_row, max_col));
    return tmp;
  }

} // end of namespace mln


#endif // ! MLN_CORE_BOX2D_HH
