// Copyright (C) 2009, 2010, 2012 EPITA Research and Development
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

#ifndef MLN_SUBSAMPLING_ANTIALIASED_HH
# define MLN_SUBSAMPLING_ANTIALIASED_HH

/// \file
///
/// Antialiased subsampling.


#include <mln/core/concept/image.hh>
#include <mln/border/thickness.hh>
#include <mln/extension/adjust_duplicate.hh>
#include <mln/core/macros.hh>

namespace mln
{

  namespace subsampling
  {

    /*! \brief Antialiased subsampling.

      \param[in] input A gray-level image.
      \param[in] factor Subsampling ratio. Must be divisible by 2 or 3.
      \param[in] output_domain Force output domain.
      \param[in] border_thickness Force output border thickness.

     */
    template <typename I>
    inline
    mln_concrete(I)
    antialiased(const Image<I>& input,
		unsigned factor,
		const mln_domain(I)& output_domain,
		unsigned border_thickness);


    /*! \overload
     */
    template <typename I>
    mln_concrete(I)
    antialiased(const Image<I>& input, unsigned factor);



# ifndef MLN_INCLUDE_ONLY



    // Tests

    namespace internal
    {

      template <typename I>
      inline
      void
      antialiased_tests(const Image<I>& input,
			unsigned factor,
			const mln_domain(I)& output_domain,
			unsigned border_thickness)
      {
	typedef mln_site(I) P;

	mlc_is_a(mln_domain(I), Box)::check();
	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(input).domain().pmin() == literal::origin);

	(void) input;
	(void) factor;
	(void) output_domain;
	(void) border_thickness;
      }

    } // end of namespace mln::subsampling::internal




    // Implementations.

    namespace impl
    {

      namespace generic
      {

	template <typename I>
	inline
	mln_concrete(I)
	antialiased(const Image<I>& input_,
		    unsigned factor,
		    const mln_domain(I)& output_domain,
		    unsigned border_thickness)
	{
	  (void) input_;
	  (void) factor;
	  (void) output_domain;
	  (void) border_thickness;

	  // To be written...
	  mlc_abort(I)::check();

	  mln_concrete(I) output;
	  return output;
	}

      } // end of namespace mln::subsampling::impl::generic



      template <typename I>
      inline
      mln_concrete(I)
      antialiased_2d_fastest_scalar(const Image<I>& input_,
				    unsigned factor,
				    const mln_domain(I)& output_domain,
				    unsigned border_thickness)
      {
	trace::entering("subsampling::impl::antialiased_2d_fastest");

	internal::antialiased_tests(input_, factor,
				    output_domain, border_thickness);

	const I& input = exact(input_);

	// No reduction.
	if (factor == 1)
	{
	  trace::exiting("subsampling::impl::antialiased_2d_fastest");
	  return duplicate(input);
	}

	typedef mln_value(I) V;
	typedef mln_sum(V) S;

	typedef mln_site(I) P;
	box<P> b = output_domain;
	if (!b.is_valid())
	{
	  P pmin = input.domain().pmin() / factor,
	    pmax = input.domain().pmax() / factor;
	  b = box<P>(pmin, pmax);
	}
	typedef mln_concrete(I) O;
	O output(b, border_thickness);

	// Make sure there is enough data in input border.
	unsigned input_border = factor - std::min(input.nrows() % factor,
						  input.ncols() % factor);
	extension::adjust_duplicate(input, input_border);


	typedef const V* ptr_t;

	util::array<ptr_t> ptrs(factor, 0);
	for (unsigned i = 0; i < factor; ++i)
	  ptrs[i] = & input.at_(i, 0);

	mln_box_runstart_piter(O) s(output.domain());
	const unsigned n = s.run_length();
	const unsigned
	  factor_2 = factor * factor,
	  factor_round = factor_2 / 2;
	unsigned offset = input.delta_offset(point2d(factor,0) - point2d(0,factor*n));

	for_all(s)
	{
	  mln_value(O)* po = & output(s);
	  for (unsigned i = 0; i < n; ++i)
	  {
	    mln_sum(V) s = literal::zero;
	    for (unsigned j = 0; j < factor; ++j)
	      for (unsigned k = 0; k < factor; ++k)
		s += *ptrs[j]++;

	    convert::from_to((s + factor_round) / factor_2, *po);
	    ++po;
	  }

	  for (unsigned j = 0; j < factor; ++j)
	    ptrs[j] += offset;
	}

	trace::exiting("subsampling::impl::antialiased_2d_fastest");
	return output;
      }



