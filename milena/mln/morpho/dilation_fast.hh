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

#ifndef MLN_MORPHO_DILATION_FAST_HH
# define MLN_MORPHO_DILATION_FAST_HH

/*! \file mln/morpho/dilation_fast.hh
 *
 * \brief Ero filtering of an image.
 *
 * \todo Add Fastest_Image versions.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/window.hh>
# include <mln/geom/shift.hh>
# include <mln/set/diff.hh>

# include <mln/canvas/browsing/snake_fwd.hh>
# include <mln/accu/max_h.hh>


namespace mln
{

  namespace morpho
  {

    /*! Compute in \p output the dilation filter of image \p input by
     *  the window \p win.
     *
     * \param[in] input The image to be filtered.
     * \param[in] win The window.
     * \param[out] output The output image.
     *
     * \pre \p input and \p output have to be initialized.
     */
    template <typename I, typename W, typename O>
    void dilation_fast(const Image<I>& input, const Window<W>& win,
		      Image<O>& output);




# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {


      // Functors.


      template <typename I, typename W, typename O>
      struct dilation_fast_t
      { 
	typedef mln_point(I)  P;
	typedef mln_dpoint(I) D;

	// i/o

	const I& input;
	const W& win;
	O& output;

	// aux data

	accu::max_h<mln_vset(I)> max;
	P p;
	window<D>    win_fp, win_fm, win_bp, win_bm, win_dp, win_dm;
	mln_qiter(W)   q_fp,   q_fm,   q_bp,   q_bm,   q_dp,   q_dm;

	// ctor

	dilation_fast_t(const I& input_, const W& win_, O& output_)
	  :
	  // i/o
	  input(exact(input_)),
	  win(exact(win_)),
	  output(exact(output_)),
	  // aux data
	  max(input.values()),
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
	  max.init();
	  mln_qiter(W) q(win, p);
	  for_all(q) if (input.has(q))
	    max.take(input(q));
	}

	void down()
	{
	  for_all(q_dm) if (input.has(q_dm))
	    max.untake(input(q_dm));
	  for_all(q_dp) if (input.has(q_dp))
	    max.take(input(q_dp));
	  output(p) = max.to_result();
	}

	void fwd()
	{
	  for_all(q_fm) if (input.has(q_fm))
	    max.untake(input(q_fm));
	  for_all(q_fp) if (input.has(q_fp))
	    max.take(input(q_fp));
	  output(p) = max.to_result();
	}

	void bkd()
	{
	  for_all(q_bm) if (input.has(q_bm))
	    max.untake(input(q_bm));
	  for_all(q_bp) if (input.has(q_bp))
	    max.take(input(q_bp));
	  output(p) = max.to_result();
	}

      }; // end of dilation_fast_t


      template <typename I, typename W, typename O>
      void dilation_fast_(const Image<I>& input, const Window<W>& win, O& output)
      {
	// FIXME: resize border!
	dilation_fast_t<I,W,O> f(exact(input), exact(win), output);
	canvas::browsing::snake_fwd(f);
      }

    } // end of namespace mln::level::impl


    // Facades.

    template <typename I, typename W, typename O>
    void dilation_fast(const Image<I>& input, const Window<W>& win,
		      Image<O>& output)
    {
      mln_assertion(exact(output).domain() == exact(input).domain());
      impl::dilation_fast_(exact(input), exact(win), exact(output)); 
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_DILATION_FAST_HH
