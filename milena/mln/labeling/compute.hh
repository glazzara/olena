// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_LABELING_COMPUTE_HH
# define MLN_LABELING_COMPUTE_HH

/// \file mln/labeling/compute.hh
///
/// Compute accumulators onto sites/values of each labeled component
/// of an image.
///
/// \todo write fastest version.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/accumulator.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/util/array.hh>
# include <mln/convert/to.hh>


namespace mln
{

  namespace labeling
  {

    /// Compute an accumulator onto the pixel values of the image \p input.
    /// for each component of the image \p label.
    ///
    /// \param[in] a An accumulator.
    /// \param[in] input The input image.
    /// \param[in] label The labeled image.
    /// \param[in] input The number of component in \p label.
    /// \return A mln::p_array of accumulator result. One result per component in
    ///	  \p label.
    ///
    template <typename A, typename I, typename L>
    util::array<mln_result(A)>
    compute(const Accumulator<A>& a,
	    const Image<I>& input,
	    const Image<L>& label,
	    const mln_value(L)& nlabels);

    /// Compute an accumulator onto the pixel values of the image \p input.
    ///  for each component of the image \p label.
    ///
    /// \param[in] a A meta-accumulator.
    /// \param[in] input The input image.
    /// \param[in] label The labeled image.
    /// \param[in] input The number of component in \p label.
    /// \return A mln::p_array of accumulator result. One result per component in
    ///	  \p label.
    ///
    template <typename A, typename I, typename L>
    util::array<mln_accu_with(A, mln_value(I))::result>
    compute(const Meta_Accumulator<A>& a,
	    const Image<I>& input,
	    const Image<L>& label,
	    const mln_value(L)& nlabels);

    /// Compute an accumulator onto the pixel sites of each component domain of
    /// \p label.
    ///
    /// \param[in] a An accumulator.
    /// \param[in] label The labeled image.
    /// \param[in] input The number of component in \p label.
    /// \return A mln::p_array of accumulator result. One result per component in
    ///	  \p label.
    ///
    template <typename A, typename L>
    util::array<mln_result(A)>
    compute(const Accumulator<A>& a,
	    const Image<L>& label,
	    const mln_value(L)& nlabels);

    /// Compute an accumulator onto the pixel sites of each component domain of
    /// \p label.
    ///
    /// \param[in] a A meta-accumulator.
    /// \param[in] label The labeled image.
    /// \param[in] input The number of component in \p label.
    /// \return A mln::p_array of accumulator result. One result per component in
    ///	  \p label.
    ///
    template <typename A, typename L>
    util::array<mln_accu_with(A, mln_psite(L))::result>
    compute(const Meta_Accumulator<A>& a,
	    const Image<L>& label,
	    const mln_value(L)& nlabels);



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename A, typename L>
      inline
      void
      compute_tests(const Accumulator<A>& a,
		    const Image<L>& label,
		    const mln_value(L)& nlabels)
      {
	mln_precondition(exact(label).is_valid());
	mlc_is_a(mln_value(L), mln::value::Symbolic)::check();
	(void) a;
	(void) label;
	(void) nlabels;
      }


