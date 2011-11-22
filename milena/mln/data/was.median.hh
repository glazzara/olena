// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_DATA_WAS_MEDIAN_HH
# define MLN_DATA_WAS_MEDIAN_HH

/*! \file
 *
 * \brief Obsolete routines for median filtering.
 */

# include <mln/win/shift.hh>
# include <mln/core/alias/window2d.hh>

# include <mln/geom/min_col.hh>
# include <mln/geom/max_col.hh>
# include <mln/geom/max_row.hh>
# include <mln/geom/min_row.hh>

# include <mln/win/diff.hh>
# include <mln/win/shift.hh>

# include <mln/data/median.hh>
# include <mln/win/hline2d.hh>

# include <mln/opt/at.hh>

namespace mln
{

  namespace data
  {

    namespace impl
    {


      // prefer using a canvas

      template <typename I, typename W, typename O>
      void median_as_procedure(const I& input,
			       const W& win,
			       O& output)
      {
	mln_precondition(input.is_valid());
	mln_precondition(output.is_valid());

	int
	  min_row = geom::min_row(input), max_row = geom::max_row(input),
	  min_col = geom::min_col(input), max_col = geom::max_col(input);

	window2d
	  win_fwd_plus  = win - win::shift(win, left),
	  win_fwd_minus = win::shift(win, left)  - win,
	  win_bkd_plus  = win - win::shift(win, right),
	  win_bkd_minus = win::shift(win, right) - win,
	  win_bot       = win - win::shift(win, up),
	  win_top       = win::shift(win, up)    - win;

	point2d p;
	mln_qiter(W)
	  q_fp(win_fwd_plus, p), q_fm(win_fwd_minus, p),
	  q_bp(win_bkd_plus, p), q_bm(win_bkd_minus, p),
	  q_top(win_top, p), q_bot(win_bot, p);

	accu::stat::median_h<mln_vset(I)> med(input.values());

	// initialization

	p = input.bbox().pmin() + up;
	med.init();
	{
	  mln_qiter(W) q(win, p);
	  for_all(q) if (input.has(q))
	    med.take(input(q));
	}

	def::coord& row = p.row();
	def::coord& col = p.col();
	bool fwd = true;

	mln_assertion(p.col() == min_col);
	mln_assertion(p.row() == min_row - 1);

	for (row = min_row; row <= max_row; ++row)
	  {
	    // "go down"
	    for_all(q_top) if (input.has(q_top))
	      med.untake(input(q_top));
	    for_all(q_bot) if (input.has(q_bot))
	      med.take(input(q_bot));
	    output(p) = med;

	    if (fwd)
	      // browse line fwd
	      while (col < max_col)
		{
		  ++col;
		  for_all(q_fm) if (input.has(q_fm))
		    med.untake(input(q_fm));
		  for_all(q_fp) if (input.has(q_fp))
		    med.take(input(q_fp));
		  output(p) = med;
		}
	    else
	      // browse line bkd
	      while (col > min_col)
		{
		  --col;
		  for_all(q_bm) if (input.has(q_bm))
		    med.untake(input(q_bm));
		  for_all(q_bp) if (input.has(q_bp))
		    med.take(input(q_bp));
		  output(p) = med;
		}

	    // change browsing
	    fwd = ! fwd;
	  }
      }



      // horizontal median

      template <typename I, typename O>
      void hmedian(const I& input, const win::hline2d& win, O& output)
      {

	const int
	  min_row = geom::min_row(input), max_row = geom::max_row(input),
	  min_col = geom::min_col(input), max_col = geom::max_col(input);
	const unsigned half = win.length() / 2;

	point2d p;
	def::coord& row = p.row();
	def::coord& col = p.col();

	accu::stat::median_h<mln_vset(I)> med(input.values());

	for (row = min_row; row <= max_row; ++row)
	  {
	    int ct, cu;

	    // initialization (before first point of the row)
	    med.init();
	    for (ct = min_col; ct < min_col + half; ++ct)
	      med.take(opt::at(input, row, ct));

	    // left columns (just take new points)
	    for (col = min_col; col <= min_col + half; ++col, ++ct)
	      {
		med.take(opt::at(input, row, ct));
		output(p) = med;
	      }

	    // middle columns (both take and untake)
	    cu = min_col;
	    for (; col <= max_col - half; ++cu, ++col, ++ct)
	      {
		med.take(opt::at(input, row, ct));
		med.untake(opt::at(input, row, cu));
		output(p) = med;
	      }

	    // right columns (now just untake old points)
	    for (; col <= max_col; ++cu, ++col)
	      {
		med.untake(opt::at(input, row, cu));
		output(p) = med;
	      }
	  }

      } // end of hmedian



    } // end of namespace mln::data::impl

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_WAS_MEDIAN_HH
