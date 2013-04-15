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

#ifndef MLN_ACCU_TRANSFORM_HH
# define MLN_ACCU_TRANSFORM_HH

/// \file
///
/// Transform an image by applying locally an accumulator on its
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
      transform(const Image<I>& input,
		const Accumulator<A>& a,
		const Window<W>& win);

    template <typename I, typename A, typename W>
    mln_ch_value(I, mln_meta_accu_result(A, mln_value(I)))
      transform(const Image<I>& input,
		const Meta_Accumulator<A>& a,
		const Window<W>& win);



# ifndef MLN_INCLUDE_ONLY


    namespace impl
    {

      // Generic version.

      namespace generic
      {

	template <typename I, typename A, typename W>
	mln_ch_value(I, mln_result(A))
	  transform(const Image<I>& input_,
		    const Accumulator<A>& a_,
		    const Window<W>& win_)
	{
	  mln_trace("accu::impl::generic::transform");

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
	      a.take(input(q));
	    output(p) = a.to_result();
	  }

	  return output;
	}

      } // end of namespace mln::accu::impl::generic


      // Fastest version.

      template <typename I, typename A, typename W>
      mln_ch_value(I, mln_result(A))
      transform_fastest(const Image<I>& input_, const Accumulator<A>& a_, const Window<W>& win_)
      {
	mln_trace("accu::impl::transform_fastest");

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
	      a.take(q.val());
	    o.val() = a.to_result();
	  }

	return output;
      }


    } // end of namespace mln::accu::impl


    // Dispatch.

    namespace internal
    {

      template <typename I, typename A, typename W>
      mln_ch_value(I, mln_result(A))
      transform_dispatch(metal::false_,
			 const Image<I>& input, const Accumulator<A>& a, const Window<W>& win)
      {
	return impl::generic::transform(input, a, win);
      }

      template <typename I, typename A, typename W>
      mln_ch_value(I, mln_result(A))
      transform_dispatch(metal::true_,
			 const Image<I>& input, const Accumulator<A>& a, const Window<W>& win)
      {
	return impl::transform_fastest(input, a, win);
      }

      template <typename I, typename A, typename W>
      mln_ch_value(I, mln_result(A))
      transform_dispatch(const Image<I>& input, const Accumulator<A>& a, const Window<W>& win)
      {
	return transform_dispatch(mln_is_fastest_IW(I, W)(),
				  input, a, win);
      }

    } // end of namespace mln::accu::internal


    // Facades.

    template <typename I, typename A, typename W>
    inline
    mln_ch_value(I, mln_result(A))
    transform(const Image<I>& input, const Accumulator<A>& a, const Window<W>& win)
    {
      mln_trace("accu::transform");

      mln_precondition(exact(input).is_valid());
      mln_precondition(exact(win).is_valid());

      mln_ch_value(I, mln_result(A)) output;
      output = internal::transform_dispatch(input, a, win);

      return output;
    }

    template <typename I, typename A, typename W>
    mln_ch_value(I, mln_meta_accu_result(A, mln_value(I)))
    transform(const Image<I>& input, const Meta_Accumulator<A>& a, const Window<W>& win)
    {
      mln_trace("accu::transform");

      mln_precondition(exact(input).is_valid());
      mln_precondition(exact(win).is_valid());

      typedef mln_accu_with(A, mln_value(I)) A_;
      A_ a_ = accu::unmeta(exact(a), mln_value(I)());

      mln_ch_value(I, mln_result(A_)) output;
      output = internal::transform_dispatch(input, a_, win);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_TRANSFORM_HH
