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

#ifndef MLN_ACCU_IMAGE_UNTAKE_HH
# define MLN_ACCU_IMAGE_UNTAKE_HH

/// \file
///
/// Update an image of accumulators by untaking a value or the
/// contents of another image.

# include <mln/core/concept/accumulator.hh>
# include <mln/core/concept/image.hh>
# include <mln/border/resize_equal.hh>


namespace mln
{

  namespace accu
  {

    namespace image
    {

      template <typename I>
      void
      untake(Image<I>& input, const mln_deduce(I, value, argument)& arg);

      template <typename I, typename J>
      void
      untake(Image<I>& input, const Image<J>& arg);


# ifndef MLN_INCLUDE_ONLY


      // Tests.

      namespace internal
      {

	template <typename I, typename J>
	inline
	void
	untake_tests(Image<I>& input_, const Image<J>& arg_)
	{
	  I& input = exact(input_);
	  const J& arg = exact(arg_);

	  mln_precondition(input.is_valid());
	  mln_precondition(arg.is_valid());

	  mln_precondition(arg.domain() <= input.domain());

	  (void) input;
	  (void) arg;
	}

      } // end of namespace mln::accu::image::internal



      // Implementations.

      namespace impl
      {

	// Generic versions.

	namespace generic
	{

	  template <typename I>
	  inline
	  void
	  untake(Image<I>& input_, const mln_deduce(I, value, argument)& arg)
	  {
	    mln_trace("accu::impl::image::generic::untake");

	    mlc_is_a(mln_value(I), Accumulator)::check();

	    I& input = exact(input_);
	    mln_precondition(input.is_valid());

	    mln_piter(I) p(input.domain());
	    for_all(p)
	      input(p).untake(arg);

	  }

	  template <typename I, typename J>
	  inline
	  void
	  untake(Image<I>& input_, const Image<J>& arg_)
	  {
	    mln_trace("accu::impl::image::generic::untake");

	    mlc_is_a(mln_value(I), Accumulator)::check();
	    mlc_converts_to(mln_value(J), mln_deduce(I, value, argument))::check();

	    I& input = exact(input_);
	    const J& arg = exact(arg_);

	    internal::untake_tests(input, arg);

	    mln_piter(J) p(arg.domain());
	    for_all(p)
	      input(p).untake(arg(p));

	  }

	} // end of namespace mln::accu::image::impl::generic


	// Fastest versions.

	template <typename I>
	inline
	void
	untake_fastest(Image<I>& input_, const mln_deduce(I, value, argument)& arg)
	{
	  mln_trace("accu::impl::image::untake_fastest");

	  mlc_is_a(mln_value(I), Accumulator)::check();
	  
	  I& input = exact(input_);
	  mln_precondition(input.is_valid());
	  
	  mln_pixter(I) px(input);
	  for_all(px)
	    px.val().untake(arg);

	}

	template <typename I, typename J>
	inline
	void
	untake_fastest(Image<I>& input_, const Image<J>& arg_)
	{
	  mln_trace("accu::impl::image::untake_fastest");

	  mlc_is_a(mln_value(I), Accumulator)::check();
	  mlc_converts_to(mln_value(J), mln_deduce(I, value, argument))::check();
	  
	  I& input = exact(input_);
	  const J& arg = exact(arg_);

	  internal::untake_tests(input, arg);
	  // Extra (stronger) test.
	  mln_precondition(arg.domain() == input.domain());

	  border::resize_equal(input, arg);
	  
	  mln_pixter(I)       p_in(input);
	  mln_pixter(const J) p_arg(arg);
	  for_all_2(p_in, p_arg)
	    p_in.val().untake( p_arg.val() );

	}

      } // end of namespace mln::accu::image::impl



      // Dispatch.

      namespace internal
      {

	// 'arg' as value.

	template <typename I>
	inline
	void
	untake_dispatch(trait::image::speed::any,
			Image<I>& input, const mln_deduce(I, value, argument)& arg)
	{
	  impl::generic::untake(input, arg);
	}

	template <typename I>
	inline
	void
	untake_dispatch(trait::image::speed::fastest,
			Image<I>& input, const mln_deduce(I, value, argument)& arg)
	{
	  impl::untake_fastest(input, arg);
	}

	template <typename I>
	inline
	void
	untake_dispatch(Image<I>& input, const mln_deduce(I, value, argument)& arg)
	{
	  untake_dispatch(mln_trait_image_speed(I)(),
			  input, arg);
	}

	// 'arg' as image.

	template <typename I, typename J>
	inline
	void
	untake_dispatch(trait::image::speed::any,
			trait::image::speed::any,
			Image<I>& input, const Image<J>& arg)
	{
	  impl::generic::untake(input, arg);
	}

	template <typename I, typename J>
	inline
	void
	untake_dispatch(trait::image::speed::fastest,
			trait::image::speed::fastest,
			Image<I>& input, const Image<J>& arg)
	{
	  if (exact(arg).domain() == exact(input).domain())
	    impl::untake_fastest(input, arg);
	  else
	    impl::generic::untake(input, arg);
	}

	template <typename I, typename J>
	inline
	void
	untake_dispatch(Image<I>& input, const Image<J>& arg)
	{
	  untake_dispatch(mln_trait_image_speed(I)(),
			  mln_trait_image_speed(J)(),
			  input, arg);
	}

      } // end of namespace mln::accu::image::internal


      // Facades.

      template <typename I>
      inline
      void
      untake(Image<I>& input, const mln_deduce(I, value, argument)& arg)
      {
	mln_trace("accu::image::untake");

	typedef mln_value(I) A;
	mlc_is_a(A, Accumulator)::check();
	mlc_equal(mln_trait_accumulator_has_untake(A),
		  trait::accumulator::has_untake::yes)::check();

	mln_precondition(exact(input).is_valid());
	internal::untake_dispatch(input, arg);

      }

      template <typename I, typename J>
      inline
      void
      untake(Image<I>& input, const Image<J>& arg)
      {
	mln_trace("accu::image::untake");

	typedef mln_value(I) A;
	mlc_is_a(A, Accumulator)::check();
	mlc_equal(mln_trait_accumulator_has_untake(A),
		  trait::accumulator::has_untake::yes)::check();
	mlc_converts_to(mln_value(J), mln_deduce(I, value, argument))::check();

	internal::untake_tests(input, arg);
	internal::untake_dispatch(input, arg);

      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::image

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_IMAGE_UNTAKE_HH
