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

#ifndef MLN_LEVEL_MEDIAN_HH
# define MLN_LEVEL_MEDIAN_HH

/*! \file mln/level/median.hh
 *
 * \brief Median filtering of an image.
 */

# include <mln/core/concept/image.hh>

# include <mln/core/window2d.hh>
# include <mln/core/hline2d.hh>

# include <mln/accu/median.hh>
# include <mln/canvas/sbrowsing.hh>


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
    void median(const Image<I>& input, const Window<W>& win,
		Image<O>& output);


# ifndef MLN_INCLUDE_ONLY


    namespace impl
    {


      template <typename I, typename W, typename O>
      struct median_functor
      { 
	// i/o

	const I& input;
	const W& win;
	O& output;

	// aux data

	accu::median<mln_vset(I)> med;
	mln_point(I) p;
	window2d     win_fp, win_fm, win_bp, win_bm, win_dp, win_dm;
	mln_qiter(W)   q_fp,   q_fm,   q_bp,   q_bm,   q_dp,   q_dm;

	// ctor

	median_functor(const I& input_, const W& win_, O& output_)
	  :
	  // i/o
	  input(exact(input_)),
	  win(exact(win_)),
	  output(exact(output_)),
	  // aux data
	  med(input.values()),
	  p(),
	  win_fp(win - (win + left)),   win_fm((win + left)  - win),
	  win_bp(win - (win + right)),  win_bm((win + right) - win),
	  win_dp(win - (win + up)),     win_dm((win + up)    - win),
	  q_fp(win_fp, p),  q_fm(win_fm, p),
	  q_bp(win_bp, p),  q_bm(win_bm, p),
	  q_dp(win_dp, p),  q_dm(win_dm, p)
	{
	}

	// parts

	void init()
	{
	  med.init();
	  mln_qiter(W) q(win, p);
	  for_all(q) if (input.has(q))
	    med.take(input(q));
	}

	void down()
	{
	  for_all(q_dm) if (input.has(q_dm))
	    med.untake(input(q_dm));
	  for_all(q_dp) if (input.has(q_dp))
	    med.take(input(q_dp));
	  output(p) = med;
	}

	void fwd()
	{
	  for_all(q_fm) if (input.has(q_fm))
	    med.untake(input(q_fm));
	  for_all(q_fp) if (input.has(q_fp))
	    med.take(input(q_fp));
	  output(p) = med;
	}

	void bkd()
	{
	  for_all(q_bm) if (input.has(q_bm))
	    med.untake(input(q_bm));
	  for_all(q_bp) if (input.has(q_bp))
	    med.take(input(q_bp));
	  output(p) = med;
	}

      }; // end of median_functor




      template <typename I, typename W, typename O>
      void median(const I& input, const Window<W>& win, O& output)
      {
	// FIXME: resize border!
	impl::median_functor<I,W,O> f(input, exact(win), output);
	canvas::sbrowsing(f);
      }




      template <typename I, typename O>
      void median(const I& input, const hline2d& win, O& output)
      {
	typedef mln_coord(I) coord;
	const coord
	  max_row = input.max_row(),
	  min_col = input.min_col(),
	  max_col = input.max_col();
	const coord half = win.length() / 2;

	point2d p;
	coord& row = p.row();
	coord& col = p.col();

	accu::median<mln_vset(I)> med(input.values());

	for (row = input.min_row(); row <= max_row; ++row)
	  {
	    coord ct, cu;

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


      // FIXME: Use transpose.

//       template <typename I, typename O>
//       void median(const I& input, const vline2d& win, O& output)
//       {

// 	median(, hline2d(win.length()), output);
//       }


    } // end of namespace mln::level::impl


    // facade

    template <typename I, typename W, typename O>
    void median(const Image<I>& input, const Window<W>& win,
		Image<O>& output)
    {
      mln_assertion(exact(output).domain() == exact(input).domain());
      impl::median(exact(input), exact(win), exact(output)); 
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_MEDIAN_HH
