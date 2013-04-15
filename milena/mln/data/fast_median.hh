// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_DATA_FAST_MEDIAN_HH
# define MLN_DATA_FAST_MEDIAN_HH

/*! \file
 *
 * \brief Fast Median filtering of an image.
 *
 * \todo There are implicit assumptions about input being 2D!!!
 */

# include <mln/core/concept/image.hh>
# include <mln/core/alias/window2d.hh>
# include <mln/accu/stat/median_h.hh>

# include <mln/win/shift.hh>
# include <mln/win/diff.hh>

# include <mln/geom/min_col.hh>
# include <mln/geom/min_row.hh>
# include <mln/geom/max_col.hh>
# include <mln/geom/max_row.hh>


namespace mln
{

  namespace data
  {

    /*! \brief Compute in \p output the median filter of image \p
     *  input by the window \p win.
     *
     * \param[in] input The image to be filtered.
     * \param[in] win The window.
     * \param[in,out] output The output image.
     *
     * \pre \p input and \p output have to be initialized.
     *
     * \ingroup mlndatafilter
     */
    template <typename I, typename W, typename O>
    void fast_median(const Image<I>& input, const Window<W>& win,
		     Image<O>& output);


# ifndef MLN_INCLUDE_ONLY


    namespace impl
    {

      template <typename I, typename W, typename O>
      inline
      void fast_median(const I& input,
		       const W& win,
		       O& output)
      {
	mln_precondition(input.is_valid());
	mln_precondition(output.is_valid());

	def::coord
	  min_row = geom::min_row(input), max_row = geom::max_row(input),
	  min_col = geom::min_col(input), max_col = geom::max_col(input);

	window2d
	  win_fwd_plus  = win - win::shift(win, left),
	  win_fwd_minus = win::shift(win, left) - win,
	  win_bkd_plus  = win - win::shift(win, right),
	  win_bkd_minus = win::shift(win, right) - win,
	  win_bot       = win - win::shift(win, up),
	  win_top       = win::shift(win, up) - win;

	accu::stat::median_h<mln_value(I)> med;

	// initialization

	point2d p = input.domain().pmin() + up;

	mln_qixter(const I, window2d)
	  q_fp(input, win_fwd_plus, p), q_fm(input, win_fwd_minus, p),
	  q_bp(input, win_bkd_plus, p), q_bm(input, win_bkd_minus, p),
	  q_top(input, win_top, p), q_bot(input, win_bot, p);

	med.init();
	{
	  mln_qixter(const I, W) q(input, win, p);
	  for_all(q)
	    med.take(q.val());
	}

	def::coord& row = p.row();
	def::coord& col = p.col();
	bool fwd = true;

	mln_assertion(p.col() == min_col);
	mln_assertion(p.row() == min_row - 1);

	for (row = min_row; row <= max_row; ++row)
	  {

	    // "go down"
	    for_all(q_top)
	      med.untake(q_top.val());

	    for_all(q_bot)
	      med.take(q_bot.val());

	    output(p) = med;

	    if (fwd)
	      // browse line fwd
	      while (col < max_col)
		{
		  ++col;
		  for_all(q_fm)
		    med.untake(q_fm.val());
		  for_all(q_fp)
		    med.take(q_fp.val());
		  output(p) = med;
		}
	    else
	      // browse line bkd
	      while (col > min_col)
		{
		  --col;
		  for_all(q_bm)
		    med.untake(q_bm.val());
		  for_all(q_bp)
		    med.take(q_bp.val());
		  output(p) = med;
		}
	    // change browsing
	    fwd = ! fwd;
	  }
      }

    } // end of namespace mln::data::impl


    // facade

    template <typename I, typename W, typename O>
    inline
    void fast_median(const Image<I>& input, const Window<W>& win,
		     Image<O>& output)
    {
      impl::fast_median(exact(input), exact(win), exact(output));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_FAST_MEDIAN_HH
