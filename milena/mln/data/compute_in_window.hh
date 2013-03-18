// Copyright (C) 2011, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_DATA_COMPUTE_IN_WINDOW_HH
# define MLN_DATA_COMPUTE_IN_WINDOW_HH

/// \file
///
/// Compute an accumulator for each image pixel values using neighbor
/// pixel values.

# include <mln/accu/image/init.hh>
# include <mln/accu/image/to_result.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/border/mirror.hh>
# include <mln/extension/adjust.hh>

# include <mln/debug/println_with_border.hh>

namespace mln
{

  namespace data
  {

    /*! \brief Compute an accumulator for each image pixel values
     * using neighbor pixel values.
     *
     * \ingroup mlnaccucompute
     */
    template <typename A, typename I, typename W>
    mln_ch_value(I, mln_result(A))
    compute_in_window(const Accumulator<A>& a, const Image<I>& input,
		      const Window<W>& win);

    /*! \overload
     *
     * \param[in] a A meta-accumulator.
     * \param[in] input The input image.
     * \return The accumulator result.
     *
     * \ingroup mlnaccucompute
     */
    template <typename A, typename I, typename W>
    mln_ch_value(I, mln_result(A))
    compute_in_window(const Meta_Accumulator<A>& a, const Image<I>& input,
		      const Window<W>& win);



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename A, typename I, typename W>
      void
      compute_in_window_tests(const Accumulator<A>& a, const Image<I>& input_,
			      const Window<W>& win_)
      {
	const W& win = exact(win_);
	const I& input = exact(input_);

	(void) a;
	(void) win;
	(void) input;

	mln_assertion(win.is_valid());
	mln_assertion(input.is_valid());
      }

    } // end of namespace mln::data::internal


    namespace impl
    {

      namespace generic
      {

	template <typename A, typename I, typename W>
	mln_ch_value(I, mln_result(A))
	compute_in_window(const Accumulator<A>& a,
			  const Image<I>& input_,
			  const Window<W>& win_)
	{
	  mln_trace("mln::impl::generic::compute_in_window");

	  const I& input = exact(input_);
	  const W& win = exact(win_);

	  internal::compute_in_window_tests(a, input, win);

	  mln_ch_value(I, A) accu;
	  initialize(accu, input);

	  accu::image::init(accu);

	  mln_piter(I) p(accu.domain());
	  mln_qiter(W) q(win, p);

	  for_all(p)
	    for_all(q)
   	      if (input.domain().has(q))
		accu(p).take(input(q));

	  mln_ch_value(I, mln_result(A))
	    output = accu::image::to_result(accu);

	  return output;
	}

      } // end of namespace mln::impl::generic


      template <typename A, typename I, typename W>
      mln_ch_value(I, mln_result(A))
      compute_in_window_fastest(const Accumulator<A>& a,
				const Image<I>& input_,
				const Window<W>& win_)
      {
	mln_trace("mln::impl::generic::compute_in_window_fastest");

	const W& win = exact(win_);
	const I& input = exact(input_);

	internal::compute_in_window_tests(a, input, win);

	extension::adjust(input, win);
	border::mirror(input);

	typedef mln_ch_value(I, A) J;
	J accu;
	initialize(accu, input);

	accu::image::init(accu);

	mln_pixter(J) p(accu);
	mln_qixter(J, W) q(p, win);

	for_all(p)
	  for_all(q)
	    p.val().take(input.element(q.offset()));

	mln_ch_value(I, mln_result(A))
	  output = accu::image::to_result(accu);

	return output;
      }


    } // end of namespace mln::data::impl


    // Dispatch

    namespace internal
    {

      template <typename A, typename I, typename W>
      mln_ch_value(I, mln_result(A))
      compute_in_window_dispatch(const Accumulator<A>& a,
				 const Image<I>& input,
				 const Window<W>& win,
				 trait::image::speed::fastest)
      {
	return impl::compute_in_window_fastest(a, input, win);
      }

      template <typename A, typename I, typename W>
      mln_ch_value(I, mln_result(A))
      compute_in_window_dispatch(const Accumulator<A>& a,
				 const Image<I>& input,
				 const Window<W>& win,
				 trait::image::speed::any)
      {
	return impl::generic::compute_in_window(a, input, win);
      }

      template <typename A, typename I, typename W>
      mln_ch_value(I, mln_result(A))
      compute_in_window_dispatch(const Accumulator<A>& a,
				 const Image<I>& input,
				 const Window<W>& win)
      {
	return compute_in_window_dispatch(a, input, win,
					  mln_trait_image_speed(I)());
      }

    } // end of namespace mln::data::internal



    // Facades.

    template <typename A, typename I, typename W>
    mln_ch_value(I, mln_result(A))
    compute_in_window(const Accumulator<A>& a, const Image<I>& input,
		      const Window<W>& win)
    {
      mln_trace("data::compute_in_window");

      internal::compute_in_window_tests(a, input, win);

      mln_ch_value(I, mln_result(A))
	output = data::internal::compute_in_window_dispatch(a, input, win);

      return output;
    }


    template <typename A, typename I, typename W>
    mln_ch_value(I, mln_result(A))
    compute_in_window(const Meta_Accumulator<A>& a, const Image<I>& input,
		      const Window<W>& win)
    {
      typedef mln_accu_with(A, mln_value(I)) A_;
      A_ a_ = accu::unmeta(exact(a), mln_value(I)());

      return data::compute_in_window(a_, input, win);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_COMPUTE_IN_WINDOW_HH
