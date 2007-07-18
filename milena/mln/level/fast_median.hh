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

#ifndef MLN_LEVEL_FAST_MEDIAN_HH
# define MLN_LEVEL_FAST_MEDIAN_HH

/*! \file mln/level/median.hh
 *
 * \brief Fast Median filtering of an image.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/window2d.hh>
# include <mln/accu/median.hh>
# include <ostream>


namespace mln
{

  namespace level
  {

    /*! Compute in \p output the median filter of image \p input by
     *  the window \p win.
     *
     * \param[in] input The image to be filtered.
     * \param[in] win The window.
     * \param[in,out] output The output image.
     *
     * \pre \p input and \p output have to be initialized.
     */
    template <typename I, typename W, typename O>
    void fast_median(Image<I>& input, const Window<W>& win,
		     Image<O>& output);


# ifndef MLN_INCLUDE_ONLY


    namespace impl
    {

      template <typename I, typename W, typename O>
      void fast_median(I& input,
		       const W& win,
		       O& output)
      {
	mln_precondition(input.has_data());
	mln_precondition(output.has_data());

	int
	  min_row = input.min_row(), max_row = input.max_row(),
	  min_col = input.min_col(), max_col = input.max_col();

	window2d
	  win_fwd_plus  = win - (win + left),
	  win_fwd_minus = (win + left) - win,
	  win_bkd_plus  = win - (win + right),
	  win_bkd_minus = (win + right) - win,
	  win_bot  = win - (win + up),
	  win_top = (win + up) - win;



	//Fixme bug
	point2d p;

	accu::median_on<mln_value(I)> med;

	// initialization

	p = input.domain().pmin() + up;
	mln_qixter(I)
	  q_fp(win_fwd_plus, p, input), q_fm(win_fwd_minus, p, input),
	  q_bp(win_bkd_plus, p, input), q_bm(win_bkd_minus, p, input),
	  q_top(win_top, p, input), q_bot(win_bot, p, input);

	med.init();
	{
	  mln_qixter(I) q(win, p, input);
	  for_all(q) //if (input.has(q))
	    med.take(*q);
	}

	int& row = p.row();
	int& col = p.col();
	bool fwd = true;

	mln_assertion(p.col() == min_col);
	mln_assertion(p.row() == min_row - 1);

	for (row = min_row; row <= max_row; ++row)
	  {

	    // "go down"
	    for_all(q_top) //if (input.has(q_top))
	      med.untake(*q_top);

	    for_all(q_bot) //if (input.has(q_bot))
	      med.take(*q_bot);

	    output(p) = med;

	    if (fwd)
	      // browse line fwd
	      while (col < max_col)
		{
		  std::cout << "debug: psite" << q_fm.psite() << "\n";
		  ++col;
		  for_all(q_fm) //if (input.has(q_fm))
		    med.untake(*q_fm);
		  for_all(q_fp) //if (input.has(q_fp))
		    med.take(*q_fp);
		  output(p) = med;
		}
	    else
	      // browse line bkd
	      while (col > min_col)
		{
		  --col;
		  for_all(q_bm) //if (input.has(q_bm))
		    med.untake(*q_bm);
		  for_all(q_bp) //if (input.has(q_bp))
		    med.take(*q_bp);
		  output(p) = med;
		}
	    // change browsing
	    fwd = ! fwd;
	  }
      }

    } // end of namespace mln::level::impl


    // facade

    template <typename I, typename W, typename O>
    void fast_median(Image<I>& input, const Window<W>& win,
		     Image<O>& output)
    {
      impl::fast_median(exact(input), exact(win), exact(output));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_FAST_MEDIAN_HH
