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

#ifndef MLN_ACCU_IMAGE_INIT_HH
# define MLN_ACCU_IMAGE_INIT_HH

/// \file
///
/// Initialize an image of accumulators.

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
      init(Image<I>& input);



# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	// Generic version.

	namespace generic
	{

	  template <typename I>
	  void
	  init(Image<I>& input_)
	  {
	    mln_trace("accu::impl::image::generic::init");

	    mlc_is_a(mln_value(I), Accumulator)::check();

	    I& input = exact(input_);
	    mln_precondition(input.is_valid());

	    mln_piter(I) p(input.domain());
	    for_all(p)
	      input(p).init();

	  }

	} // end of namespace mln::accu::image::impl::generic


	// Fastest version.

	template <typename I>
	void
	init_fastest(Image<I>& input_)
	{
	  mln_trace("accu::impl::image::init_fastest");

	  mlc_is_a(mln_value(I), Accumulator)::check();
	  
	  I& input = exact(input_);
	  mln_precondition(input.is_valid());
	  
	  mln_pixter(I) px(input);
	  for_all(px)
	    px.val().init();

	}

      } // end of namespace mln::accu::image::impl



      // Dispatch.

      namespace internal
      {

	template <typename I>
	void
	init_dispatch(trait::image::speed::any,
		      Image<I>& input)
	{
	  impl::generic::init(input);
	}

	template <typename I>
	void
	init_dispatch(trait::image::speed::fastest,
		      Image<I>& input)
	{
	  impl::init_fastest(input);
	}

	template <typename I>
	void
	init_dispatch(Image<I>& input)
	{
	  init_dispatch(mln_trait_image_speed(I)(),
			input);
	}

      } // end of namespace mln::accu::image::internal


      // Facade.

      template <typename I>
      void
      init(Image<I>& input)
      {
	mln_trace("accu::image::init");

	mlc_is_a(mln_value(I), Accumulator)::check();

	mln_precondition(exact(input).is_valid());
	internal::init_dispatch(input);

      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::image

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_IMAGE_INIT_HH
