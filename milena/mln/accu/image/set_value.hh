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

#ifndef MLN_ACCU_IMAGE_SET_VALUE_HH
# define MLN_ACCU_IMAGE_SET_VALUE_HH

/// \file
///
/// Set the values of an image of accumulators.

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
      set_value(Image<I>& input, const mln_deduce(I, value, result)& res);

      template <typename I, typename J>
      void
      set_value(Image<I>& input, const Image<J>& res);


# ifndef MLN_INCLUDE_ONLY


      // Tests.

      namespace internal
      {

	template <typename I, typename J>
	inline
	void
	set_value_tests(Image<I>& input_, const Image<J>& res_)
	{
	  I& input = exact(input_);
	  const J& res = exact(res_);

	  mln_precondition(input.is_valid());
	  mln_precondition(res.is_valid());

	  mln_precondition(res.domain() <= input.domain());

	  (void) input;
	  (void) res;
	}

      } // end of namespace mln::accu::image::internal



      namespace impl
      {

	// Generic versions.

	namespace generic
	{

	  template <typename I>
	  inline
	  void
	  set_value(Image<I>& input_, const mln_deduce(I, value, result)& res)
	  {
	    mln_trace("accu::impl::image::generic::set_value");

	    typedef mln_value(I) A;
	    mlc_is_a(A, Accumulator)::check();
	    mlc_equal(mln_trait_accumulator_has_set_value(A),
		      trait::accumulator::has_set_value::yes)::check();

	    I& input = exact(input_);
	    mln_precondition(input.is_valid());

	    mln_piter(I) p(input.domain());
	    for_all(p)
	      input(p).set_value(res);

	  }

	  template <typename I, typename J>
	  inline
	  void
	  set_value(Image<I>& input_, const Image<J>& res_)
	  {
	    mln_trace("accu::impl::image::generic::set_value");

	    typedef mln_value(I) A;
	    mlc_is_a(A, Accumulator)::check();
	    mlc_equal(mln_trait_accumulator_has_set_value(A),
		      trait::accumulator::has_set_value::yes)::check();
	    mlc_converts_to(mln_value(J), mln_deduce(I, value, result))::check();

	    I& input = exact(input_);
	    const J& res = exact(res_);

	    internal::set_value_tests(input, res);

	    mln_piter(J) p(res.domain());
	    for_all(p)
	      input(p).set_value(res(p));

	  }

	} // end of namespace mln::accu::image::impl::generic


	// Fastest versions.

	template <typename I>
	inline
	void
	set_value_fastest(Image<I>& input_, const mln_deduce(I, value, result)& res)
	{
	  mln_trace("accu::impl::image::set_value_fastest");

	  typedef mln_value(I) A;
	  mlc_is_a(A, Accumulator)::check();
	  mlc_equal(mln_trait_accumulator_has_set_value(A),
		    trait::accumulator::has_set_value::yes)::check();
	  
	  I& input = exact(input_);
	  mln_precondition(input.is_valid());
	  
	  mln_pixter(I) px(input);
	  for_all(px)
	    px.val().set_value(res);

	}

	template <typename I, typename J>
	inline
	void
	set_value_fastest(Image<I>& input_, const Image<J>& res_)
	{
	  mln_trace("accu::impl::image::set_value_fastest");

	  typedef mln_value(I) A;
	  mlc_is_a(A, Accumulator)::check();
	  mlc_equal(mln_trait_accumulator_has_set_value(A),
		    trait::accumulator::has_set_value::yes)::check();
	  mlc_converts_to(mln_value(J), mln_deduce(I, value, result))::check();
	  
	  I& input = exact(input_);
	  const J& res = exact(res_);
	  
	  internal::set_value_tests(input, res);
	  mln_precondition(res.domain() == input.domain());

	  border::resize_equal(input, res);

	  mln_pixter(I) p_in(input);
	  mln_pixter(const J) p_res(res);
	  for_all_2(p_in, p_res)
	    p_in.val().set_value(p_res.val());

	}

      } // end of namespace mln::accu::image::impl



      // Dispatch.

      namespace internal
      {

	// 'res' as value.

	template <typename I>
	inline
	void
	set_value_dispatch(trait::image::speed::any,
			   Image<I>& input, const mln_deduce(I, value, result)& res)
	{
	  impl::generic::set_value(input, res);
	}

	template <typename I>
	void
	set_value_dispatch(trait::image::speed::fastest,
			   Image<I>& input, const mln_deduce(I, value, result)& res)
	{
	  impl::set_value_fastest(input, res);
	}

	template <typename I>
	inline
	void
	set_value_dispatch(Image<I>& input, const mln_deduce(I, value, result)& res)
	{
	  set_value_dispatch(mln_trait_image_speed(I)(),
			     input, res);
	}

	// 'res' as image.

	template <typename I, typename J>
	inline
	void
	set_value_dispatch(trait::image::speed::any,
			   trait::image::speed::any,
			   Image<I>& input, const Image<J>& res)
	{
	  impl::generic::set_value(input, res);
	}

	template <typename I, typename J>
	inline
	void
	set_value_dispatch(trait::image::speed::fastest,
			   trait::image::speed::fastest,
			   Image<I>& input, const Image<J>& res)
	{
	  if (exact(res).domain() == exact(input).domain())
	    impl::set_value_fastest(input, res);
	  else
	    impl::generic::set_value(input, res);
	}

	template <typename I, typename J>
	inline
	void
	set_value_dispatch(Image<I>& input, const Image<J>& res)
	{
	  set_value_dispatch(mln_trait_image_speed(I)(),
			     mln_trait_image_speed(J)(),
			     input, res);
	}

      } // end of namespace mln::accu::image::internal


      // Facades.

      template <typename I>
      inline
      void
      set_value(Image<I>& input, const mln_deduce(I, value, result)& res)
      {
	mln_trace("accu::image::set_value");

	typedef mln_value(I) A;
	mlc_is_a(A, Accumulator)::check();
	mlc_equal(mln_trait_accumulator_has_set_value(A),
		  trait::accumulator::has_set_value::yes)::check();

	mln_precondition(exact(input).is_valid());
	internal::set_value_dispatch(input, res);

      }

      template <typename I, typename J>
      inline
      void
      set_value(Image<I>& input, const Image<J>& res)
      {
	mln_trace("accu::image::set_value");

	typedef mln_value(I) A;
	mlc_is_a(A, Accumulator)::check();
	mlc_equal(mln_trait_accumulator_has_set_value(A),
		  trait::accumulator::has_set_value::yes)::check();
	mlc_converts_to(mln_value(J), mln_deduce(I, value, result))::check();

	internal::set_value_tests(input, res);
	internal::set_value_dispatch(input, res);

      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::image

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_IMAGE_SET_VALUE_HH
