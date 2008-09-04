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

#ifndef MLN_MAKE_WINDOW3D_HH
# define MLN_MAKE_WINDOW3D_HH

/*! \file mln/make/window3d.hh
 *
 * \brief Routine to create an mln::window3d.
 */

# include <cmath>
# include <mln/core/alias/window3d.hh>
# include <mln/make/dpoint3d.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create an mln::window3d.
     *
     * \param[in] values Array of Booleans.
     *
     * \pre The array size, \c M, has to be a cube of an odd integer.
     *
     * \return A 3D window.
     */
    template <unsigned M>
    mln::window3d window3d(bool (&values)[M]);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned M>
    inline
    mln::window3d window3d(bool (&values)[M])
    {
      int h = unsigned(std::pow(float(M), float(1. / 3.))) / 2;
      mln_precondition((2 * h + 1) * (2 * h + 1) * (2 * h + 1) == M);
      mln::window3d tmp;
      unsigned i = 0;
      for (int sli = - h; sli <= h; ++sli)
	for (int row = - h; row <= h; ++row)
	  for (int col = - h; col <= h; ++col)
	    if (values[i++])
	      tmp.insert(make::dpoint3d(sli, row, col));
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_WINDOW3D_HH
