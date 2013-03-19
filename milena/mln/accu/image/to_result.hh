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

#ifndef MLN_ACCU_IMAGE_TO_RESULT_HH
# define MLN_ACCU_IMAGE_TO_RESULT_HH

/// \file
///
/// Convert an image of accumulators into a result image.

# include <mln/core/concept/accumulator.hh>
# include <mln/core/concept/image.hh>


namespace mln
{

  namespace accu
  {

    namespace image
    {

      template <typename I>
      mln_ch_value(I, mln_deduce(I, value, result))
	to_result(const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	// Generic version.

	namespace generic
	{

	  template <typename I>
	  mln_ch_value(I, mln_deduce(I, value, result))
	    to_result(const Image<I>& input_)
	  {
	    mln_trace("accu::impl::image::generic::to_result");

	    mlc_is_a(mln_value(I), Accumulator)::check();

	    const I& input = exact(input_);
	    mln_precondition(input.is_valid());

	    typedef mln_deduce(I, value, result) R;
	    mln_ch_value(I, R) output;
	    initialize(output, input);

	    mln_piter(I) p(input.domain());
	    for_all(p)
	      output(p) = input(p).to_result();

	    return output;
	  }

	} // end of namespace mln::accu::image::impl::generic


	// Fastest version.

	template <typename I>
	mln_ch_value(I, mln_deduce(I, value, result))
	  to_result_fastest(const Image<I>& input_)
	{
	  mln_trace("accu::impl::image::to_result_fastest");

	  mlc_is_a(mln_value(I), Accumulator)::check();
	  
	  const I& input = exact(input_);
	  mln_precondition(input.is_valid());

	  typedef mln_deduce(I, value, result) R;
	  typedef mln_ch_value(I, R) O;
	  O output;
	  initialize(output, input);
	  
	  mln_pixter(const I) p_in(input);
	  mln_pixter(O)       p_out(output);
	  for_all_2(p_in, p_out)
	    p_out.val() = p_in.val().to_result();

	  return output;
	}

      } // end of namespace mln::accu::image::impl



      // Dispatch.

      namespace internal
      {

	template <typename I>
	mln_ch_value(I, mln_deduce(I, value, result))
	  to_result_dispatch(trait::image::speed::any,
			     const Image<I>& input)
	{
	  return impl::generic::to_result(input);
	}

	template <typename I>
	mln_ch_value(I, mln_deduce(I, value, result))
	  to_result_dispatch(trait::image::speed::fastest,
			     const Image<I>& input)
	{
	  return impl::to_result_fastest(input);
	}

	template <typename I>
	mln_ch_value(I, mln_deduce(I, value, result))
	  to_result_dispatch(const Image<I>& input)
	{
	  return to_result_dispatch(mln_trait_image_speed(I)(),
				    input);
	}

      } // end of namespace mln::accu::image::internal


      // Facade.

      template <typename I>
      mln_ch_value(I, mln_deduce(I, value, result))
	to_result(const Image<I>& input)
      {
	mln_trace("accu::image::to_result");

	mlc_is_a(mln_value(I), Accumulator)::check();

	mln_precondition(exact(input).is_valid());

	typedef mln_deduce(I, value, result) R;
	mln_ch_value(I, R) output;
	output = internal::to_result_dispatch(input);

	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::image

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_IMAGE_TO_RESULT_HH