      template <typename A, typename I, typename L>
      inline
      void
      compute_tests(const Accumulator<A>& a,
		    const Image<I>& input,
		    const Image<L>& label,
		    const mln_value(L)& nlabels)
      {
	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(label).is_valid());
	mlc_is_a(mln_value(L), mln::value::Symbolic)::check();
	(void) a;
	(void) input;
	(void) label;
	(void) nlabels;
      }

    } // end of namespace mln::labeling::internal



    namespace impl
    {

      namespace generic
      {

	template <typename A, typename L>
	inline
	util::array<mln_result(A)>
	compute(const Accumulator<A>& a_,
		const Image<L>& label_,
		const mln_value(L)& nlabels)
	{
	  trace::entering("labeling::impl::generic::compute");
	  internal::compute_tests(a_, label_, nlabels);

	  const A& a = exact(a_);
	  const L& label = exact(label_);

	  util::array<A> accus(nlabels.next(), a);

	  mln_piter(L) p(label.domain());
	  for_all(p)
	    accus[label(p)].take(p);

	  trace::exiting("labeling::impl::generic::compute");
	  return convert::to< util::array<mln_result(A)> >(accus);
	}


	template <typename A, typename I, typename L>
	inline
	util::array<mln_result(A)>
	compute(const Accumulator<A>& a_,
		const Image<I>& input_,
		const Image<L>& label_,
		const mln_value(L)& nlabels)
	{
	  trace::entering("labeling::impl::generic::compute");
	  internal::compute_tests(a_, input_, label_, nlabels);

	  const A& a = exact(a_);
	  const I& input = exact(input_);
	  const L& label = exact(label_);

	  util::array<A> accus (nlabels.next(), a);

	  mln_piter(I) p(input.domain());
	  for_all(p)
	    accus[label(p)].take(input(p));

	  trace::exiting("labeling::impl::generic::compute");
	  return convert::to< util::array<mln_result(A)> >(accus);
	}

      } // end of namespace mln::labeling::impl::generic

    } // end of namespace mln::labeling::impl



    namespace internal
    {

      template <typename A, typename L>
      inline
      util::array<mln_result(A)>
      compute_dispatch(const Accumulator<A>& a,
		       const Image<L>& label,
		       const mln_value(L)& nlabels)
      {
	return impl::generic::compute(a, label, nlabels);
      }


      template <typename A, typename I, typename L>
      inline
      util::array<mln_result(A)>
      compute_dispatch(const Accumulator<A>& a,
		       const Image<I>& input,
		       const Image<L>& label,
		       const mln_value(L)& nlabels)
      {
	return impl::generic::compute(a, input, label, nlabels);
      }

    } // end of namespace mln::labeling::internal



    /// Facades.

    template <typename A, typename I, typename L>
    inline
    util::array<mln_result(A)>
    compute(const Accumulator<A>& a,
	    const Image<I>& input,
	    const Image<L>& label,
	    const mln_value(L)& nlabels)
    {
      trace::entering("labeling::compute");

      internal::compute_tests(a, input, label, nlabels);

      typedef util::array<mln_result(A)> R;
      R res = internal::compute_dispatch(a, input, label, nlabels);

      trace::exiting("labeling::compute");
      return res;
    }

    template <typename A, typename I, typename L>
    inline
    util::array<mln_accu_with(A, mln_value(I))::result>
    compute(const Meta_Accumulator<A>& a,
	    const Image<I>& input,
	    const Image<L>& label,
	    const mln_value(L)& nlabels)
    {
      typedef mln_accu_with(A, mln_value(I)) A_;
      A_ a_ = accu::unmeta(exact(a), mln_value(I)());

      return compute(a_, input, label, nlabels);
    }


    template <typename A, typename L>
    inline
    util::array<mln_result(A)>
    compute(const Accumulator<A>& a,
	    const Image<L>& label,
	    const mln_value(L)& nlabels)
    {
      trace::entering("labeling::compute");

      internal::compute_tests(a, label, nlabels);

      typedef util::array<mln_result(A)> R;
      R res = internal::compute_dispatch(a, label, nlabels);

      mln_postcondition(res.nelements() == nlabels.next());

      trace::exiting("labeling::compute");
      return res;
    }


    template <typename A, typename L>
    inline
    util::array<mln_accu_with(A, mln_psite(L))::result>
    compute(const Meta_Accumulator<A>& a,
	    const Image<L>& label,
	    const mln_value(L)& nlabels)
    {
      typedef mln_accu_with(A, mln_psite(L)) A_;
      A_ a_ = accu::unmeta(exact(a), mln_psite(L)());

      return compute(a_, label, nlabels);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_COMPUTE_HH
