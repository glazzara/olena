// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ACCU_IMAGE_TAKE_N_TIMES_HH
# define MLN_ACCU_IMAGE_TAKE_N_TIMES_HH

/// \file
///
/// Update an image of accumulators by taking the contents of another
/// image with a multiplicity.
///
/// \todo Add versions with n as unsigned.

# include <mln/core/concept/accumulator.hh>
# include <mln/core/concept/image.hh>
# include <mln/border/resize_equal.hh>


namespace mln
{

  namespace accu
  {

    namespace image
    {

      template <typename I, typename J>
      void
      take_n_times(Image<I>& input, const Image<J>& n_times, const mln_deduce(I, value, argument)& arg);

      template <typename I, typename J, typename K>
      void
      take_n_times(Image<I>& input, const Image<J>& n_times, const Image<K>& arg);



# ifndef MLN_INCLUDE_ONLY


      // Tests.

      namespace internal
      {

	template <typename I, typename J>
	inline
	void
	take_n_times_tests(Image<I>& input_, const Image<J>& n_times_)
	{
	  I& input = exact(input_);
	  const J& n_times = exact(n_times_);
	  
	  mln_precondition(input.is_valid());
	  mln_precondition(n_times.is_valid());

	  mln_precondition(n_times.domain() <= input.domain());

	  (void) input;
	  (void) n_times;
	}

	template <typename I, typename J, typename K>
	inline
	void
	take_n_times_tests(Image<I>& input_, const Image<J>& n_times_, const Image<K>& arg_)
	{
	  I& input = exact(input_);
	  const J& n_times = exact(n_times_);
	  const K& arg = exact(arg_);
	  
	  mln_precondition(input.is_valid());
	  mln_precondition(n_times.is_valid());
	  mln_precondition(arg.is_valid());

	  mln_precondition(arg.domain() <= input.domain());
	  mln_precondition(arg.domain() <= n_times.domain());

	  (void) input;
	  (void) n_times;
	  (void) arg;
	}

      } // end of namespace mln::accu::image::internal


      // Implementations.

      namespace impl
      {

	// Generic versions.

	namespace generic
	{

	  template <typename I, typename J, typename K>
	  inline
	  void
	  take_n_times(Image<I>& input_, const Image<J>& n_times_, const Image<K>& arg_)
	  {
	    mln_trace("accu::impl::image::generic::take_n_times");

	    mlc_is_a(mln_value(I), Accumulator)::check();
	    mlc_converts_to(mln_value(J), unsigned)::check();
	    mlc_converts_to(mln_value(K), mln_deduce(I, value, argument))::check();

	    I& input = exact(input_);
	    const J& n_times = exact(n_times_);
	    const K& arg = exact(arg_);

	    internal::take_n_times_tests(input, n_times, arg);

	    mln_piter(J) p(arg.domain());
	    for_all(p)
	      input(p).take_n_times(n_times(p), arg(p));

	  }

	  template <typename I, typename J>
	  inline
	  void
	  take_n_times(Image<I>& input_, const Image<J>& n_times_, const mln_deduce(I, value, argument)& arg)
	  {
	    mln_trace("accu::impl::image::generic::take_n_times");

	    mlc_is_a(mln_value(I), Accumulator)::check();
	    mlc_converts_to(mln_value(J), unsigned)::check();

	    I& input = exact(input_);
	    const J& n_times = exact(n_times_);

	    internal::take_n_times_tests(input, n_times);

	    mln_piter(J) p(n_times.domain());
	    for_all(p)
	      input(p).take_n_times(n_times(p), arg);

	  }

	} // end of namespace mln::accu::image::impl::generic


	// Fastest versions.

	template <typename I, typename J>
	inline
	void
	take_n_times_fastest(Image<I>& input_, const Image<J>& n_times_, const mln_deduce(I, value, argument)& arg)
	{
	  mln_trace("accu::impl::image::take_n_times_fastest");

	  mlc_is_a(mln_value(I), Accumulator)::check();
	  mlc_converts_to(mln_value(J), unsigned)::check();

	  I& input = exact(input_);
	  const J& n_times = exact(n_times_);

	  internal::take_n_times_tests(input, n_times);
	  // Extra (stronger) test:
	  mln_precondition(n_times.domain() == input.domain());

	  border::resize_equal(input, n_times);
	  
	  mln_pixter(I)       p_in(input);
	  mln_pixter(const J) p_ntm(n_times);
	  for_all_2(p_in, p_ntm)
	    p_in.val().take_n_times( p_ntm.val(), arg );

	}

