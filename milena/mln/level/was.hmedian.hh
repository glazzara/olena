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

#ifndef MLN_LEVEL_WAS_HMEDIAN_HH
# define MLN_LEVEL_WAS_HMEDIAN_HH

/*! \file mln/level/was.hmedian.hh
 *
 * \brief Median filtering of an image.
 */


namespace mln
{

  namespace level
  {

    namespace impl
    {

      template <typename I, typename O>
      void hmedian(const I& input, const hline2d& win, O& output)
      {
	const int
	  max_row = input.max_row(),
	  min_col = input.min_col(),
	  max_col = input.max_col();
	const unsigned half = win.length() / 2;

	point2d p;
	int& row = p.row();
	int& col = p.col();

	accu::median_on<mln_value(I)> med;

	for (row = input.min_row(); row <= max_row; ++row)
	  {
	    int ct, cu;

	    // initialization (before first point of the row)
	    med.init();
	    for (ct = min_col; ct < min_col + half; ++ct)
	      med.take(input.at(row, ct));

	    // left columns (just take new points)
	    for (col = min_col; col <= min_col + half; ++col, ++ct)
	      {
		med.take(input.at(row, ct));
		output(p) = med;
	      }
	    
	    // middle columns (both take and untake)
	    cu = min_col;
	    for (; col <= max_col - half; ++cu, ++col, ++ct)
	      {
		med.take(input.at(row, ct));
		med.untake(input.at(row, cu));
		output(p) = med;
	      }

	    // right columns (now just untake old points)
	    for (; col <= max_col; ++cu, ++col)
	      {
		med.untake(input.at(row, cu));
		output(p) = med;
	      }
	  }
      }

    } // end of namespace mln::level::impl

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_WAS_HMEDIAN_HH
