template <typename I, typename W, typename A>
mln_concrete(I)
snake_2d()

      template <typename I, typename W, typename A>
      struct snake_2d_functor
      {
	typedef snake_2d_functor<I,W, A> self;
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
	  extension::adjust_fill(input, win, mln_max(mln_value(I)));
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

      template <typename I, typename W, typename A>
      inline
      mln_concrete(I)
      snake_2d(const Accumulator<A>&, const Image<I>& input, const Window<W>& win)
      {
	trace::entering("morpho::impl:erosion_arbitrary_2d");

	typedef erosion_arbitrary_2d_functor<I, W, A> F;
	F f(exact(input), exact(win));
	canvas::browsing::snake_generic(f);

	trace::exiting("morpho::impl:erosion_arbitrary_2d");

	return f.output;
      }
