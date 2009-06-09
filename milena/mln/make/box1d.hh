// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MAKE_BOX1D_HH
# define MLN_MAKE_BOX1D_HH

/// \file
///
/// Routines to construct an mln::box1d.

# include <mln/core/alias/box1d.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create an mln::box1d.
     *
     * \param[in] ninds Number of indices.
     *
     * \pre \p ninds != 0 and \p ncols != 0.
     *
     * \return A 1D box.
     */
    mln::box1d box1d(unsigned ninds);


    /*! \brief Create an mln::box1d.
     *
     * \overload
     *
     * \param[in] min_ind Minimum index.
     * \param[in] max_ind Maximum index.
     *
     * \pre \p max_ind >= \p min_ind.
     *
     * \return A 1D box.
     */
    mln::box1d box1d(def::coord min_ind, def::coord max_ind);


# ifndef MLN_INCLUDE_ONLY

    inline
    mln::box1d box1d(unsigned ninds)
    {
      mln_precondition(ninds != 0);
      mln::box1d tmp(point1d(0),
		     point1d(static_cast<def::coord>(ninds - 1)));
      return tmp;
    }

    inline
    mln::box1d box1d(def::coord min_ind, def::coord max_ind)
    {
      mln_precondition(max_ind >= min_ind);
      return mln::box1d(point1d(min_ind),
                        point1d(max_ind));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_BOX1D_HH
