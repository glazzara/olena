// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_EROSION_SPE_HH
# define MLN_MORPHO_EROSION_SPE_HH

# ifndef MLN_MORPHO_EROSION_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_MORPHO_EROSION_HH

# include <mln/win/octagon2d.hh>
# include <mln/win/rectangle2d.hh>
# include <mln/geom/shift.hh>
# include <mln/accu/min_h.hh>
# include <mln/set/diff.hh>
# include <mln/canvas/browsing/snake_fwd.hh>


/*! \file mln/morpho/erosion.spe.hh
 *
 * \brief Specialization for mln::morpho::erosion.
 *
 *
 */

# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace morpho
  {

    // Fwd decl of the facade.
    template <typename I, typename W>
    mln_concrete(I)
    erosion(const Image<I>& input, const Window<W>& win);


    namespace impl
    {


      namespace generic
      {
	// Fwd decl.
	// Implementation is in mln/morpho/erosion.hh.
	template <typename I, typename W>
	mln_concrete(I)
	erosion_on_function(const Image<I>& input_, const Window<W>& win_);

	// Fwd decl.
	// Implementation is in mln/morpho/erosion.hh.
	template <typename I, typename W>
	mln_concrete(I)
	erosion_on_set(const Image<I>& input_, const Window<W>& win_);
      }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_on_function_fastest(const Image<I>& input_, const Window<W>& win_)
      {
	trace::entering("morpho::impl::erosion_on_function_fastest");

	typedef mln_concrete(I) O;
	const I& input = exact(input_);
	const W& win = exact(win_);
	O output;

	initialize(output, input);
	border::fill(input, mln_max(mln_value(I)));

	mln_pixter(const I) p(input);
	mln_pixter(O) o(output);
	mln_qixter(const I, W) q(p, win);
	accu::min_<mln_value(I)> min;
	for_all_2(p, o)
	{
	  min.init();
	  for_all(q)
	    min.take(q.val());
	  o.val() = min.to_result();
	}

	trace::exiting("morpho::impl::erosion_on_function_fastest");

	return output;
      }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_on_set_fastest(const Image<I>& input_, const Window<W>& win_)
      {
	trace::entering("morpho::impl::erosion_on_set_fastest");

	typedef mln_concrete(I) O;
	const I& input = exact(input_);
	const W& win = exact(win_);
	O output;

	initialize(output, input);
	border::fill(input, true);

	mln_pixter(const I) p(input);
	mln_pixter(O) p_out(output);
	mln_qixter(const I, W) q(p, win);
	for_all_2(p, p_out)
	{
	  for_all(q)
	    if (q.val() == false)
	      break;
	  p_out.val() = ! q.is_valid();
	}

	trace::exiting("morpho::impl::erosion_on_set_fastest");

	return output;
      }


      template <typename I, typename W>
      mln_concrete(I)
      erosion_on_set_centered(const Image<I>& input_, const Window<W>& win_)
      {
	trace::entering("morpho::impl::erosion_on_set_centered");

	typedef mln_concrete(I) O;
	const I& input = exact(input_);
	const W& win = exact(win_);
	O output;

	border::fill(input, true);

	output = clone(input);
	mln_piter(I) p(input.domain());
	mln_qiter(W) q(win, p);
	for_all(p)
	  if (input(p) == true)
	    for_all(q) if (input.has(q))
	      if (input(q) == false)
	      {
		output(p) = false;
		break;
	      }
	trace::exiting("morpho::impl::erosion_on_set_centered");

	return output;
      }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_on_set_centered_fastest(const Image<I>& input_, const Window<W>& win_)
      {
	trace::entering("morpho::impl::erosion_on_set_centered_fastest");

	typedef mln_concrete(I) O;
	const I& input = exact(input_);
	const W& win = exact(win_);
	O output;

	border::fill(input, true);

	output = clone(input);
	mln_pixter(const I) p(input);
	mln_qixter(const I, W) q(p, win);
	mln_pixter(O) p_out(output);
	for_all_2(p, p_out)
	  if (p.val() == true)
	    for_all(q)
	      if (q.val() == false)
	      {
		p_out.val() = false;
		break;
	      }
	trace::exiting("morpho::impl::erosion_on_set_centered_fastest");

	return output;
      }


      template <typename I>
      inline
      mln_concrete(I)
      erosion_rectangle2d(const Image<I>& input_, const win::rectangle2d& win)
      {
	trace::entering("morpho::impl::erosion_rectangle2d");

	const I& input = exact(input_);

	mln_concrete(I) temp, output;
	temp   = morpho::erosion(input, win::hline2d(win.width()));
	output = morpho::erosion(temp,  win::vline2d(win.height()));

	trace::exiting("morpho::impl::erosion_rectangle2d");
	return output;
      }


      template <typename I>
      inline
      mln_concrete(I)
      erosion_octagon2d(const Image<I>& input_, const win::octagon2d& win)
      {
	trace::entering("morpho::impl::erosion_octagon2d");

	const I& input = exact(input_);

	const unsigned len = win.length() / 3 + 1;

	mln_concrete(I) temp_1, temp_2, output;
	temp_1 = morpho::erosion(input,  win::hline2d(len));
	temp_2 = morpho::erosion(temp_1, win::vline2d(len));
	temp_1 = morpho::erosion(temp_2, win::diag2d(len));
	output = morpho::erosion(temp_1, win::backdiag2d(len));

	trace::exiting("morpho::impl::erosion_(win::octagon2d)");
	return output;
      }

      template <typename I, typename W>
      struct erosion_arbitrary_2d_fastest_functor
      {
	const I& input;
	const W& win;
	mln_concrete(I) output;
	accu::min_h<mln_value(I)> min;

	mln_psite(I) p;

	window2d
	win_left,
	  win_right,
	  win_bot,
	  win_top;

	mln_qixter(const I, window2d)
	q_l,
	  q_r,
	  q_top,
	  q_bot;

	erosion_arbitrary_2d_fastest_functor(const I& input, const W& win)
	  : input(input),
	    win(win),
	    min(input.values()),
	    win_left(set::diff(geom::shift(win, left), win)),
	    win_right(set::diff(win, geom::shift(win, left))),
	    win_bot(set::diff(win, geom::shift(win, up))),
	    win_top(set::diff(geom::shift(win, up), win)),
	    q_l(input, win_left, p),
	    q_r(input, win_right, p),
	    q_top(input, win_top, p),
	    q_bot(input, win_bot, p)
	{ }

	void init()
	{
	  initialize(output, input);
	  min.init();
	  p = input.domain().pmin() + up;
	  mln_qixter(const I, W) q(input, win, p);
	  for_all(q)
	    min.take(q.val());
	}

	void fwd()
	{
	  ++p.col();
	  for_all(q_l)
	    min.untake(q_l.val());
	  for_all(q_r)
	    min.take(q_r.val());
	  output(p) = min;
	}

	void bkd()
	{
	  --p.col();
	  for_all(q_r)
	    min.untake(q_r.val());
	  for_all(q_l)
	    min.take(q_l.val());
	  output(p) = min;
	}

	void down()
	{
	  ++p.row();
	  for_all(q_top)
	    min.untake(q_top.val());
	  for_all(q_bot)
	    min.take(q_bot.val());
	  output(p) = min;
	}

      };

      template <typename I, typename W>
      inline
      mln_concrete(I)
      erosion_arbitrary_2d_fastest(const Image<I>& input, const Window<W>& win)
      {
	trace::entering("morpho::impl:erosion_arbitrary_2d_fastest");

	typedef erosion_arbitrary_2d_fastest_functor<I, W> F;
	mlc_equal(W, win::octagon2d)::check();
	F f(input, win);
	canvas::browsing::snake_fwd(f);

	trace::exiting("morpho::impl:erosion_arbitrary_2d_fastest");

	return f.output;
      }


      template <typename I, typename W>
      struct erosion_arbitrary_2d_functor
      {
	const I& input;
	const W& win;
	mln_concrete(I) output;
	accu::min_h<mln_value(I)> min;

	mln_psite(I) p;

	window2d
	win_left,
	  win_right,
	  win_bot,
	  win_top;

	mln_qiter(window2d)
	q_l,
	  q_r,
	  q_top,
	  q_bot;

	erosion_arbitrary_2d_functor(const I& input, const W& win)
	  : input(input),
	    win(win),
	    min(input.values()),
	    win_left(set::diff(geom::shift(win, left), win)),
	    win_right(set::diff(win, geom::shift(win, left))),
	    win_bot(set::diff(win, geom::shift(win, up))),
	    win_top(set::diff(geom::shift(win, up), win)),
	    q_l(win_left, p),
	    q_r(win_right, p),
	    q_top(win_top, p),
	    q_bot(win_bot, p)
	{ }

	void init()
	{
	  initialize(output, input);
	  min.init();
	  p = input.domain().pmin() + up;
	  mln_qiter(W) q(win, p);
	  for_all(q)
	    min.take(input(q));
	}

	void fwd()
	{
	  ++p.col();
	  for_all(q_l)
	    min.untake(input(q_l));
	  for_all(q_r)
	    min.take(input(q_r));
	  output(p) = min;
	}

	void bkd()
	{
	  --p.col();
	  for_all(q_r)
	    min.untake(input(q_r));
	  for_all(q_l)
	    min.take(input(q_l));
	  output(p) = min;
	}

	void down()
	{
	  ++p.row();
	  for_all(q_top)
	    min.untake(input(q_top));
	  for_all(q_bot)
	    min.take(input(q_bot));
	  output(p) = min;
	}

      };

      template <typename I, typename W>
      inline
      mln_concrete(I)
      erosion_arbitrary_2d(const Image<I>& input, const Window<W>& win)
      {
	trace::entering("morpho::impl:erosion_arbitrary_2d");

	typedef erosion_arbitrary_2d_functor<I, W> F;
	mlc_equal(W, win::octagon2d)::check();
	F f(input, win);
	canvas::browsing::snake_fwd(f);

	trace::exiting("morpho::impl:erosion_arbitrary_2d");

	return f.output;
      }

    } // end of namespace mln::morpho::impl


    namespace internal
    {
      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch(const Image<I>& input, const Window<W>& win)
      {
	if (mlc_equal(mln_trait_image_kind(I)(),
		      trait::image::kind::logic)::value == true)
	  if (mlc_equal(mln_trait_image_speed(I)(),
			trait::image::speed::fastest)::value == true)
	    impl::erosion_on_set_fastest(input, win);
	  else
	    impl::generic::erosion_on_set(input, win);
	else
	  if (mlc_equal(mln_trait_image_speed(I)(),
			trait::image::speed::fastest)::value == true)
	    impl::erosion_on_function_fastest(input, win);
	  else
	    impl::generic::erosion_on_function(input, win);
      }

    } // end of namespace mln::morpho::internal

  } // end of namespace mln::morpho

} // end of namespace mln


# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_MORPHO_EROSION_SPE_HH
