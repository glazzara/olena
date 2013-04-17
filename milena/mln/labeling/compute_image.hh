// Copyright (C) 2009, 2013 EPITA Research and Development Laboratory
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

#ifndef MLN_LABELING_COMPUTE_IMAGE_HH
# define MLN_LABELING_COMPUTE_IMAGE_HH

///
/// \file
///
/// \brief Compute accumulators onto sites/values of each labeled
/// component of an image and return a new image where labels are
/// replaced by the result of the accumulator.
///
///
///

# include <mln/core/concept/image.hh>
# include <mln/core/concept/accumulator.hh>
# include <mln/core/concept/meta_accumulator.hh>

# include <mln/labeling/compute.hh>


namespace mln
{

  namespace labeling
  {

    /*! Compute an accumulator onto the pixel values of the image \p input.
      for each component of the image \p label.

      \param[in] a The mln::p_array of accumulator result.
      \param[in] input The input image (values).
      \param[in] labels The label image.
      \param[in] nlabels The count of labels.

      \return The image where labels are replaced by the result of
      the accumulator.

      \ingroup labeling
    */
    template <typename A, typename I, typename L>
    mln_ch_value(L, mln_result(A))
      compute_image(const util::array<mln_result(A)>& a,
		    const Image<I>& input,
		    const Image<L>& labels,
		    const mln_value(L)& nlabels);

    /*! \brief Compute an accumulator onto the pixel values of the
      image \p input.  for each component of the image \p label.

      \param[in] accu The accumulator.
      \param[in] input The input image (values).
      \param[in] labels The label image.
      \param[in] nlabels The count of labels.

      \return The image where labels are replaced by the result of
      the accumulator.

      \ingroup labeling
    */
    template <typename A, typename I, typename L>
    inline
    mln_ch_value(L, mln_result(A))
      compute_image(const Accumulator<A>& accu,
		    const Image<I>& input,
		    const Image<L>& labels,
		    const mln_value(L)& nlabels);


    /*! \brief Compute an accumulator onto the pixel values of the
     image \p input.  for each component of the image \p label.

     \param[in] accu The meta-accumulator.
     \param[in] input The input image (values).
     \param[in] labels The label image.
     \param[in] nlabels The count of labels.

     \return The image where labels are replaced by the result of
     the accumulator.

     \ingroup labeling
    */
    template <typename A, typename I, typename L>
    inline
    mln_ch_value(L, mln_meta_accu_result(A, mln_value(I)))
      compute_image(const Meta_Accumulator<A>& accu,
		    const Image<I>& input,
		    const Image<L>& labels,
		    const mln_value(L)& nlabels);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename A, typename L>
      mln_ch_value(L, A)
	compute_image(const util::array<A>& a,
		      const Image<L>& labels_,
		      const mln_value(L)& nlabels)
      {
	const L& labels = exact(labels_);
	(void) nlabels;

	typedef mln_ch_value(L, A) O;
	O output;
	initialize(output, labels);

	mln_piter(L) p(labels.domain());
	for_all(p)
	{
	  mln_assertion(labels(p) <= nlabels);
	  output(p) = a[labels(p)];
	}

	return output;
      }

    } // end of namespace mln::labeling::internal


    template <typename A, typename I, typename L>
    inline
    mln_ch_value(L, mln_result(A))
      compute_image(const util::array<mln_result(A)>& a,
		    const Image<L>& labels,
		    const mln_value(L)& nlabels)
    {
      (void) nlabels;

      mln_trace("labeling::compute_image");

      mln_ch_value(L, mln_result(A)) output =
	labeling::internal::compute_image(a, labels, nlabels);

      return output;
    }

    template <typename A, typename I, typename L>
    inline
    mln_ch_value(L, mln_result(A))
      compute_image(const Accumulator<A>& accu,
		    const Image<I>& input,
		    const Image<L>& labels,
		    const mln_value(L)& nlabels)
    {
      mln_trace("labeling::compute_image");

      util::array<mln_result(A)> res =
	compute(accu, input, labels, nlabels);

      mln_ch_value(L, mln_result(A)) output =
	internal::compute_image(res, labels, nlabels);

      return output;
    }

    template <typename A, typename I, typename L>
    inline
    mln_ch_value(L, mln_meta_accu_result(A, mln_value(I)))
      compute_image(const Meta_Accumulator<A>& accu,
		    const Image<I>& input,
		    const Image<L>& labels,
		    const mln_value(L)& nlabels)
    {
      mln_trace("labeling::compute_image");

      typedef mln_meta_accu_result(A, mln_value(I)) T;

      util::array<T> res =
	compute(accu, input, labels, nlabels);

      mln_ch_value(L, T) output =
	labeling::internal::compute_image(res, labels, nlabels);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln

#endif // ! MLN_LABELING_COMPUTE_IMAGE_HH