      template <typename I>
      inline
      mln_concrete(I)
      antialiased_2d_fastest_rgb(const Image<I>& input_,
				 unsigned factor,
				 const mln_domain(I)& output_domain,
				 unsigned border_thickness)
      {
	trace::entering("subsampling::impl::antialiased_2d_rgb");

	internal::antialiased_tests(input_, factor,
				    output_domain, border_thickness);

	const I& input = exact(input_);


	// No reduction.
	if (factor == 1)
	{
	  trace::exiting("subsampling::impl::antialiased_2d_rgb");
	  return duplicate(input);
	}

	typedef mln_value(I) V;
	typedef mln_sum(V) S;

	typedef mln_site(I) P;
 	box<P> b = output_domain;
 	if (!b.is_valid())
 	{
	  P pmin = input.domain().pmin() / factor,
	    pmax = input.domain().pmax() / factor;
	  b = box<P>(pmin, pmax);
 	}
	typedef mln_concrete(I) O;
	O output(b, border_thickness);

	// Make sure there is enough data in input border.
	unsigned input_border = factor - std::min(input.nrows() % factor,
						  input.ncols() % factor);
	extension::adjust_duplicate(input, input_border);


	typedef const V* ptr_t;

	util::array<ptr_t> ptrs(factor, 0);
	for (unsigned i = 0; i < factor; ++i)
	  ptrs[i] = & input.at_(i, 0);

	mln_box_runstart_piter(O) s(output.domain());
	const unsigned n = s.run_length();
	const unsigned
	  factor_2 = factor * factor,
	  factor_round = factor_2 / 2;
	unsigned offset = input.delta_offset(point2d(factor,0) - point2d(0,factor*n));

	for_all(s)
	{
	  mln_value(O)* po = & output(s);
	  for (unsigned i = 0; i < n; ++i)
	    {
	      mln_sum(V) s = literal::zero;
	      for (unsigned j = 0; j < factor; ++j)
		for (unsigned k = 0; k < factor; ++k)
		{
		  algebra::vec<3, float> tmp = *ptrs[j]++;
		  s += tmp;
		}

	      // FIXME: should be removed and replaced by the
	      // commented part below.
	      for (unsigned j = 0; j < P::dim; ++j)
		s[j] += factor_round;

	      *po++ = (s /*+ factor_round*/) / factor_2;
	    }
	  for (unsigned j = 0; j < factor; ++j)
	    ptrs[j] += offset;
	}

	trace::exiting("subsampling::impl::antialiased_2d_rgb");
	return output;
      }



    } // end of namespace mln::subsampling::impl



    // Dispatch.

    namespace internal
    {

      template <unsigned dim, typename I>
      inline
      mln_concrete(I)
      antialiased_dispatch(trait::image::value_alignment::any,
			   trait::image::value_storage::any,
			   trait::image::value_access::any,
			   const Image<I>& input,
			   unsigned factor,
			   const mln_domain(I)& output_domain,
			   unsigned border_thickness)
      {
	(void) input;
	(void) factor;
	(void) output_domain;
	(void) border_thickness;

	// Not implemented yet.
	mlc_abort(I)::check();
      }

      template <typename I>
      inline
      mln_concrete(I)
      antialiased_2d_fastest_dispatch(const mln_value(I)&,
				      const Image<I>& input,
				      unsigned factor,
				      const mln_domain(I)& output_domain,
				      unsigned border_thickness)
      {
	return impl::antialiased_2d_fastest_scalar(input, factor,
						   output_domain,
						   border_thickness);
      }


      template <unsigned n, typename I>
      inline
      mln_concrete(I)
      antialiased_2d_fastest_dispatch(const value::rgb<n>&,
				      const Image<I>& input,
				      unsigned factor,
				      const mln_domain(I)& output_domain,
				      unsigned border_thickness)
      {
	return impl::antialiased_2d_fastest_rgb(input, factor,
						output_domain,
						border_thickness);
      }


      template <typename I>
      inline
      mln_concrete(I)
      antialiased_2d_fastest_dispatch(const Image<I>& input,
				      unsigned factor,
				      const mln_domain(I)& output_domain,
				      unsigned border_thickness)
      {
	typedef mln_value(I) V;
	return antialiased_2d_fastest_dispatch(V(), input, factor,
					       output_domain,
					       border_thickness);
      }


      template <typename I>
      inline
      mln_concrete(I)
      antialiased_dispatch_2d(trait::image::value_alignment::with_grid,
			      trait::image::value_storage::one_block,
			      trait::image::value_access::direct,
			      const Image<I>& input,
			      unsigned factor,
			      const mln_domain(I)& output_domain,
			      unsigned border_thickness)
      {
	return antialiased_2d_fastest_dispatch(input, factor,
					       output_domain,
					       border_thickness);
      }


      template <typename I>
      inline
      mln_concrete(I)
      antialiased_dispatch(const Image<I>& input,
			   unsigned factor,
			   const mln_domain(I)& output_domain,
			   unsigned border_thickness)
      {
	unsigned dim = mln_site_(I)::dim;

	if (dim == 2)
	  return antialiased_dispatch_2d(
	    mln_trait_image_value_alignment(I)(),
	    mln_trait_image_value_storage(I)(),
	    mln_trait_image_value_access(I)(),
	    input,
	    factor,
	    output_domain,
	    border_thickness);
	else
	  trace::warning("Not implemented yet.");

	mln_concrete(I) output;
	return output;
      }

    } // end of namespace mln::subsampling::internal



    // Facades.

    template <typename I>
    inline
    mln_concrete(I)
    antialiased(const Image<I>& input,
		unsigned factor,
		const mln_domain(I)& output_domain,
		unsigned border_thickness)
    {
      trace::entering("subsampling::antialiased");

      typedef mln_site(I) P;

      internal::antialiased_tests(input, factor,
				  output_domain, border_thickness);

      mln_concrete(I)
	output = internal::antialiased_dispatch(input, factor,
						output_domain,
						border_thickness);

      trace::exiting("subsampling::antialiased");
      return output;
    }


    template <typename I>
    inline
    mln_concrete(I)
    antialiased(const Image<I>& input, unsigned factor)
    {
      mln_domain(I) domain;
      return antialiased(input, factor, domain, border::thickness);
    }



# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::subsampling

} // end of namespace mln


#endif // ! MLN_SUBSAMPLING_ANTIALIASED_HH
