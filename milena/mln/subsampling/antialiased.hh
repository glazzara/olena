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

#ifndef MLN_SUBSAMPLING_ANTIALIASED_HH
# define MLN_SUBSAMPLING_ANTIALIASED_HH

/// \file
///
/// Antialiased subsampling.

/// \fixme: shift is not used for the moment......

#include <mln/core/concept/image.hh>
#include <mln/border/thickness.hh>


namespace mln
{

  namespace subsampling
  {

    /*! FIXME: Doc.
     */
    template <typename I>
    inline
    mln_concrete(I)
    antialiased(const Image<I>& input,
		unsigned gap,
		const mln_deduce(I, site, delta)& shift,
		const mln_domain(I)& output_domain,
		unsigned border_thickness);


    /*! \overload
     */
    template <typename I>
    mln_concrete(I)
    antialiased(const Image<I>& input,
		unsigned gap,
		const mln_deduce(I, site, delta)& shift); // FIXME: Add round_up_size.


    /*! \overload
     */
    template <typename I>
    mln_concrete(I)
    antialiased(const Image<I>& input, unsigned gap);



# ifndef MLN_INCLUDE_ONLY

    // Tests

    namespace internal
    {

      template <typename I>
      inline
      void
      antialiased_tests(const Image<I>& input,
			unsigned gap,
			const mln_deduce(I, site, delta)& shift,
			const mln_domain(I)& output_domain,
			unsigned border_thickness)
      {
	typedef mln_site(I) P;

	mlc_is_a(mln_domain(I), Box)::check();
	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(input).domain().pmin() == literal::origin);
	mln_precondition(gap > 1);
	for (unsigned i = 0; i < P::dim; ++i)
	  mln_precondition(shift[i] < static_cast<int>(gap));

	(void) input;
	(void) gap;
	(void) shift;
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
		    unsigned gap,
		    const mln_deduce(I, site, delta)& shift,
		    const mln_domain(I)& output_domain,
		    unsigned border_thickness)
	{
	  // To be written...
	  mlc_abort(I)::check();

	  mln_concrete(I) output;
	  return output;
	}

      } // end of namespace mln::subsampling::impl::generic


      template <typename I>
      inline
      mln_concrete(I)
      antialiased_2d_antialias_fastest_2(const Image<I>& input_,
					 const mln_deduce(I, site, delta)& shift,
					 const mln_domain(I)& output_domain,
					 unsigned border_thickness)
      {
	trace::entering("subsampling::impl::antialiased_2d_antialias_fastest_2");

	internal::antialiased_tests(input_, 2, shift,
				    output_domain, border_thickness);

	const I& input = exact(input_);

	typedef mln_value(I) V;
	typedef mln_sum(V) S;

	typedef mln_site(I) P;
	box<P> b = output_domain;
	if (!b.is_valid())
	{
	  P pmin = input.domain().pmin() / 2,
	    pmax = input.domain().pmax() / 2;
	  b = box<P>(pmin, pmax);
	}

	typedef mln_concrete(I) O;
	O output(b, border_thickness);

	const V* ptr1 = & input.at_(0, 0);
	const V* ptr2 = & input.at_(1, 0);

	mln_box_runstart_piter(O) s(output.domain());
	const unsigned n = s.run_length();

	unsigned offset = input.delta_index(point2d(2,0) - point2d(0,2*n));

	for_all(s)
	{
	  mln_value(O)* po = & output(s);
	  for (unsigned i = 0; i < n; ++i)
	    {
	      S s;
	      s  = *ptr1 + *(ptr1 + 1);
	      s += *ptr2 + *(ptr2 + 1);
	      ptr1 += 2;
	      ptr2 += 2;

	      *po++ = (s + 2) / 4;
	    }
	  ptr1 += offset;
	  ptr2 += offset;
	}

	trace::exiting("subsampling::impl::antialiased_2d_antialias_fastest_2");
	return output;
      }



