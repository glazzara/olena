// Copyright (C) 2007, 2008, 2009, 2010, 2011 EPITA Research and
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

#ifndef MLN_MAKE_BOX3D_HH
# define MLN_MAKE_BOX3D_HH

/// \file
///
/// Routines to construct an mln::box3d.

# include <mln/core/alias/box3d.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create an mln::box3d.
     *
     * \param[in] nslis Number of slices.
     * \param[in] nrows Number of rows.
     * \param[in] ncols Number of columns.
     *
     * \pre \p ninds != 0 and \p ncols != 0 and \p nslis != 0.
     *
     * \return A 3D box.
     */
    mln::box3d box3d(unsigned nslis, unsigned nrows, unsigned ncols);


    /*! \brief Create an mln::box3d.
     *
     * \overload
     *
     * \param[in] min_sli Index of the lowest slice.
     * \param[in] min_row Index of the top most row.
     * \param[in] min_col Index of the left most column.
     * \param[in] max_sli Index of the highest slice.
     * \param[in] max_row Index of the botton most row.
     * \param[in] max_col Index of the right most column.
     *
     * \pre \p max_sli >= \p min_sli.
     * \pre \p max_row >= \p min_row.
     * \pre \p max_col >= \p min_col.
     *
     * \return A 3D box.
     */
    mln::box3d box3d(def::coord min_sli, def::coord min_row, def::coord min_col,
		     def::coord max_sli, def::coord max_row, def::coord max_col);


# ifndef MLN_INCLUDE_ONLY

    inline
    mln::box3d box3d(unsigned nslis, unsigned nrows, unsigned ncols)
    {
      mln_precondition(nrows != 0 && ncols != 0 && nslis != 0);
      mln::box3d tmp(point3d(0, 0, 0),
		     point3d(static_cast<def::coord>(nslis - 1),
			     static_cast<def::coord>(nrows - 1),
			     static_cast<def::coord>(ncols - 1)));
      return tmp;
    }

    inline
    mln::box3d
    box3d(def::coord min_sli, def::coord min_row, def::coord min_col,
	  def::coord max_sli, def::coord max_row, def::coord max_col)
    {
      mln_precondition(max_row >= min_row && max_sli >= min_sli && max_col >= min_col);
      mln::box3d tmp(point3d(min_sli, min_row, min_col),
		     point3d(max_sli, max_row, max_col));
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_BOX3D_HH
