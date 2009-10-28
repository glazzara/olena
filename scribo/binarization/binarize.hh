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

#ifndef SCRIBO_BINARIZATION_BINARIZE_HH
# define SCRIBO_BINARIZATION_BINARIZE_HH

# include <mln/core/concept/image.hh>
# include <mln/value/concept/vectorial.hh>

/// \file
///
/// Binarize an image from a threshold image.

namespace scribo
{

  using namespace mln;


  namespace binarization
  {

    namespace internal
    {

      template <typename I, typename T>
      void
      binarize_tests(const Image<I>& input, const Image<T>& threshold)
      {
	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(threshold).is_valid());
	mln_precondition(exact(input).domain() == exact(threshold).domain());
	mlc_is_not_a(mln_value(I), value::Vectorial)::check();
	(void) input;
	(void) threshold;
      }

    } // end of namespace scribo::binarization::internal


    namespace impl
    {

      namespace generic
      {

	template <typename I, typename T>
	mln_ch_value(I, bool)
	binarize(const Image<I>& input_, const Image<T>& threshold_)
	{
	  trace::entering("scribo::binarization::impl::generic::binarize");

	  internal::binarize_tests(input_, threshold_);

	  const I& input = exact(input_);
	  const T& threshold = exact(threshold_);

	  mln_ch_value(I, bool) output;
	  initialize(output, input);

	  mln_piter(I) p(input.domain());
	  for_all(p)
	    output(p) = (input(p) <= threshold(p));


	  trace::exiting("scribo::binarization::impl::generic::binarize");
	  return output;
	}


      } // end of namespace scribo::binarization::impl::generic


      template <typename I, typename T>
      mln_ch_value(I, bool)
      binarize_fastest(const Image<I>& input_, const Image<T>& threshold_)
      {
	trace::entering("scribo::binarization::impl::generic::binarize_fastest");
	internal::binarize_tests(input_, threshold_);

	const I& input = exact(input_);
	const T& threshold = exact(threshold_);

	typedef mln_ch_value(I, bool) O;
	O output;
	initialize(output, input);

	mln_pixter(const I) pi(input);
	mln_pixter(const T) pt(threshold);
	mln_pixter(O) po(output);
	for_all_3(pi, pt, po)
	  po.val() = pi.val() <= pt.val();

	trace::exiting("scribo::binarization::impl::generic::binarize_fastest");
	return output;
      }



    } // end of namespace scribo::binarization::impl


    namespace internal
    {

      template <typename I, typename T>
      mln_ch_value(I, bool)
      binarize_dispatch(trait::image::value_alignment::any,
			trait::image::speed::any,
			const Image<I>& input, const Image<T>& threshold)
      {
	return impl::generic::binarize(input, threshold);
      }


      template <typename I, typename T>
      mln_ch_value(I, bool)
      binarize_dispatch(trait::image::value_alignment::with_grid,
			trait::image::speed::fastest,
			const Image<I>& input, const Image<T>& threshold)
      {
	return impl::binarize_fastest(input, threshold);
      }

      template <typename I, typename T>
      mln_ch_value(I, bool)
      binarize_dispatch(const Image<I>& input, const Image<T>& threshold)
      {
	return binarize_dispatch(mln_trait_image_value_alignment(I)(),
				 mln_trait_image_speed(I)(),
				 exact(input), exact(threshold));
      }

    } // end of namespace scribo::binarization::internal


    template <typename I, typename T>
    mln_ch_value(I, bool)
    binarize(const Image<I>& input, const Image<T>& threshold)
    {
      trace::entering("scribo::binarization::binarize");

      internal::binarize_tests(input, threshold);

      mln_ch_value(I, bool)
	output = internal::binarize_dispatch(input, threshold);

      trace::exiting("scribo::binarization::binarize");
      return output;
    }


  } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // ! SCRIBO_BINARIZATION_BINARIZE_HH
