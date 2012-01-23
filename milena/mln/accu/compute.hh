// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_ACCU_COMPUTE_HH
# define MLN_ACCU_COMPUTE_HH

/// \file
///
/// Compute an accumulator on image pixels, i.e., couples (site,
/// value).
///
/// \todo Specialize for fastest images.

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/core/concept/image.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace accu
  {

    /*! \brief Make an accumulator compute the pixels of the image \p
        input.

        \param[in] a An accumulator.
        \param[in] input The input image.

        This routine runs: \n
          a.take(make::pix(input, p));
          on all pixels on the images.

        \warning This routine does not perform a.init().

	\ingroup mlnaccucompute
    */
    template <typename A, typename I>
    mln_result(A)
    compute(const Accumulator<A>& a, const Image<I>& input);


    /*! \brief Make an accumulator compute the pixels of the image \p
        input.

        \param[in] a A meta accumulator.
        \param[in] input The input image.

        This routine runs: \n
          a.take(make::pix(input, p));
          on all pixels on the images.

        \warning This routine does not perform a.init().

	\ingroup mlnaccucompute
    */
    template <typename A, typename I>
    mln_meta_accu_result(A, util::pix<I>)
    compute(const Meta_Accumulator<A>& a, const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY


    namespace impl
    {

      namespace generic
      {

	template <typename A, typename I>
	mln_result(A)
	compute(const Accumulator<A>&, const Image<I>& input_)
	{
	  trace::entering("accu::impl::generic::compute");

	  const I& input = exact(input_);
	  mln_precondition(input.is_valid());

	  A a;
	  mln_piter(I) p(input.domain());
	  for_all(p)
	    a.take(make::pix(input, p));

	  trace::exiting("accu::impl::generic::compute");
	  return a.to_result();
	}

      } // end of namespace mln::accu::impl::generic

    } // end of namespace mln::accu::impl


    namespace internal
    {

      template <typename A, typename I>
      inline
      mln_result(A)
      compute_dispatch(const Accumulator<A>& a, const Image<I>& input)
      {
	return impl::generic::compute(a, input);
      }

    } // end of namespace mln::accu::internal



    template <typename A, typename I>
    inline
    mln_result(A)
    compute(const Accumulator<A>& a, const Image<I>& input)
    {
      trace::entering("accu::compute");

      mln_precondition(exact(input).is_valid());
      mln_result(A) output = internal::compute_dispatch(a, input);

      trace::exiting("accu::compute");
      return output;
    }

    template <typename A, typename I>
    inline
    mln_meta_accu_result(A, util::pix<I>)
    compute(const Meta_Accumulator<A>& a, const Image<I>& input)
    {
      trace::entering("accu::compute");

      mln_precondition(exact(input).is_valid());

      typedef mln_accu_with(A, util::pix<I>) A_;
      util::pix<I> pix_(exact(input), mln_psite(I)());
      A_ a_ = accu::unmeta(exact(a), pix_);

      mln_result(A_) output = internal::compute_dispatch(a_, input);

      trace::exiting("accu::compute");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_COMPUTE_HH