      template <typename I>
      inline
      mln_concrete(I)
      antialiased_2d_antialias_fastest_3(const Image<I>& input_,
					 const mln_deduce(I, site, delta)& shift,
					 const mln_domain(I)& output_domain,
					 unsigned border_thickness)
      {
	trace::entering("subsampling::impl::antialiased_2d_antialias_fastest_3");

	internal::antialiased_tests(input_, 3, shift,
				    output_domain, border_thickness);

	const I& input = exact(input_);

	typedef mln_value(I) V;
	typedef mln_sum(V) S;

	typedef mln_site(I) P;
	box<P> b = output_domain;
	if (!b.is_valid())
	{
	  P pmin = input.domain().pmin() / 3,
	    pmax = input.domain().pmax() / 3;
	  b = box<P>(pmin, pmax);
	}
	typedef mln_concrete(I) O;
	O output(b, border_thickness);

	const V* ptr1 = & input.at_(0, 0);
	const V* ptr2 = & input.at_(1, 0);
	const V* ptr3 = & input.at_(2, 0);

	mln_box_runstart_piter(O) s(output.domain());
	const unsigned n = s.run_length();

	unsigned offset = input.delta_index(point2d(3,0) - point2d(0,3*n));

	for_all(s)
	{
	  mln_value(O)* po = & output(s);
	  for (unsigned i = 0; i < n; ++i)
	    {
	      S s;
	      s  = *ptr1 + *(ptr1 + 1) + *(ptr1 + 2);
	      s += *ptr2 + *(ptr2 + 1) + *(ptr2 + 2);
	      s += *ptr3 + *(ptr3 + 1) + *(ptr3 + 2);
	      ptr1 += 3;
	      ptr2 += 3;
	      ptr3 += 3;

	      *po++ = (s + 4) / 9;
	    }
	  ptr1 += offset;
	  ptr2 += offset;
	  ptr3 += offset;
	}

	trace::exiting("subsampling::impl::antialiased_2d_antialias_fastest_3");
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
			   unsigned gap,
			   const mln_deduce(I, site, delta)& shift,
			   const mln_domain(I)& output_domain,
			   unsigned border_thickness)
      {
	// Not implemented yet.
	mlc_abort(I)::check();
      }

      template <typename I>
      inline
      mln_concrete(I)
      antialiased_2d_antialias_fastest_dispatch(
	const Image<I>& input,
	unsigned gap,
	const mln_deduce(I, site, delta)& shift,
	const mln_domain(I)& output_domain,
	unsigned border_thickness)
      {
	if (gap == 2)
	  return impl::antialiased_2d_antialias_fastest_2(input, shift,
							  output_domain,
							  border_thickness);
	else if (gap == 3)
	  return impl::antialiased_2d_antialias_fastest_3(input, shift,
							  output_domain,
							  border_thickness);
	else
	  trace::warning("Not implemented yet!");

	mln_concrete(I) output;
	return output;
      }


      template <typename I>
      inline
      mln_concrete(I)
      antialiased_dispatch_2d(trait::image::value_alignment::with_grid,
			      trait::image::value_storage::one_block,
			      trait::image::value_access::direct,
			      const Image<I>& input,
			      unsigned gap,
			      const mln_deduce(I, site, delta)& shift,
			      const mln_domain(I)& output_domain,
			      unsigned border_thickness)
      {
	return antialiased_2d_antialias_fastest_dispatch(input, gap,
							 shift,
							 output_domain,
							 border_thickness);
      }


      template <typename I>
      inline
      mln_concrete(I)
      antialiased_dispatch(const Image<I>& input,
			   unsigned gap,
			   const mln_deduce(I, site, delta)& shift,
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
	    gap,
	    shift,
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
		unsigned gap,
		const mln_deduce(I, site, delta)& shift,
		const mln_domain(I)& output_domain,
		unsigned border_thickness)
    {
      trace::entering("subsampling::antialiased");

      typedef mln_site(I) P;

      internal::antialiased_tests(input, gap, shift,
				  output_domain, border_thickness);

      mln_concrete(I) output;
      output = internal::antialiased_dispatch(input, gap, shift,
					      output_domain, border_thickness);

      trace::exiting("subsampling::antialiased");
      return output;
    }


    template <typename I>
    inline
    mln_concrete(I)
    antialiased(const Image<I>& input,
		unsigned gap,
		const mln_deduce(I, site, delta)& shift)
    {
      mln_domain(I) domain;
      return antialiased(input, gap, shift, domain, border::thickness);
    }


    template <typename I>
    inline
    mln_concrete(I)
    antialiased(const Image<I>& input, unsigned gap)
    {
      return antialiased(input, gap, literal::zero);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::subsampling

} // end of namespace mln


#endif // ! MLN_SUBSAMPLING_ANTIALIASED_HH
