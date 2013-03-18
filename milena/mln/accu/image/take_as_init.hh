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

#ifndef MLN_ACCU_IMAGE_TAKE_AS_INIT_HH
# define MLN_ACCU_IMAGE_TAKE_AS_INIT_HH

/// \file
///
/// Initialize an image of accumulators by taking a first value.

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
      take_as_init(Image<I>& input, const mln_deduce(I, value, argument)& v);

      template <typename I, typename J>
      void
      take_as_init(Image<I>& input, const Image<J>& values);



# ifndef MLN_INCLUDE_ONLY

      // Tests.

      namespace internal
      {

	template <typename I, typename J>
	inline
	void
	take_as_init_tests(Image<I>& input_, const Image<J>& values_)
	{
	  I& input = exact(input_);
	  const J& values = exact(values_);

	  mln_precondition(input.is_valid());
	  mln_precondition(values.is_valid());

	  mln_precondition(values.domain() <= input.domain());

	  (void) input;
	  (void) values;
	}

      } // end of namespace mln::accu::image::internal


      namespace impl
      {

	// Generic versions.

	namespace generic
	{

	  template <typename I>
	  void
	  take_as_init(Image<I>& input_, const mln_deduce(I, value, argument)& v)
	  {
	    mln_trace("accu::impl::image::generic::take_as_init");

	    mlc_is_a(mln_value(I), Accumulator)::check();

	    I& input = exact(input_);
	    mln_precondition(input.is_valid());

	    mln_piter(I) p(input.domain());
	    for_all(p)
	      input(p).take_as_init(v);

	  }

	  template <typename I, typename J>
	  void
	  take_as_init(Image<I>& input_, const Image<J>& values_)
	  {
	    mln_trace("accu::impl::image::generic::take_as_init");

	    typedef mln_value(I) A;
	    mlc_is_a(A, Accumulator)::check();
	    mlc_converts_to(mln_value(J), mln_argument(A))::check();

	    I& input = exact(input_);
	    const J& values = exact(values_);

	    internal::take_as_init_tests(input, values);

	    mln_piter(J) p(values.domain());
	    for_all(p)
	      input(p).take_as_init(values(p));

	  }

	} // end of namespace mln::accu::image::impl::generic


	// Fastest versions.

	template <typename I>
	void
	take_as_init_fastest(Image<I>& input_, const mln_deduce(I, value, argument)& v)
	{
	  mln_trace("accu::impl::image::take_as_init_fastest");
	  
	  mlc_is_a(mln_value(I), Accumulator)::check();

	  I& input = exact(input_);
	  mln_precondition(input.is_valid());
	  
	  mln_pixter(I) px(input);
	  for_all(px)
	    px.val().take_as_init(v);

	}

	template <typename I, typename J>
	void
	take_as_init_fastest(Image<I>& input_, const Image<J>& values_)
	{
	  mln_trace("accu::impl::image::take_as_init_fastest");

	  typedef mln_value(I) A;
	  mlc_is_a(A, Accumulator)::check();
	  mlc_converts_to(mln_value(J), mln_argument(A))::check();
	  
	  I& input = exact(input_);
	  const J& values = exact(values_);

	  internal::take_as_init_tests(input, values);
	  // Extra test:
	  mln_precondition(values.domain() == input.domain());

	  border::resize_equal(input, values);
	  
	  mln_pixter(I) p_in(input);
	  mln_pixter(const J) p_v(values);
	  for_all_2(p_in, p_v)
	    p_in.val().take_as_init(p_v.val());

	}

      } // end of namespace mln::accu::image::impl



      // Dispatch.

      namespace internal
      {

	// With a single value.

	template <typename I, typename V>
	inline
	void
	take_as_init_dispatch(trait::image::speed::any,
			      Image<I>& input, const V& v)
	{
	  impl::generic::take_as_init(input, v);
	}

	template <typename I, typename V>
	inline
	void
	take_as_init_dispatch(trait::image::speed::fastest,
			      Image<I>& input, const V& v)
	{
	  impl::take_as_init_fastest(input, v);
	}

	template <typename I, typename V>
	inline
	void
	take_as_init_dispatch(Image<I>& input, const V& v)
	{
	  take_as_init_dispatch(mln_trait_image_speed(I)(),
				input, v);
	}

	// With an image of values.

	template <typename I, typename J>
	inline
	void
	take_as_init_dispatch(trait::image::speed::any,
			      trait::image::speed::any,
			      Image<I>& input, const Image<J>& values)
	{
	  impl::generic::take_as_init(input, values);
	}

	template <typename I, typename J>
	inline
	void
	take_as_init_dispatch(trait::image::speed::fastest,
			      trait::image::speed::fastest,
			      Image<I>& input, const Image<J>& values)
	{
	  if (exact(values).domain() == exact(input).domain())
	    impl::take_as_init_fastest(input, values);
	  else
	    impl::generic::take_as_init(input, values);
	}

	template <typename I, typename J>
	inline
	void
	take_as_init_dispatch(Image<I>& input,
			      const Image<J>& values)
	{
	  take_as_init_dispatch(mln_trait_image_speed(I)(),
				mln_trait_image_speed(J)(),
				input, values);
	}

      } // end of namespace mln::accu::image::internal


      // Facades.

      template <typename I>
      inline
      void
      take_as_init(Image<I>& input, const mln_deduce(I, value, argument)& v)
      {
	mln_trace("accu::image::take_as_init");

	mlc_is_a(mln_value(I), Accumulator)::check();

	mln_precondition(exact(input).is_valid());
	internal::take_as_init_dispatch(input, v);

      }

      template <typename I, typename J>
      inline
      void
      take_as_init(Image<I>& input, const Image<J>& values)
      {
	mln_trace("accu::image::take_as_init");

	typedef mln_value(I) A;
	mlc_is_a(A, Accumulator)::check();
	mlc_converts_to(mln_value(J), mln_argument(A))::check();

	internal::take_as_init_tests(input, values);
	internal::take_as_init_dispatch(input, values);

      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::image

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_IMAGE_TAKE_AS_INIT_HH
