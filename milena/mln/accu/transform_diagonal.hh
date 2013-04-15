// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ACCU_TRANSFORM_DIAGONAL_HH
# define MLN_ACCU_TRANSFORM_DIAGONAL_HH

/// \file
///
/// \brief Run an accumulator over a diagonal.
///
/// \todo Split dispatch and impl.
///
/// \todo Pass the accumulator to the function-object.


#include <mln/core/concept/image.hh>
#include <mln/core/concept/meta_accumulator.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/win/diff.hh>
#include <mln/win/shift.hh>
#include <mln/geom/delta.hh>
#include <mln/extension/adjust.hh>

#include <mln/win/diag2d.hh>
#include <mln/canvas/browsing/diagonal2d.hh>

#include <mln/win/backdiag2d.hh>
#include <mln/canvas/browsing/backdiagonal2d.hh>



namespace mln
{

  namespace accu
  {


    template <typename A, typename I, typename W>
    mln_ch_value(I, mln_result(A))
    transform_diagonal(const Accumulator<A>& a,
		       const Image<I>& input, const Window<W>& win);


    template <typename A, typename I, typename W>
    mln_ch_value(I, mln_meta_accu_result(A, mln_value(I)))
    transform_diagonal(const Meta_Accumulator<A>& a,
		       const Image<I>& input, const Window<W>& win);



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {


      // Tests.


      template <typename I, typename W>
      void transform_diagonal_tests(const Image<I>& input_, const Window<W>& win_)
      {
	const I& input = exact(input_);
	const W& win   = exact(win_);

	mln_precondition(input.is_valid());
	mln_precondition(win.is_valid());
	mln_precondition(! win.is_empty());

	(void) input;
	(void) win;
      }



      // Functors.


      template <typename I_, typename W, typename A>
      struct diagonal_functor
      {
	typedef I_ I;
	typedef mln_deduce(I, psite, delta) dpsite;

	const I& input;
	const W& win;
	mln_ch_value(I, mln_result(A)) output;
	A accu;

	mln_psite(I) p;
	typedef mln_site(I) S; // Help g++-2.95.
	enum { dim = S::dim };

	window2d win_left, win_right;

	mln_qiter(window2d) q_l, q_r;

	diagonal_functor(const I& input, const W& win, const A& a)
	  : input(input),
	    win(win),
	    accu(a),
	    win_left(win::shift(win, dpsite(1, -1)) - win),
	    win_right(win - win::shift(win, dpsite(1, -1))),
	    q_l(win_left, p),
	    q_r(win_right, p)
	{
	}

	void init()
	{
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



      template <typename I_, typename W, typename A>
      struct backdiagonal_functor
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

	window2d win_left, win_right;

	mln_qiter(window2d) q_l, q_r;

	backdiagonal_functor(const I& input, const W& win, const A& a)
	  : input(input),
	    win(win),
	    accu(a),
	    win_left(win::shift(win, dpsite(-1, -1)) - win),
	    win_right(win - win::shift(win, dpsite(-1, -1))),
	    q_l(win_left, p),
	    q_r(win_right, p)
	{
	}

	void init()
	{
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



      // Functors (fastest versions).


      template <typename I_, typename W, typename A>
      struct diagonal_fastest_functor
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

	window2d win_left, win_right;

	mln_qixter(const I, window2d) q_l, q_r;

	diagonal_fastest_functor(const I& input, const W& win, const A& a)
	  : input(input),
	    win(win),
	    accu(a),
	    win_left(win::shift(win, dpsite(1, -1)) - win),
	    win_right(win - win::shift(win, dpsite(1, -1))),
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


      template <typename I_, typename W, typename A>
      struct backdiagonal_fastest_functor
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

	window2d win_left, win_right;

	mln_qixter(const I, window2d) q_l, q_r;

	backdiagonal_fastest_functor(const I& input, const W& win, const A& a)
	  : input(input),
	    win(win),
	    accu(a),
	    win_left(win::shift(win, dpsite(-1, -1)) - win),
	    win_right(win - win::shift(win, dpsite(-1, -1))),
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




      // Both dispatch and implementation (hum...)

      template <typename A, typename I>
      inline
      mln_ch_value(I, mln_result(A))
      transform_diagonal_dispatch(metal::false_,
				  const Accumulator<A>& a,
				  const Image<I>& input, const win::diag2d& win)
      {
	typedef diagonal_functor<I, win::diag2d, A> F;
	F f(exact(input), win, exact(a));
	canvas::browsing::diagonal2d(f);
	return f.output;
      }

      template <typename B, typename A, typename I>
      inline
      mln_ch_value(I, mln_result(A))
      transform_diagonal_dispatch(metal::false_,
				  const Accumulator<A>& a,
				  const Image<I>& input, const win::backdiag2d& win)
      {
	typedef backdiagonal_functor<I, win::backdiag2d, A> F;
	F f(exact(input), win, exact(a));
	canvas::browsing::backdiagonal2d(f);
	return f.output;
      }

      template <typename A, typename I>
      inline
      mln_ch_value(I, mln_result(A))
      transform_diagonal_dispatch(metal::true_,
				  const Accumulator<A>& a,
				  const Image<I>& input, const win::diag2d& win)
      {
	typedef diagonal_fastest_functor<I, win::diag2d, A> F;
	F f(exact(input), win, exact(a));
	canvas::browsing::diagonal2d(f);
	return f.output;
      }

      template <typename A, typename I>
      inline
      mln_ch_value(I, mln_result(A))
      transform_diagonal_dispatch(metal::true_,
				  const Accumulator<A>& a,
				  const Image<I>& input, const win::backdiag2d& win)
      {
	typedef backdiagonal_fastest_functor<I, win::backdiag2d, A> F;
	F f(exact(input), win, exact(a));
	canvas::browsing::backdiagonal2d(f);
	return f.output;
      }

      template <typename A, typename I, typename W>
      inline
      mln_ch_value(I, mln_result(A))
      transform_diagonal_dispatch(const Accumulator<A>& a,
				  const Image<I>& input, const Window<W>& win)
      {
	return transform_diagonal_dispatch(mln_is_fastest_IW(I, W)(),
					   a, input, exact(win));
      }

    } // end of namespace mln::accu::internal




    template <typename A, typename I, typename W>
    inline
    mln_ch_value(I, mln_result(A))
    transform_diagonal(const Accumulator<A>& a,
		       const Image<I>& input, const Window<W>& win)
    {
      mln_trace("accu::transform_diagonal");

      internal::transform_diagonal_tests(input, win);

      extension::adjust(input, geom::delta(win) + 1);
      mln_ch_value(I, mln_result(A)) output;
      output = internal::transform_diagonal_dispatch(a, input, win);

      return output;
    }


    template <typename A, typename I, typename W>
    inline
    mln_ch_value(I, mln_meta_accu_result(A, mln_value(I)))
    transform_diagonal(const Meta_Accumulator<A>& a,
		       const Image<I>& input, const Window<W>& win)
    {
      mln_trace("accu::transform_diagonal");

      internal::transform_diagonal_tests(input, win);

      typedef mln_accu_with(A, mln_value(I)) A_;
      A_ a_ = accu::unmeta(exact(a), mln_value(I)());

      extension::adjust(input, geom::delta(win) + 1);
      mln_ch_value(I, mln_result(A_)) output;
      output = internal::transform_diagonal_dispatch(a_, input, win);

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_TRANSFORM_DIAGONAL_HH
