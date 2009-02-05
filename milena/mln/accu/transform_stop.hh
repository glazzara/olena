// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_ACCU_TRANSFORM_STOP_HH
# define MLN_ACCU_TRANSFORM_STOP_HH

/// \file mln/accu/transform_stop.hh
///
/// Transform_Stop an image by applying locally an accumulator on its
/// values.

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/extension/adjust.hh>


namespace mln
{

  namespace accu
  {

    template <typename I, typename A, typename W>
    mln_ch_value(I, mln_result(A))
    transform_stop(const Image<I>& input, const Accumulator<A>& a, const Window<W>& win);

    template <typename I, typename A, typename W>
    mln_ch_value(I, mln_accu_with(A, mln_value(I))::result)
    transform_stop(const Image<I>& input, const Meta_Accumulator<A>& a, const Window<W>& win);



# ifndef MLN_INCLUDE_ONLY


    namespace impl
    {

      // Generic version.

      namespace generic
      {

	template <typename I, typename A, typename W>
	mln_ch_value(I, mln_result(A))
	transform_stop(const Image<I>& input_,
		       const Accumulator<A>& a_,
		       const Window<W>& win_)
	{
	  trace::entering("accu::impl::generic::transform_stop");

	  const I& input = exact(input_);
	  const W& win   = exact(win_);
	  A a = exact(a_);

	  mln_precondition(input.is_valid());
	  mln_precondition(win.is_valid());

	  extension::adjust(input, win);

	  mln_ch_value(I, mln_result(A)) output;
	  initialize(output, input);

	  mln_piter(I) p(input.domain());
	  mln_qiter(W) q(win, p);
	  for_all(p)
	  {
	    a.init();
	    for_all(q) if (input.has(q))
	    {
	      a.take(input(q));
	      if (a.can_stop())
		break;
	    }
	    output(p) = a.to_result();
	  }

	  trace::exiting("accu::impl::generic::transform_stop");
	  return output;
	}

      } // end of namespace mln::accu::impl::generic


      // Fastest version.

      template <typename I, typename A, typename W>
      mln_ch_value(I, mln_result(A))
      transform_stop_fastest(const Image<I>& input_, const Accumulator<A>& a_, const Window<W>& win_)
      {
	trace::entering("accu::impl::transform_stop_fastest");

	const I& input = exact(input_);
	const W& win   = exact(win_);
	A a = exact(a_);

	mln_precondition(input.is_valid());
	mln_precondition(win.is_valid());

	extension::adjust(input, win);

	typedef mln_ch_value(I, mln_result(A)) O;
	O output;
	initialize(output, input);
	mln_pixter(O) o(output);

	mln_pixter(const I)    p(input);
	mln_qixter(const I, W) q(p, win);
	for_all_2(p, o)
	  {
	    a.init();
	    for_all(q)
	    {
	      a.take(q.val());
	      if (a.can_stop())
		break;
	    }
	    o.val() = a.to_result();
	  }

	trace::exiting("accu::impl::transform_stop_fastest");
	return output;
      }


    } // end of namespace mln::accu::impl


    // Dispatch.

    namespace internal
    {

      template <typename I, typename A, typename W>
      mln_ch_value(I, mln_result(A))
      transform_stop_dispatch(metal::false_,
			      const Image<I>& input, const Accumulator<A>& a, const Window<W>& win)
      {
	return impl::generic::transform_stop(input, a, win);
      }

      template <typename I, typename A, typename W>
      mln_ch_value(I, mln_result(A))
      transform_stop_dispatch(metal::true_,
			      const Image<I>& input, const Accumulator<A>& a, const Window<W>& win)
      {
	return impl::transform_stop_fastest(input, a, win);
      }

      template <typename I, typename A, typename W>
      mln_ch_value(I, mln_result(A))
      transform_stop_dispatch(const Image<I>& input, const Accumulator<A>& a, const Window<W>& win)
      {
	return transform_stop_dispatch(mln_is_fastest_IW(I, W)(),
				       input, a, win);
      }

    } // end of namespace mln::accu::internal


    // Facades.

    template <typename I, typename A, typename W>
    inline
    mln_ch_value(I, mln_result(A))
    transform_stop(const Image<I>& input, const Accumulator<A>& a, const Window<W>& win)
    {
      trace::entering("accu::transform_stop");

      mln_precondition(exact(input).is_valid());
      mln_precondition(exact(win).is_valid());

      mln_ch_value(I, mln_result(A)) output;
      output = internal::transform_stop_dispatch(input, a, win);

      trace::exiting("accu::transform_stop");
      return output;
    }

    template <typename I, typename A, typename W>
    mln_ch_value(I, mln_accu_with(A, mln_value(I))::result)
    transform_stop(const Image<I>& input, const Meta_Accumulator<A>& a, const Window<W>& win)
    {
      trace::entering("accu::transform_stop");

      mln_precondition(exact(input).is_valid());
      mln_precondition(exact(win).is_valid());

      typedef mln_accu_with(A, mln_value(I)) A_;
      A_ a_ = accu::unmeta(exact(a), mln_value(I)());

      mln_ch_value(I, mln_result(A_)) output;
      output = internal::transform_stop_dispatch(input, a_, win);

      trace::exiting("accu::transform_stop");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_TRANSFORM_STOP_HH
