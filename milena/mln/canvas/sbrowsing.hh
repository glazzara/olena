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

#ifndef MLN_CANVAS_SBROWSING_HH
# define MLN_CANVAS_SBROWSING_HH

/*! \file mln/canvas/sbrowsing.hh
 *
 * \brief Sbrowsing of an image.
 */

# include <mln/core/dpoint2d.hh> // for "up"


namespace mln
{

  namespace canvas
  {

    /*! FIXME: Doc!
     *
     *
     * F should be
     * {
     *   input;  p;  --- attributes
     *   void init();
     *   void down(); void fwd(); void bkd(); --- methods
     * }
     */
    template <typename F>
    void sbrowsing(F& f);


# ifndef MLN_INCLUDE_ONLY

    template <typename F>
    void sbrowsing(F& f)
    {
      mln_precondition(f.input.has_data());
      int
	min_row = f.input.min_row(), max_row = f.input.max_row(),
	min_col = f.input.min_col(), max_col = f.input.max_col();

      // p
      f.p = f.input.domain().pmin() + up;
      int& row = f.p.row();
      int& col = f.p.col();

      // initialization
      f.init();

      bool fwd = true;
      for (row = min_row; row <= max_row; ++row)
	{
	  // go down
	  f.down();

	  if (fwd)
	    // browse line fwd
	    while (col < max_col)
	      {
		++col;
		f.fwd();
	      }
	  else
	    // browse line bkd
	    while (col > min_col)
	      {
		--col;
		f.bkd();
	      }

	  // change browsing
	  fwd = ! fwd;
	}
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_SBROWSING_HH
