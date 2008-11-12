// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

# include <mln/core/alias/window2d.hh>
# include <mln/core/alias/dpoint2d.hh>

# include <mln/win/octagon2d.hh>
# include <mln/win/rectangle2d.hh>

# include <mln/win/shift.hh>
# include <mln/win/diff.hh>

# include <mln/accu/min_h.hh>
# include <mln/accu/line.hh>

# include <mln/canvas/browsing/snake_fwd.hh>
# include <mln/canvas/browsing/snake_generic.hh>
# include <mln/canvas/browsing/directional.hh>
# include <mln/canvas/browsing/diagonal2d.hh>
# include <mln/canvas/browsing/backdiagonal2d.hh>


/// \file mln/morpho/erosion.spe.hh
///
/// Specialization for mln::morpho::erosion.
///
/// \todo Warning: we should also have the "arbitrary" versions working
/// on sets (not only on functions!)
///
/// \todo HERE Factor dispatch for lines...


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

	extension::adjust_fill(input, win, mln_max(mln_value(I)));

	O output;
	initialize(output, input);

	mln_pixter(const I) p(input);
	mln_pixter(O) o(output);
	mln_qixter(const I, W) q(p, win);
	accu::min<mln_value(I)> min;
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

	extension::adjust_fill(input, win, true);

	O output;
	initialize(output, input);

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

	extension::adjust_fill(input, win, true);

	O output;
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

	extension::adjust_fill(input, win, true);

	O output;
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


      template <typename I, typename G, unsigned Dir, typename C>
      inline
      mln_concrete(I)
      erosion_line_on_function(const Image<I>& input_, const win::line<G,Dir,C>& win)
      {
	trace::entering("morpho::impl::erosion_line");

	typedef mln_site(I) P;
	enum { dim = P::dim };

	const I& input = exact(input_);

	mln_concrete(I) output;
	initialize(output, input);

	mln_psite(I)
	  pmin = input.domain().pmin(),
	  pmax = input.domain().pmax(),
	  p = pmin;

	const unsigned len = input.domain().len(Dir);
	const unsigned win_half_length = win.length() / 2;

	do
	  {
	    accu::line< accu::meta::min_h, Dir >(input,
						 p, len,
						 win_half_length,
						 output);

	    for (int c = dim - 1; c >= 0; --c)
	      {
		if (c == int(Dir))
		  continue;
		if (p[c] != pmax[c])
		  {
		    ++p[c];
		    break;
		  }
		p[c] = pmin[c];
	      }
	  }
	while (p != pmin);

	trace::exiting("morpho::impl::erosion_line");
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

	const unsigned len = (win.length() + 2) / 3;

	mln_concrete(I) output;
	output = morpho::erosion(input,  win::hline2d(len));
	output = morpho::erosion(output,  win::vline2d(len));
	output = morpho::erosion(output, win::diag2d(len));
	output = morpho::erosion(output, win::backdiag2d(len));

	trace::exiting("morpho::impl::erosion_(win::octagon2d)");
	return output;
      }

      template <typename I, typename W, typename A>
      struct erosion_arbitrary_2d_fastest_functor
      {
	typedef erosion_arbitrary_2d_fastest_functor<I,W,A> self;
	typedef void (self::*move_fun)();
	typedef mln_deduce(I, psite, delta) dpsite;

	const I& input;
	const W& win;
	mln_concrete(I) output;
	A accu;

	mln_psite(I) p;

	window2d
	win_left_fwd,
	  win_right_fwd,
	  win_left_bkd,
	  win_right_bkd,
	  win_bot_up,
	  win_top_up,
	  win_bot_down,
	  win_top_down;

	mln_qixter(const I, window2d)
	q_l_fwd,
	  q_r_fwd,
	  q_l_bkd,
	  q_r_bkd,
	  q_top_up,
	  q_bot_up,
	  q_top_down,
	  q_bot_down;


	std::vector<move_fun> moves;
	std::vector<dpsite> dps;

	erosion_arbitrary_2d_fastest_functor(const I& input, const W& win)
	  : input(input),
	    win(win),
	    accu(),

	    win_left_fwd(win::shift(win, mln::left) - win),
	    win_right_fwd(win - win::shift(win, mln::left)),
	    win_left_bkd(win::shift(win_left_fwd, mln::right)),
	    win_right_bkd(win::shift(win_right_fwd, mln::right)),

	    win_bot_up(win::shift(win, mln::down) - win),
	    win_top_up(win - win::shift(win, mln::down)),
	    win_bot_down(win::shift(win_bot_up, mln::up)),
	    win_top_down(win::shift(win_top_up, mln::up)),

	    q_l_fwd(input, win_left_fwd, p),
	    q_r_fwd(input, win_right_fwd, p),
	    q_l_bkd(input, win_left_bkd, p),
	    q_r_bkd(input, win_right_bkd, p),

	    q_top_up(input, win_top_up, p),
	    q_bot_up(input, win_bot_up, p),
	    q_top_down(input, win_top_down, p),
	    q_bot_down(input, win_bot_down, p),

	    moves(3),
	    dps(3)
	{
	  if (win_bot_up.size()   + win_top_up.size() +
	      win_bot_down.size() + win_top_down.size() <
	      win_left_fwd.size() + win_right_fwd.size() +
	      win_left_bkd.size() + win_right_bkd.size())
	  {
	    // Vertical snake
	    dps[0] = mln::right;
	    dps[1] = mln::down;
	    dps[2] = mln::up;
	    moves[0] = &self::right;
	    moves[1] = &self::down;
	    moves[2] = &self::up;
	  }
	  else
	  {
	    // Horizontal snake
	    dps[0] = mln::down;
	    dps[1] = mln::right;
	    dps[2] = mln::left;
	    moves[0] = &self::down;
	    moves[1] = &self::right;
	    moves[2] = &self::left;
	  }
	}

	void init()
	{
	  // extension::adjust_fill is performed in the routine
	  // because it has to be done before the initialization of
	  // the fast iterators (q_*).
	  initialize(output, input);
	  accu.init();
	  p = input.domain().pmin() - dps[0];
	  mln_qixter(const I, W) q(input, win, p);
	  for_all(q)
	    accu.take(q.val());
	  p = input.domain().pmin();
	}

	void right()
	{
	  for_all(q_l_fwd)
	    accu.untake(q_l_fwd.val());
	  for_all(q_r_fwd)
	    accu.take(q_r_fwd.val());
	  output(p) = accu;
	}

	void left()
	{
	  for_all(q_r_bkd)
	    accu.untake(q_r_bkd.val());
	  for_all(q_l_bkd)
	    accu.take(q_l_bkd.val());
	  output(p) = accu;
	}

	void down()
	{
	  for_all(q_top_down)
	    accu.untake(q_top_down.val());
	  for_all(q_bot_down)
	    accu.take(q_bot_down.val());
	  output(p) = accu;
	}

	void up()
	{
	  for_all(q_bot_up)
	    accu.untake(q_bot_up.val());
	  for_all(q_top_up)
	    accu.take(q_top_up.val());
	  output(p) = accu;
	}

      };

      template <typename I, typename W>
      inline
      mln_concrete(I)
      erosion_arbitrary_2d_fastest(const Image<I>& input, const Window<W>& win)
      {
	trace::entering("morpho::impl:erosion_arbitrary_2d_fastest");

	typedef mlc_is(mln_trait_image_kind(I),
		       trait::image::kind::binary) is_binary;
	typedef mlc_if(is_binary, accu::land, accu::min_h<mln_value(I)>) A;

	extension::adjust_fill(input,
			       geom::delta(win) + 1,
			       A());

	typedef erosion_arbitrary_2d_fastest_functor<I, W, A> F;
	F f(exact(input), exact(win));
	canvas::browsing::snake_generic(f);

	trace::exiting("morpho::impl:erosion_arbitrary_2d_fastest");

	return f.output;
      }


      template <typename I, typename W, typename A>
      struct erosion_arbitrary_2d_functor
      {
	typedef erosion_arbitrary_2d_functor<I,W, A> self;
	typedef void (self::*move_fun)();
	typedef mln_deduce(I, psite, delta) dpsite;

	const I& input;
	const W& win;
	mln_concrete(I) output;
	A accu;

	mln_psite(I) p;

	window2d
	win_left_fwd,
	  win_right_fwd,
	  win_left_bkd,
	  win_right_bkd,
	  win_bot_up,
	  win_top_up,
	  win_bot_down,
	  win_top_down;

	mln_qiter(window2d)
	q_l_fwd,
	  q_r_fwd,
	  q_l_bkd,
	  q_r_bkd,
	  q_top_up,
	  q_bot_up,
	  q_top_down,
	  q_bot_down;

	std::vector<move_fun> moves;
	std::vector<dpsite> dps;

	erosion_arbitrary_2d_functor(const I& input, const W& win)
	  : input(input),
	    win(win),
	    accu(),

	    win_left_fwd(win::shift(win, mln::left) - win),
	    win_right_fwd(win - win::shift(win, mln::left)),
	    win_left_bkd(win::shift(win_left_fwd, mln::right)),
	    win_right_bkd(win::shift(win_right_fwd, mln::right)),

	    win_bot_up(win::shift(win, mln::down) - win),
	    win_top_up(win - win::shift(win, mln::down)),
	    win_bot_down(win::shift(win_bot_up, mln::up)),
	    win_top_down(win::shift(win_top_up, mln::up)),

	    q_l_fwd(win_left_fwd, p),
	    q_r_fwd(win_right_fwd, p),
	    q_l_bkd(win_left_bkd, p),
	    q_r_bkd(win_right_bkd, p),

	    q_top_up(win_top_up, p),
	    q_bot_up(win_bot_up, p),
	    q_top_down(win_top_down, p),
	    q_bot_down(win_bot_down, p),

	    moves(3),
	    dps(3)
	{
	  if (win_bot_up.size()   + win_top_up.size() +
	      win_bot_down.size() + win_top_down.size() <
	      win_left_fwd.size() + win_right_fwd.size() +
	      win_left_bkd.size() + win_right_bkd.size())
	  {
	    // Vertical snake
	    dps[0] = mln::right;
	    dps[1] = mln::down;
	    dps[2] = mln::up;
	    moves[0] = &self::right;
	    moves[1] = &self::down;
	    moves[2] = &self::up;
	  }
	  else
	  {
	    // Horizontal snake
	    dps[0] = mln::down;
	    dps[1] = mln::right;
	    dps[2] = mln::left;
	    moves[0] = &self::down;
	    moves[1] = &self::right;
	    moves[2] = &self::left;
	  }
	}

	void init()
	{
	  extension::adjust_fill(input, win, accu);
	  initialize(output, input);
	  accu.init();
	  p = input.domain().pmin() - dps[0];
	  mln_qiter(W) q(win, p);
	  for_all(q)
	    accu.take(input(q));
	  p = input.domain().pmin();
	}

	void right()
	{
	  for_all(q_l_fwd)
	    accu.untake(input(q_l_fwd));
	  for_all(q_r_fwd)
	    accu.take(input(q_r_fwd));
	  output(p) = accu;
	}

	void left()
	{
	  for_all(q_r_bkd)
	    accu.untake(input(q_r_bkd));
	  for_all(q_l_bkd)
	    accu.take(input(q_l_bkd));
	  output(p) = accu;
	}

	void down()
	{
	  for_all(q_top_down)
	    accu.untake(input(q_top_down));
	  for_all(q_bot_down)
	    accu.take(input(q_bot_down));
	  output(p) = accu;
	}

	void up()
	{
	  for_all(q_bot_up)
	    accu.untake(input(q_bot_up));
	  for_all(q_top_up)
	    accu.take(input(q_top_up));
	  output(p) = accu;
	}

      };

      template <typename I, typename W>
      inline
      mln_concrete(I)
      erosion_arbitrary_2d(const Image<I>& input, const Window<W>& win)
      {
	trace::entering("morpho::impl:erosion_arbitrary_2d");

	typedef erosion_arbitrary_2d_functor<I, W, accu::min_h<mln_value(I)> > F;
	F f(exact(input), exact(win));
	canvas::browsing::snake_generic(f);

	trace::exiting("morpho::impl:erosion_arbitrary_2d");

	return f.output;
      }


      template <typename Dp>
      Dp dp_directional(int dir)
      {
	Dp dp = literal::zero;
	dp[dir] = 1;
	return dp;
      }

      template <typename I_, typename W, typename A>
      struct erosion_directional_nd_functor
      {
	typedef I_ I;
	typedef mln_deduce(I, psite, delta) dpsite;

	const I& input;
	const W& win;
	mln_concrete(I) output;
	A accu;
	enum { dim = I::site::dim };

	mln_psite(I) p;
	unsigned dir;

	window2d
	win_left,
	  win_right;

	mln_qiter(window2d)
	q_l,
	  q_r;

	erosion_directional_nd_functor(const I& input, const W& win, int dir)
	  : input(input),
	    win(win),
	    accu(),
	    dir(dir),
	    win_left(win::shift(win, -dp_directional<dpsite>(dir)) - win),
	    win_right(win - win::shift(win, -dp_directional<dpsite>(dir))),
	    q_l(win_left, p),
	    q_r(win_right, p)
	{

	}

	void init()
	{
	  extension::adjust_fill(input, win, accu);
	  initialize(output, input);
	}

	void init_run()
	{
	  accu.init();
	  p[dir]--;
	  mln_qiter(W) q(win, p);
	  for_all(q) if (input.has(q))
	    accu.take(input(q));
	  p[dir]++;
	}

	void next()
	{
	  for_all(q_l) if (input.has(q_l))
	    accu.untake(input(q_l));
	  for_all(q_r) if (input.has(q_r))
	    accu.take(input(q_r));
	  output(p) = accu;
	}

	void final()
	{
	}

      };

      template <typename I, typename W>
      inline
      mln_concrete(I)
      erosion_directional_nd(const Image<I>& input, const Window<W>& win, unsigned dir)
      {
	trace::entering("morpho::impl:erosion_directional_nd");

	typedef mlc_is(mln_trait_image_kind(I),
		       trait::image::kind::binary) is_binary;
	typedef mlc_if(is_binary, accu::land, accu::min_h<mln_value(I)>) A;

	typedef erosion_directional_nd_functor<I, W, A> F;
	F f(exact(input), exact(win), dir);
	canvas::browsing::directional(f);

	trace::exiting("morpho::impl:erosion_directional_nd");

	return f.output;
      }


      template <typename I_, typename W, typename A>
      struct erosion_directional_nd_fastest_functor
      {
	typedef I_ I;
	typedef mln_deduce(I, psite, delta) dpsite;

	const I& input;
	const W& win;
	mln_concrete(I) output;
	A accu;

	mln_psite(I) p;
	enum { dim = I::site::dim };
	unsigned dir;

	window2d win_left, win_right;

	mln_qixter(const I, window2d) q_l, q_r;

	erosion_directional_nd_fastest_functor(const I& input, const W& win, unsigned dir)
	  : input(input),
	    win(win),
	    accu(),
	    dir(dir),
	    win_left(win::shift(win, -dp_directional<dpsite>(dir)) - win),
	    win_right(win - win::shift(win, -dp_directional<dpsite>(dir))),
	    q_l(input, win_left, p),
	    q_r(input, win_right, p)
	{
	}

	void init()
	{
	  // extension::adjust_fill is performed in the routine
	  // because it has to be done before the initialization of
	  // the fast iterators (q_l and q_r).
	  initialize(output, input);
	}

	void next()
	{
	  for_all(q_l)
	    accu.untake(q_l.val());
	  for_all(q_r)
	    accu.take(q_r.val());
	  output(p) = accu;
	}


	void init_run()
	{
	  accu.init();
	  p[dir]--;
	  mln_qixter(const I, W) q(input, win, p);
	  for_all(q)
	    accu.take(q.val());
	  p[dir]++;
	}

	void final()
	{
	}

      };


      template <typename I, typename W>
      inline
      mln_concrete(I)
      erosion_directional_nd_fastest(const Image<I>& input, const Window<W>& win, unsigned dir)
      {
	trace::entering("morpho::impl:erosion_directional_nd_fastest");

	typedef mlc_is(mln_trait_image_kind(I),
		       trait::image::kind::binary) is_binary;
	typedef mlc_if(is_binary, accu::land, accu::min_h<mln_value(I)>) A;

	extension::adjust_fill(input,
			       geom::delta(win) + 1,
			       A());

	typedef erosion_directional_nd_fastest_functor<I, W, A> F;
	F f(exact(input), exact(win), dir);
	canvas::browsing::directional(f);

	trace::exiting("morpho::impl:erosion_directional_nd_fastest");

	return f.output;
      }


      // Diagonal2d non fastest.
      template <typename I_, typename W, typename A>
      struct erosion_diagonal2d_functor
      {
	typedef I_ I;
	typedef mln_deduce(I, psite, delta) dpsite;

	const I& input;
	const W& win;
	mln_concrete(I) output;
	A accu;

	mln_psite(I) p;
	enum { dim = I::site::dim };
	unsigned dir;

	window2d win_left, win_right;

	mln_qiter(window2d) q_l, q_r;

	erosion_diagonal2d_functor(const I& input, const W& win)
	  : input(input),
	    win(win),
	    accu(),
	    dir(dir),
	    win_left(win::shift(win, dpsite(1, -1)) - win),
	    win_right(win - win::shift(win, dpsite(1, -1))),
	    q_l(win_left, p),
	    q_r(win_right, p)
	{
	}

	void init()
	{
	  extension::adjust_fill(input, win, accu);
	  initialize(output, input);
	}

	void next()
	{
	  for_all(q_l)
	    accu.untake(input(q_l));
	  for_all(q_r)
	    accu.take(input(q_r));
	  output(p) = accu;
	}


	void init_diag()
	{
	  accu.init();
	  p = p - dpsite(-1, 1);
	  mln_qiter(W) q(win, p);
	  for_all(q)
	    accu.take(input(q));
	  p = p + dpsite(-1, 1);
	}

	void final()
	{
	}

      };

      // Backdiagonal2d non fastest.
      template <typename I, typename W>
      inline
      mln_concrete(I)
      erosion_diagonal2d(const Image<I>& input, const Window<W>& win)
      {
	trace::entering("morpho::impl:erosion_diagonal2d");

	typedef mlc_is(mln_trait_image_kind(I),
		       trait::image::kind::binary) is_binary;
	typedef mlc_if(is_binary, accu::land, accu::min_h<mln_value(I)>) A;

	typedef erosion_diagonal2d_functor<I, W, A> F;
	F f(exact(input), exact(win));
	canvas::browsing::diagonal2d(f);

	trace::exiting("morpho::impl:erosion_diagonal2d");

	return f.output;
      }



      template <typename I_, typename W, typename A>
      struct erosion_backdiagonal2d_functor
      {
	typedef I_ I;
	typedef mln_deduce(I, psite, delta) dpsite;

	const I& input;
	const W& win;
	mln_concrete(I) output;
	A accu;

	mln_psite(I) p;
	enum { dim = I::site::dim };
	unsigned dir;

	window2d win_left, win_right;

	mln_qiter(window2d) q_l, q_r;

	erosion_backdiagonal2d_functor(const I& input, const W& win)
	  : input(input),
	    win(win),
	    accu(),
	    dir(dir),
	    win_left(win::shift(win, dpsite(-1, -1)) - win),
	    win_right(win - win::shift(win, dpsite(-1, -1))),
	    q_l(win_left, p),
	    q_r(win_right, p)
	{
	}

	void init()
	{
	  extension::adjust_fill(input, win, accu);
	  initialize(output, input);
	}

	void next()
	{
	  for_all(q_l)
	    accu.untake(input(q_l));
	  for_all(q_r)
	    accu.take(input(q_r));
	  output(p) = accu;
	}


	void init_diag()
	{
	  accu.init();
	  p = p - dpsite(1, 1);
	  mln_qiter(W) q(win, p);
	  for_all(q)
	    accu.take(input(q));
	  p = p + dpsite(1, 1);
	}

	void final()
	{
	}

      };

      template <typename I, typename W>
      inline
      mln_concrete(I)
      erosion_backdiagonal2d(const Image<I>& input, const Window<W>& win)
      {
	trace::entering("morpho::impl:erosion_backdiagonal2d");

	typedef mlc_is(mln_trait_image_kind(I),
		       trait::image::kind::binary) is_binary;
	typedef mlc_if(is_binary, accu::land, accu::min_h<mln_value(I)>) A;

	typedef erosion_backdiagonal2d_functor<I, W, A> F;
	F f(exact(input), exact(win));
	canvas::browsing::backdiagonal2d(f);

	trace::exiting("morpho::impl:erosion_backdiagonal2d");

	return f.output;
      }



      // Diagonal2d fastest.
      template <typename I_, typename W, typename A>
      struct erosion_diagonal2d_fastest_functor
      {
	typedef I_ I;
	typedef mln_deduce(I, psite, delta) dpsite;

	const I& input;
	const W& win;
	mln_concrete(I) output;
	A accu;

	mln_psite(I) p;
	enum { dim = I::site::dim };
	unsigned dir;

	window2d win_left, win_right;

	mln_qixter(const I, window2d) q_l, q_r;

	erosion_diagonal2d_fastest_functor(const I& input, const W& win)
	  : input(input),
	    win(win),
	    accu(),
	    dir(dir),
	    win_left(win::shift(win, dpsite(1, -1)) - win),
	    win_right(win - win::shift(win, dpsite(1, -1))),
	    q_l(input, win_left, p),
	    q_r(input, win_right, p)
	{
	}

	void init()
	{
	  extension::adjust_fill(input, win, accu);
	  initialize(output, input);
	}

	void next()
	{
	  for_all(q_l)
	    accu.untake(q_l.val());
	  for_all(q_r)
	    accu.take(q_r.val());
	  output(p) = accu;
	}


	void init_diag()
	{
	  accu.init();
	  p = p - dpsite(-1, 1);
	  mln_qixter(const I, W) q(input, win, p);
	  for_all(q)
	    accu.take(q.val());
	  p = p + dpsite(-1, 1);
	}

	void final()
	{
	}

      };

      template <typename I, typename W>
      inline
      mln_concrete(I)
      erosion_diagonal2d_fastest(const Image<I>& input, const Window<W>& win)
      {
	trace::entering("morpho::impl:erosion_diagonal2d_fastest");

	typedef mlc_is(mln_trait_image_kind(I),
		       trait::image::kind::binary) is_binary;
	typedef mlc_if(is_binary, accu::land, accu::min_h<mln_value(I)>) A;

	typedef erosion_diagonal2d_fastest_functor<I, W, A> F;
	F f(exact(input), exact(win));
	canvas::browsing::diagonal2d(f);

	trace::exiting("morpho::impl:erosion_diagonal2d_fastest");

	return f.output;
      }



      // Backdiagonal2d fastest.
      template <typename I_, typename W, typename A>
      struct erosion_backdiagonal2d_fastest_functor
      {
	typedef I_ I;
	typedef mln_deduce(I, psite, delta) dpsite;

	const I& input;
	const W& win;
	mln_concrete(I) output;
	A accu;

	mln_psite(I) p;
	enum { dim = I::site::dim };
	unsigned dir;

	window2d win_left, win_right;

	mln_qixter(const I, window2d) q_l, q_r;

	erosion_backdiagonal2d_fastest_functor(const I& input, const W& win)
	  : input(input),
	    win(win),
	    accu(),
	    dir(dir),
	    win_left(win::shift(win, dpsite(-1, -1)) - win),
	    win_right(win - win::shift(win, dpsite(-1, -1))),
	    q_l(input, win_left, p),
	    q_r(input, win_right, p)
	{
	}

	void init()
	{
	  extension::adjust_fill(input, win, accu);
	  initialize(output, input);
	}

	void next()
	{
	  for_all(q_l)
	    accu.untake(q_l.val());
	  for_all(q_r)
	    accu.take(q_r.val());
	  output(p) = accu;
	}


	void init_diag()
	{
	  accu.init();
	  p = p - dpsite(1, 1);
	  mln_qixter(const I, W) q(input, win, p);
	  for_all(q)
	    accu.take(q.val());
	  p = p + dpsite(1, 1);
	}

	void final()
	{
	}

      };

      template <typename I, typename W>
      inline
      mln_concrete(I)
      erosion_backdiagonal2d_fastest(const Image<I>& input, const Window<W>& win)
      {
	trace::entering("morpho::impl:erosion_backdiagonal2d_fastest");

	typedef mlc_is(mln_trait_image_kind(I),
		       trait::image::kind::binary) is_binary;
	typedef mlc_if(is_binary, accu::land, accu::min_h<mln_value(I)>) A;

	typedef erosion_backdiagonal2d_fastest_functor<I, W, A> F;
	F f(exact(input), exact(win));
	canvas::browsing::backdiagonal2d(f);

	trace::exiting("morpho::impl:erosion_backdiagonal2d_fastest");

	return f.output;
      }

    } // end of namespace mln::morpho::impl


    namespace internal
    {

      // dispatch for the generic version

      // FIXME: De-activate because when the window is multiple, the access
      // win.dp(i), used in dpoints_pixter, is impossible.

      //       template <typename I, typename W>
      //       mln_concrete(I)
      //       erosion_dispatch_for_generic(trait::image::kind::logic, // On sets.
      // 				   trait::image::speed::fastest,
      // 				   const I& input, const W& win)
      //       {
      // 	if (win.is_centered())
      // 	  return impl::erosion_on_set_centered_fastest(input, win);
      // 	else
      // 	  return impl::erosion_on_set_fastest(input, win);
      //       }

      //       template <typename I, typename W>
      //       mln_concrete(I)
      //       erosion_dispatch_for_generic(trait::image::kind::any, // On functions.
      // 				   trait::image::speed::fastest,
      // 				   const I& input, const W& win)
      //       {
      // 	return impl::erosion_on_function_fastest(input, win);
      //       }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_for_generic(trait::image::kind::logic, // On sets.
				   trait::image::speed::any,
				   const I& input, const W& win)
      {
	if (win.is_centered())
	  return impl::erosion_on_set_centered(input, win);
	else
	  return impl::generic::erosion_on_set(input, win);
      }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_for_generic(trait::image::kind::any, // On functions.
				   trait::image::speed::any,
				   const I& input, const W& win)
      {
	return impl::generic::erosion_on_function(input, win);
      }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_for_generic(const I& input, const W& win) // Entry point.
      {
	trace::entering("morpho::erosion_dispatch_for_generic");
	mln_concrete(I) ima =
	  erosion_dispatch_for_generic(mln_trait_image_kind(I)(),
				       mln_trait_image_speed(I)(),
				       input, win);
	trace::entering("morpho::erosion_dispatch_for_generic");
	return ima;
      }


      // dispatch for arbitrary elements

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_for_arbitrary(trait::image::speed::fastest,
				     const I& input, const W& win)
      {
	return impl::erosion_arbitrary_2d_fastest(input, win);
      }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_for_arbitrary(trait::image::speed::any,
				     const I& input, const W& win)
      {
	return impl::erosion_arbitrary_2d(input, win);
      }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_for_arbitrary(const I& input, const W& win)
      {
	trace::entering("morpho::erosion_dispatch_for_arbitrary");
	mln_concrete(I) ima =
	  erosion_dispatch_for_arbitrary(mln_trait_image_speed(I)(),
					 input, win);
	trace::exiting("morpho::erosion_dispatch_for_arbitrary");
	return ima;
      }



      // dispatch for directional_nd w.r.t. speed

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_for_directional(trait::image::speed::fastest,
				       const I& input, const W& win, unsigned dir)
      {
	return impl::erosion_directional_nd_fastest(input, win, dir);
      }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_for_directional(trait::image::speed::any,
				       const I& input, const W& win, unsigned dir)
      {
	return impl::erosion_directional_nd(input, win, dir);
      }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_for_directional(const I& input, const W& win, unsigned dir)
      {
	trace::entering("morpho::erosion_dispatch_for_directional");
	mln_concrete(I) ima =
	  erosion_dispatch_for_directional(mln_trait_image_speed(I)(),
					   input, win, dir);
	trace::exiting("morpho::erosion_dispatch_for_directional");
	return ima;
      }


      // dispatch for diagonal2d w.r.t. speed

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_for_diagonal2d(trait::image::speed::fastest,
				      const I& input, const W& win)
      {
	return impl::erosion_diagonal2d_fastest(input, win);
      }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_for_diagonal2d(trait::image::speed::any,
				       const I& input, const W& win)
      {
	return impl::erosion_diagonal2d(input, win);
      }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_for_diagonal2d(const I& input, const W& win)
      {
	trace::entering("morpho::erosion_dispatch_for_diagonal2d");
	mln_concrete(I) ima =
	  erosion_dispatch_for_diagonal2d(mln_trait_image_speed(I)(),
					  input, win);
	trace::exiting("morpho::erosion_dispatch_for_diagonal2d");
	return ima;
      }


      // dispatch for backdiagonal2d w.r.t. speed

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_for_backdiagonal2d(trait::image::speed::fastest,
				      const I& input, const W& win)
      {
	return impl::erosion_backdiagonal2d_fastest(input, win);
      }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_for_backdiagonal2d(trait::image::speed::any,
				       const I& input, const W& win)
      {
	return impl::erosion_backdiagonal2d(input, win);
      }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_for_backdiagonal2d(const I& input, const W& win)
      {
	trace::entering("morpho::erosion_dispatch_for_backdiagonal2d");
	mln_concrete(I) ima =
	  erosion_dispatch_for_backdiagonal2d(mln_trait_image_speed(I)(),
					      input, win);
	trace::exiting("morpho::erosion_dispatch_for_backdiagonal2d");
	return ima;
      }


      // dispatch w.r.t. win


      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_wrt_arbitrary_win(metal::true_,
					 const I& input, const W& win)
      {
	return erosion_dispatch_for_arbitrary(input, win);
      }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_wrt_arbitrary_win(metal::false_,
					 const I& input, const W& win)
      {
	return erosion_dispatch_for_generic(input, win);
      }

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch_wrt_win(const I& input, const W& win)
      {
	// FIXME:
	// The test "win.size() >= 10" (size is not too small) has been
	// de-activated...
	  enum { test = mlc_equal(mln_trait_image_dimension(I),
				  trait::image::dimension::two_d)::value
		 &&
		        mlc_is_a(mln_site(I), Gpoint)::value
		 &&
		        mln_is_simple_window(W)::value
		 &&
		        mlc_not_equal(mln_trait_image_value_storage(I),
				      trait::image::value_storage::disrupted)::value };
	return erosion_dispatch_wrt_arbitrary_win(metal::bool_<test>(),
						  input, win);
      }

      template <typename I>
      mln_concrete(I)
      erosion_dispatch_wrt_win(const I& input, const win::rectangle2d& win)
      {
	if (win.size() <= 9) // FIXME: Hard-coded!
	  return erosion_dispatch_for_generic(input, win);
	else
	  return impl::erosion_rectangle2d(input, win);
      }

      template <typename I>
      mln_concrete(I)
      erosion_dispatch_wrt_win(const I& input, const win::octagon2d& win)
      {
	if (win.length() < 5)
	  return erosion_dispatch_for_arbitrary(input, win);
	else
	  return impl::erosion_octagon2d(input, win);
      }


      /// Handling win::hline2d.
      /// \{

      template <typename I>
      mln_concrete(I)
      erosion_dispatch_wrt_win(metal::true_,
			       const I& input, const win::hline2d& win)
      {
	return erosion_dispatch_for_directional(input, win, 1);
      }

      template <typename I>
      mln_concrete(I)
      erosion_dispatch_wrt_win(metal::false_,
			       const I& input, const win::hline2d& win)
      {
	return erosion_dispatch_for_generic(input, win);
      }

      template <typename I>
      mln_concrete(I)
      erosion_dispatch_wrt_win(const I& input, const win::hline2d& win)
      {
	if (win.size() == 1)
	  return clone(input);
	else if (win.size() == 3)
	  return erosion_dispatch_for_generic(input, win);
	else
	  {
	    typedef mlc_is_a(mln_pset(I), Box) test_box;
	    typedef mlc_equal(mln_trait_image_quant(I),
			      mln::trait::image::quant::low) test_lowq;
	    typedef mlc_and(test_box, test_lowq) tests;
	    return erosion_dispatch_wrt_win(typename tests::eval (),
					    input, win);
	  }
      }

      /// \}



      /// Handling win::vline2d.
      /// \{

      template <typename I>
      mln_concrete(I)
      erosion_dispatch_wrt_win(metal::true_,
			       const I& input, const win::vline2d& win)
      {
	return erosion_dispatch_for_directional(input, win, 0);
      }

      template <typename I>
      mln_concrete(I)
      erosion_dispatch_wrt_win(metal::false_,
			       const I& input, const win::vline2d& win)
      {
	return erosion_dispatch_for_generic(input, win);
      }

      template <typename I>
      mln_concrete(I)
      erosion_dispatch_wrt_win(const I& input, const win::vline2d& win)
      {
	if (win.size() == 1)
	  return clone(input);
	else if (win.size() == 3)
	  return erosion_dispatch_for_generic(input, win);
	else
	  {
	    typedef mlc_is_a(mln_pset(I), Box) test_box;
	    typedef mlc_equal(mln_trait_image_quant(I),
			      mln::trait::image::quant::low) test_lowq;
	    typedef mlc_and(test_box, test_lowq) tests;
	    return erosion_dispatch_wrt_win(typename tests::eval (),
					    input, win);
	  }
      }

      /// \}


      /// Handling win::diag2d.
      /// \{

      template <typename I>
      mln_concrete(I)
      erosion_dispatch_wrt_win(metal::true_,
			       const I& input, const win::diag2d& win)
      {
	return erosion_dispatch_for_diagonal2d(input, win);
      }

      template <typename I>
      mln_concrete(I)
      erosion_dispatch_wrt_win(metal::false_,
			       const I& input, const win::diag2d& win)
      {
	return erosion_dispatch_for_generic(input, win);
      }

      template <typename I>
      mln_concrete(I)
      erosion_dispatch_wrt_win(const I& input, const win::diag2d& win)
      {
	if (win.size() == 1)
	  return clone(input);
	else if (win.size() == 3)
	  return erosion_dispatch_for_generic(input, win);
	else
	  {
	    typedef mlc_is_not(mln_trait_image_kind(I),
			       mln::trait::image::kind::logic) test_not_logic;
	    typedef mlc_is_a(mln_pset(I), Box) test_box;
	    typedef mlc_equal(mln_trait_image_quant(I),
			      mln::trait::image::quant::low) test_lowq;
	    typedef mlc_and(test_not_logic, test_box) temp;
	    typedef mlc_and(temp, test_lowq) tests;
	    return erosion_dispatch_wrt_win(typename tests::eval (),
					    input, win);
	  }
      }

      /// \}


      /// Handling win::backdiag2d.
      /// \{

      template <typename I>
      mln_concrete(I)
      erosion_dispatch_wrt_win(metal::true_,
			       const I& input, const win::backdiag2d& win)
      {
	return erosion_dispatch_for_backdiagonal2d(input, win);
      }

      template <typename I>
      mln_concrete(I)
      erosion_dispatch_wrt_win(metal::false_,
			       const I& input, const win::backdiag2d& win)
      {
	return erosion_dispatch_for_generic(input, win);
      }

      template <typename I>
      mln_concrete(I)
      erosion_dispatch_wrt_win(const I& input, const win::backdiag2d& win)
      {
	if (win.size() == 1)
	  return clone(input);
	else if (win.size() == 3)
	  return erosion_dispatch_for_generic(input, win);
	else
	  {
	    typedef mlc_is_not(mln_trait_image_kind(I),
			       mln::trait::image::kind::logic) test_not_logic;
	    typedef mlc_is_a(mln_pset(I), Box) test_box;
	    typedef mlc_equal(mln_trait_image_quant(I),
			      mln::trait::image::quant::low) test_lowq;
	    typedef mlc_and(test_not_logic, test_box) temp;
	    typedef mlc_and(temp, test_lowq) tests;
	    return erosion_dispatch_wrt_win(typename tests::eval (),
					    input, win);
	  }
      }

      /// \}


      // The dispatch entry point.

      template <typename I, typename W>
      mln_concrete(I)
      erosion_dispatch(const Image<I>& input, const Window<W>& win)
      {
	return erosion_dispatch_wrt_win(exact(input), exact(win));
      }

    } // end of namespace mln::morpho::internal

  } // end of namespace mln::morpho

} // end of namespace mln


# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_MORPHO_EROSION_SPE_HH
