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
 *
 * \todo Add Fast_Image versions.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/window.hh>
# include <mln/geom/shift.hh>
# include <mln/set/diff.hh>

# include <mln/canvas/browsing/snake_fwd.hh>
# include <mln/canvas/dirbrowsing.hh>
# include <mln/accu/median.hh>


namespace mln
{

  namespace level
  {

    /*! Compute in \p output the median filter of image \p input by
     *  the window \p win.
     *
     * \param[in] input The image to be filtered.
     * \param[in] win The window.
     * \param[out] output The output image.
     *
     * \pre \p input and \p output have to be initialized.
     */
    template <typename I, typename W, typename O>
    void median(const Image<I>& input, const Window<W>& win,
		Image<O>& output);



    /*! Compute in \p output the median filter of image \p input in
     *  the direction \p dir with strength \p length.
     *
     * \param[in] input The image to be filtered.
     * \param[in] dir The filtering direction.
     * \param[in] length The filtering strength.
     * \param[out] output The output image.
     *
     * \pre \p input and \p output have to be initialized.
     * \pre \p dir is between 0 and less than the image dimension.
     * \pre \p length has to be odd.
     */
    template <typename I, typename O>
    void median_dir(const Image<I>& input, unsigned dir, unsigned length,
		    Image<O>& output);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {


      // Functors.


      template <typename I, typename W, typename O>
      struct median_t
      { 
	typedef mln_point(I)  P;
	typedef mln_dpoint(I) D;

	// i/o

	const I& input;
	const W& win;
	O& output;

	// aux data

	accu::median<mln_vset(I)> med;
	P p;
	window<D>    win_fp, win_fm, win_bp, win_bm, win_dp, win_dm;
	mln_qiter(W)   q_fp,   q_fm,   q_bp,   q_bm,   q_dp,   q_dm;

	// ctor

	median_t(const I& input_, const W& win_, O& output_)
	  :
	  // i/o
	  input(exact(input_)),
	  win(exact(win_)),
	  output(exact(output_)),
	  // aux data
	  med(input.values()),
	  p(),
	  win_fp(set::diff(win, geom::shift(win, left))),
	  win_fm(set::diff(geom::shift(win, left),  win)),
	  win_bp(set::diff(win, geom::shift(win, right))),
	  win_bm(set::diff(geom::shift(win, right), win)),
	  win_dp(set::diff(win, geom::shift(win, up))),
	  win_dm(set::diff(geom::shift(win, up),    win)),
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
	  output(p) = med.to_result();
	}

	void fwd()
	{
	  for_all(q_fm) if (input.has(q_fm))
	    med.untake(input(q_fm));
	  for_all(q_fp) if (input.has(q_fp))
	    med.take(input(q_fp));
	  output(p) = med.to_result();
	}

	void bkd()
	{
	  for_all(q_bm) if (input.has(q_bm))
	    med.untake(input(q_bm));
	  for_all(q_bp) if (input.has(q_bp))
	    med.take(input(q_bp));
	  output(p) = med.to_result();
	}

      }; // end of median_t



      template <typename I, typename O>
      struct median_dir_t
      {
	typedef mln_point(I) point;
	enum { dim = point::dim };

	// i/o
	const I& input;
	const unsigned dir;
	const unsigned length;
	O& output;

	// aux data
	const mln_point(I)
	  pmin, pmax;
	const mln_coord(I)
	  pmin_dir,      pmax_dir,
	  pmin_dir_plus, pmax_dir_minus;
	accu::median<mln_vset(I)> med;

	// ctor
	median_dir_t(const I& input, unsigned dir, unsigned length, O& output)
	  : // i/o
	    input(input),
	    dir(dir),
	    length(length),
	    output(exact(output)),
	    // aux data
	    pmin(input.domain().pmin()),
	    pmax(input.domain().pmax()),
	    pmin_dir(pmin[dir]),
	    pmax_dir(pmax[dir]),
	    pmin_dir_plus (pmin[dir] + length / 2),
	    pmax_dir_minus(pmax[dir] - length / 2),
	    med(input.values())
	{
	}

	void init()
	{
	}

	void process(const mln_point(I)& p_)
	{
	  mln_point(I)
	    p  = p_,
	    pt = p,
	    pu = p;

	  typedef mln_coord(I)& coord_ref;
	  coord_ref
	    ct = pt[dir],
	    cu = pu[dir],
	    p_dir = p[dir];

	  // initialization (before first point of the row)
	  med.init();
	  for (ct = pmin_dir; ct < pmin_dir_plus; ++ct)
	    if (input.has(pt))
	      med.take(input(pt));

	  // left columns (just take new points)
	  for (p_dir = pmin_dir; p_dir <= pmin_dir_plus; ++p_dir, ++ct)
	    {
	      if (input.has(pt))
		med.take(input(pt));
	      if (output.has(p))
		output(p) = med.to_result();
	    }

	  // middle columns (both take and untake)
	  cu = pmin[dir];
	  for (; p_dir <= pmax_dir_minus; ++cu, ++p_dir, ++ct)
	    {
	      if (input.has(pt))
		med.take(input(pt));
	      if (input.has(pu))
		med.untake(input(pu));
	      if (output.has(p))
		output(p) = med.to_result();
	    }

	  // right columns (now just untake old points)
	  for (; p_dir <= pmax_dir; ++cu, ++p_dir)
	    {
	      if (input.has(pu))
		med.untake(input(pu));
	      if (output.has(p))
		output(p) = med.to_result();
	    }
	}

      }; // end of median_dir_t



      template <typename I, typename O>
      void median_dir_(const Image<I>& input, unsigned dir, unsigned length, O& output)
      {
	median_dir_t<I,O> f(exact(input), dir, length, output);
	canvas::dirbrowsing(f);
      }


      template <typename I, typename W, typename O>
      void median_(const Image<I>& input, const Window<W>& win, O& output)
      {
	// FIXME: resize border!
	median_t<I,W,O> f(exact(input), exact(win), output);
	canvas::browsing::snake_fwd(f);
      }


#  ifdef MLN_CORE_WIN_HLINE2D_HH
      template <typename I, typename O>
      void median_(const Image<I>& input, const win::hline2d& win, O& output)
      {
	median_dir(input, 1, win.length(), output); // FIXME: Make 1 explicit!
      }
#  endif

#  ifdef MLN_CORE_WIN_VLINE2D_HH
      template <typename I, typename O>
      void median_(const Image<I>& input, const win::vline2d& win, O& output)
      {
	median_dir(input, 0, win.length(), output);
      }
#  endif


    } // end of namespace mln::level::impl



    // Facades.

    template <typename I, typename W, typename O>
    void median(const Image<I>& input, const Window<W>& win,
		Image<O>& output)
    {
      mln_assertion(exact(output).domain() == exact(input).domain());
      impl::median_(exact(input), exact(win), exact(output)); 
    }

    template <typename I, typename O>
    void median_dir(const Image<I>& input, unsigned dir, unsigned length,
		    Image<O>& output)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      typedef mln_point(I) P;
      mln_precondition(dir < P::dim);
      mln_precondition(length % 2 == 1);
      impl::median_dir_(exact(input), dir, length, exact(output)); 
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_MEDIAN_HH
