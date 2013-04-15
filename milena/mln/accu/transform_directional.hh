// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_ACCU_TRANSFORM_DIRECTIONAL_HH
# define MLN_ACCU_TRANSFORM_DIRECTIONAL_HH

/// \file
///
/// \brief Run an accumulator over a particular direction.
///
/// \todo Split dispatch and impl.
///
/// \todo Pass the accumulator to the function-object.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/core/alias/window2d.hh>
# include <mln/win/diff.hh>
# include <mln/win/shift.hh>
# include <mln/geom/delta.hh>
# include <mln/literal/zero.hh>
# include <mln/extension/adjust.hh>
# include <mln/canvas/browsing/directional.hh>



namespace mln
{

  namespace accu
  {


    template <typename A, typename I, typename W>
    mln_ch_value(I, mln_result(A))
    transform_directional(const Accumulator<A>& a,
			  const Image<I>& input, const Window<W>& win,
			  unsigned dir);


    template <typename A, typename I, typename W>
    mln_ch_value(I, mln_meta_accu_result(A, mln_value(I)))
    transform_directional(const Meta_Accumulator<A>& a,
			  const Image<I>& input, const Window<W>& win,
			  unsigned dir);



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {


      // Tests.


      template <typename I, typename W>
      void transform_directional_tests(const Image<I>& input_, const Window<W>& win_)
      {
	const I& input = exact(input_);
	const W& win   = exact(win_);

	mln_precondition(input.is_valid());
	mln_precondition(win.is_valid());
	mln_precondition(! win.is_empty());

	(void) input;
	(void) win;
      }



      // Utility.


      template <typename Dp>
      Dp dp_directional(int dir)
      {
	Dp dp = literal::zero;
	dp[dir] = 1;
	return dp;
      }



      // Functor.


      template <typename I_, typename W, typename A>
      struct directional_functor
      {
	typedef I_ I;
	typedef mln_deduce(I, psite, delta) dpsite;

	const I& input;
	const W& win;
	mln_ch_value(I, mln_result(A)) output;
	A accu;

	typedef mln_site(I) S; // Help g++-2.95.
	enum { dim = S::dim };

	mln_psite(I) p;
	unsigned dir;

	window2d
	win_left,
	  win_right;

	mln_qiter(window2d)
	q_l,
	  q_r;

	directional_functor(const I& input, const W& win, const A& a, int dir)
	  : input(input),
	    win(win),
	    accu(a),
	    dir(dir),
	    win_left(win::shift(win, -dp_directional<dpsite>(dir)) - win),
	    win_right(win - win::shift(win, -dp_directional<dpsite>(dir))),
	    q_l(win_left, p),
	    q_r(win_right, p)
	{

	}

	void init()
	{
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



      // Functor (fastest version).


      template <typename I_, typename W, typename A>
      struct directional_fastest_functor
      {
	typedef I_ I;
	typedef mln_deduce(I, psite, delta) dpsite;

	const I& input;
	const W& win;
	mln_ch_value(I, mln_result(A)) output;
	A accu;

	mln_psite(I) p;
	typedef mln_site(I) S;
	enum { dim = S::dim };
	unsigned dir;

	window2d win_left, win_right;

	mln_qixter(const I, window2d) q_l, q_r;

	directional_fastest_functor(const I& input, const W& win, const A& a, unsigned dir)
	  : input(input),
	    win(win),
	    accu(a),
	    dir(dir),
	    win_left(win::shift(win, -dp_directional<dpsite>(dir)) - win),
	    win_right(win - win::shift(win, -dp_directional<dpsite>(dir))),
	    q_l(input, win_left, p),
	    q_r(input, win_right, p)
	{
	}

	void init()
	{
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




      // Both dispatch and implementation (hum...)

      template <typename A, typename I, typename W>
      inline
      mln_ch_value(I, mln_result(A))
      transform_directional_dispatch(metal::false_,
				     const Accumulator<A>& a,
				     const Image<I>& input, const Window<W>& win,
				     unsigned dir)
      {
	typedef directional_functor<I, W, A> F;
	F f(exact(input), exact(win), exact(a), dir);
	canvas::browsing::directional(f);
	return f.output;
      }

      template <typename A, typename I, typename W>
      inline
      mln_ch_value(I, mln_result(A))
      transform_directional_dispatch(metal::true_,
				     const Accumulator<A>& a,
				     const Image<I>& input, const Window<W>& win,
				     unsigned dir)
      {
	typedef directional_fastest_functor<I, W, A> F;
	F f(exact(input), exact(win), exact(a), dir);
	canvas::browsing::directional(f);
	return f.output;
      }

      template <typename A, typename I, typename W>
      inline
      mln_ch_value(I, mln_result(A))
      transform_directional_dispatch(const Accumulator<A>& a,
				     const Image<I>& input, const Window<W>& win,
				     unsigned dir)
      {
	return transform_directional_dispatch(mln_is_fastest_IW(I, W)(),
					      a, input, win, dir);
      }

    } // end of namespace mln::accu::internal




    template <typename A, typename I, typename W>
    inline
    mln_ch_value(I, mln_result(A))
    transform_directional(const Accumulator<A>& a,
			  const Image<I>& input, const Window<W>& win,
			  unsigned dir)
    {
      mln_trace("accu::transform_directional");

      internal::transform_directional_tests(input, win);

      extension::adjust(input, geom::delta(win) + 1);
      mln_ch_value(I, mln_result(A)) output;
      output = internal::transform_directional_dispatch(a, input, win, dir);

      return output;
    }


    template <typename A, typename I, typename W>
    inline
    mln_ch_value(I, mln_meta_accu_result(A, mln_value(I)))
    transform_directional(const Meta_Accumulator<A>& a,
			  const Image<I>& input, const Window<W>& win,
			  unsigned dir)
    {
      mln_trace("accu::transform_directional");

      internal::transform_directional_tests(input, win);

      typedef mln_accu_with(A, mln_value(I)) A_;
      A_ a_ = accu::unmeta(exact(a), mln_value(I)());

      extension::adjust(input, geom::delta(win) + 1);
      mln_ch_value(I, mln_result(A_)) output;
      output = internal::transform_directional_dispatch(a_, input, win, dir);

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_TRANSFORM_DIRECTIONAL_HH
