// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ACCU_IMAGE_TAKE_AS_INIT_HH
# define MLN_ACCU_IMAGE_TAKE_AS_INIT_HH

/// \file mln/accu/image/take_as_init.hh
///
/// Initialize an image of accumulators by taking a first value.

# include <mln/core/concept/accumulator.hh>
# include <mln/core/concept/image.hh>


namespace mln
{

  namespace accu
  {

    namespace image
    {

      template <typename I>
      void
      take_as_init(Image<I>& input,
		   const mln_deduce(I, value, argument)& v);



# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	// Generic version.

	namespace generic
	{

	  template <typename I>
	  void
	  take_as_init(Image<I>& input_,
		       const mln_deduce(I, value, argument)& v)
	  {
	    trace::entering("accu::impl::image::generic::take_as_init");

	    I& input = exact(input_);
	    mln_precondition(input.is_valid());

	    mln_piter(I) p(input.domain());
	    for_all(p)
	      input(p).take_as_init(v);

	    trace::exiting("accu::impl::image::generic::take_as_init");
	  }

	} // end of namespace mln::accu::image::impl::generic


	// Fastest version.

	template <typename I>
	void
	take_as_init_fastest(Image<I>& input_,
			     const mln_deduce(I, value, argument)& v)
	{
	  trace::entering("accu::impl::image::take_as_init_fastest");
	  
	  I& input = exact(input_);
	  mln_precondition(input.is_valid());
	  
	  mln_pixter(I) px(input);
	  for_all(px)
	    px.val().take_as_init(v);

	  trace::exiting("accu::impl::image::take_as_init_fastest");
	}

      } // end of namespace mln::accu::image::impl



      // Dispatch.

      namespace internal
      {

	template <typename I, typename V>
	void
	take_as_init_dispatch(trait::image::speed::any,
			      Image<I>& input, const V& v)
	{
	  impl::generic::take_as_init(input, v);
	}

	template <typename I, typename V>
	void
	take_as_init_dispatch(trait::image::speed::fastest,
			      Image<I>& input, const V& v)
	{
	  impl::take_as_init_fastest(input, v);
	}

	template <typename I, typename V>
	void
	take_as_init_dispatch(Image<I>& input, const V& v)
	{
	  take_as_init_dispatch(mln_trait_image_speed(I)(),
				input, v);
	}

      } // end of namespace mln::accu::image::internal


      // Facade.

      template <typename I>
      void
      take_as_init(Image<I>& input,
		   const mln_deduce(I, value, argument)& v)
      {
	trace::entering("accu::image::take_as_init");

	mlc_is_a(mln_value(I), Accumulator)::check();

	mln_precondition(exact(input).is_valid());
	internal::take_as_init_dispatch(input, v);

	trace::exiting("accu::image::take_as_init");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::image

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_IMAGE_TAKE_AS_INIT_HH