	template <typename I, typename J, typename K>
	inline
	void
	take_n_times_fastest(Image<I>& input_, const Image<J>& n_times_, const Image<K>& arg_)
	{
	  mln_trace("accu::impl::image::take_n_times_fastest");

	  mlc_is_a(mln_value(I), Accumulator)::check();
	  mlc_converts_to(mln_value(J), unsigned)::check();
	  mlc_converts_to(mln_value(K), mln_deduce(I, value, argument))::check();

	  I& input = exact(input_);
	  const J& n_times = exact(n_times_);
	  const K& arg = exact(arg_);

	  internal::take_n_times_tests(input, n_times, arg);

	  // Extra (stronger) tests.
	  mln_precondition(arg.domain() == input.domain());
	  mln_precondition(arg.domain() == n_times.domain());

	  border::resize(n_times, input.border());
	  border::resize(arg,     input.border());
	  
	  mln_pixter(I)       p_in(input);
	  mln_pixter(const J) p_ntm(n_times);
	  mln_pixter(const K) p_arg(arg);
	  for_all_3(p_in, p_ntm, p_arg)
	    p_in.val().take_n_times( p_ntm.val(), p_arg.val() );

	}

      } // end of namespace mln::accu::image::impl



      // Dispatch.

      namespace internal
      {

	// 'arg' as value.

	template <typename I, typename J>
	inline
	void
	take_n_times_dispatch(trait::image::speed::any,
			      trait::image::speed::any,
			      Image<I>& input, const Image<J>& n_times, const mln_deduce(I, value, argument)& arg)
	{
	  impl::generic::take_n_times(input, n_times, arg);
	}

	template <typename I, typename J>
	inline
	void
	take_n_times_dispatch(trait::image::speed::fastest,
			      trait::image::speed::fastest,
			      Image<I>& input, const Image<J>& n_times, const mln_deduce(I, value, argument)& arg)
	{
	  if (exact(n_times).domain() == exact(input).domain())
	    impl::take_n_times_fastest(input, n_times, arg);
	  else
	    impl::generic::take_n_times(input, n_times, arg);
	}

	template <typename I, typename J>
	inline
	void
	take_n_times_dispatch(Image<I>& input, const Image<J>& n_times, const mln_deduce(I, value, argument)& arg)
	{
	  take_n_times_dispatch(mln_trait_image_speed(I)(),
				mln_trait_image_speed(J)(),
				input, n_times, arg);
	}

	// Triplet of images.

	template <typename I, typename J, typename K>
	inline
	void
	take_n_times_dispatch(trait::image::speed::any,
			      trait::image::speed::any,
			      trait::image::speed::any,
			      Image<I>& input, const Image<J>& n_times, const Image<K>& arg)
	{
	  impl::generic::take_n_times(input, n_times, arg);
	}

	template <typename I, typename J, typename K>
	inline
	void
	take_n_times_dispatch(trait::image::speed::fastest,
			      trait::image::speed::fastest,
			      trait::image::speed::fastest,
			      Image<I>& input, const Image<J>& n_times, const Image<K>& arg)
	{
	  if (exact(n_times).domain() == exact(input).domain() &&
	      exact(arg)    .domain() == exact(input).domain())
	    impl::take_n_times_fastest(input, n_times, arg);
	  else
	    impl::generic::take_n_times(input, n_times, arg);
	}

	template <typename I, typename J, typename K>
	inline
	void
	take_n_times_dispatch(Image<I>& input, const Image<J>& n_times, const Image<K>& arg)
	{
	  take_n_times_dispatch(mln_trait_image_speed(I)(),
				mln_trait_image_speed(J)(),
				mln_trait_image_speed(K)(),
				input, n_times, arg);
	}

      } // end of namespace mln::accu::image::internal


      // Facades.

      template <typename I, typename J>
      void
      take_n_times(Image<I>& input, const Image<J>& n_times, const mln_deduce(I, value, argument)& arg)
      {
	mln_trace("accu::image::take_n_times");

	mlc_is_a(mln_value(I), Accumulator)::check();
	mlc_converts_to(mln_value(J), unsigned)::check();

	internal::take_n_times_tests(input, n_times);
	internal::take_n_times_dispatch(input, n_times, arg);

      }

      template <typename I, typename J, typename K>
      inline
      void
      take_n_times(Image<I>& input, const Image<J>& n_times, const Image<K>& arg)
      {
	mln_trace("accu::image::take_n_times");

	mlc_is_a(mln_value(I), Accumulator)::check();
	mlc_converts_to(mln_value(J), unsigned)::check();
	mlc_converts_to(mln_value(K), mln_deduce(I, value, argument))::check();

	internal::take_n_times_tests(input, n_times, arg);
	internal::take_n_times_dispatch(input, n_times, arg);

      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::image

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_IMAGE_TAKE_N_TIMES_HH
