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

#ifndef MLN_SUBSAMPLING_DEBASE_HH
# define MLN_SUBSAMPLING_DEBASE_HH

/// \file
///
/// Debase subsampling.

#include <mln/core/concept/image.hh>

#define MLN_FLOAT double
#include <sandbox/theo/exec/gaussian_directional_2d.hh>



namespace mln
{

  namespace subsampling
  {

    /// FIXME: Doc.

    template <typename I>
    mln_concrete(I)
    debase(const Image<I>& input,
	   unsigned gap,
	   const mln_deduce(I, site, delta)& shift); // FIXME: Add round_up_size.


    template <typename I>
    mln_concrete(I)
    debase(const Image<I>& input, unsigned gap);



# ifndef MLN_INCLUDE_ONLY


    // Implementation.

    namespace impl
    {


      template <typename I>
      inline
      mln_concrete(I)
      debase(const Image<I>& input_,
	     unsigned gap,
	     const mln_deduce(I, site, delta)& shift)
      {
	trace::entering("subsampling::impl::debase");

	const I& input = exact(input_);

	typedef mln_site(I) P;
	P pmin = input.domain().pmin() / gap,
	  pmax = input.domain().pmax() / gap;
	
	box<P> b(pmin, pmax);
	mln_concrete(I) output(b);
	mln_piter(I) p(output.domain());
	for_all(p)
	  output(p) = input(p * gap + shift);

	trace::exiting("subsampling::impl::debase");
	return output;
      }


      template <typename I>
      inline
      mln_concrete(I)
      debase_fastest(const Image<I>& input_,
		     unsigned gap,
		     const mln_deduce(I, site, delta)& shift)
      {
	trace::entering("subsampling::impl::debase_fastest");

	const I& input = exact(input_);

	typedef mln_site(I) P;
	P pmin = input.domain().pmin() / gap,
	  pmax = input.domain().pmax() / gap;
	
	box<P> b(pmin, pmax);

	typedef mln_concrete(I) O;
	O output(b);

	mln_box_runstart_piter(O) s(output.domain());
	const unsigned n = s.run_length();
	for_all(s)
	{
	  const mln_value(I)* pi = & input(s * gap + shift);
	  mln_value(O)* po = & output(s);
	  for (unsigned i = 0; i < n; ++i)
	    {
	      *po++ = *pi;
	      pi += gap;
	    }
	}

	trace::exiting("subsampling::impl::debase_fastest");
	return output;
      }


      template <typename I>
      inline
      mln_concrete(I)
      debase_gaussian_antialiased_fastest(const Image<I>& input_,
					  unsigned gap,
					  const mln_deduce(I, site, delta)& shift)
      {
	trace::entering("subsampling::impl::debase_gaussian_antialiased_fastest");

	const I& input = exact(input_);

	image2d<MLN_FLOAT> temp(input.domain());
	data::fill(temp, input);
	temp = linear::gaussian_directional_2d(temp, 1, 1.5, 0);
	temp = linear::gaussian_directional_2d(temp, 0, 1.5, 0);

	typedef mln_value(I) V;
	typedef mln_site(I) P;
	P pmin = input.domain().pmin() / gap,
	  pmax = input.domain().pmax() / gap;
	
	box<P> b(pmin, pmax);

	typedef mln_concrete(I) O;
	O output(b);

	mln_box_runstart_piter(O) s(output.domain());
	const unsigned n = s.run_length();
	for_all(s)
	{
	  const MLN_FLOAT* pi = & temp(s * gap + shift);
	  mln_value(O)* po = & output(s);
	  for (unsigned i = 0; i < n; ++i)
	    {
	      *po++ = V(*pi + 0.49);
	      pi += gap;
	    }
	}

	trace::entering("subsampling::impl::debase_gaussian_antialiased_fastest");
	return output;
      }




      template <typename I>
      inline
      mln_concrete(I)
      debase_2d_antialias_pointer_fastest(const Image<I>& input_,
				       unsigned gap,
				       const mln_deduce(I, site, delta)& shift)
      {
	trace::entering("subsampling::impl::debase_2d_antialias_pointer_fastest");

	const I& input = exact(input_);

	typedef mln_site(I) P;
	P pmin = input.domain().pmin() / gap,
	  pmax = input.domain().pmax() / gap;
	
	box<P> b(pmin, pmax);

	typedef mln_concrete(I) O;
	O output(b);

	const unsigned nrows = input.nrows();
	const unsigned ncols = input.ncols();

	typedef mln_value(I) V;
	typedef mln_sum(V) S;
	for (unsigned row = 0; row < nrows; row += gap)
	  {
	    const V* ptr1 = & input.at_(row, 0);
	    const V* ptr2 = & input.at_(row + 1, 0);
	    const V* ptr3 = & input.at_(row + 2, 0);
	    for (unsigned col = 0; col < ncols; col += gap)
	      {
		S s;

		// mean
		s  = *ptr1++ + *ptr1++ + *ptr1++;
		s += *ptr2++ + *ptr2++ + *ptr2++;
		s += *ptr3++ + *ptr3++ + *ptr3++;
		output.at_(row / gap, col / gap) = (s + 4) / 9;
// 		// cut
// 		s  = 1 * *ptr1++ + 2 * *ptr1++ + 1 * *ptr1++;
// 		s += 2 * *ptr2++ + 3 * *ptr2++ + 2 * *ptr2++;
// 		s += 1 * *ptr3++ + 2 * *ptr3++ + 1 * *ptr3++;
// 		output.at_(row / gap, col / gap) = (s + 7) / 15;
	      }
	  }

	trace::exiting("subsampling::impl::debase_2d_antialias_pointer_fastest");
	return output;
      }



      template <typename I>
      inline
      mln_concrete(I)
      debase_2d_antialias_offset_fastest(const Image<I>& input_,
				       unsigned gap,
				       const mln_deduce(I, site, delta)& shift)
      {
	trace::entering("subsampling::impl::debase_2d_antialias_offset_fastest");

	const I& input = exact(input_);

	typedef mln_value(I) V;
	typedef mln_sum(V) S;

	typedef mln_site(I) P;
	P pmin = input.domain().pmin() / gap,
	  pmax = input.domain().pmax() / gap;
	box<P> b(pmin, pmax);
	typedef mln_concrete(I) O;
	O output(b);

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
	      s  = *ptr1++ + *ptr1++ + *ptr1++;
	      s += *ptr2++ + *ptr2++ + *ptr2++;
	      s += *ptr3++ + *ptr3++ + *ptr3++;
	      *po++ = (s + 4) / 9;
	    }
	  ptr1 += offset;
	  ptr2 += offset;
	  ptr3 += offset;
	}

	trace::exiting("subsampling::impl::debase_2d_antialias_offset_fastest");
	return output;
      }


    } // end of namespace mln::subsampling::impl



    // Dispatch.

    namespace internal
    {

      template <typename I>
      inline
      mln_concrete(I)
      debase_dispatch(const Image<I>& input,
		      unsigned gap,
		      const mln_deduce(I, site, delta)& shift)
      {
	return impl::debase_2d_antialias_pointer_fastest /* debase_2d_antialias_offset_fastest */(input, gap, shift);
      }


    } // end of namespace mln::subsampling::internal



    // Facades.

    template <typename I>
    inline
    mln_concrete(I)
    debase(const Image<I>& input,
	   unsigned gap,
	   const mln_deduce(I, site, delta)& shift)
    {
      trace::entering("subsampling::debase");

      typedef mln_site(I) P;
      
      mln_precondition(exact(input).is_valid());
      mln_precondition(exact(input).domain().pmin() == literal::origin);
      mln_precondition(gap > 1);
      for (unsigned i = 0; i < P::dim; ++i)
	mln_precondition(shift[i] < gap);

      mln_concrete(I) output;
      output = internal::debase_dispatch(input, gap, shift);

      trace::exiting("subsampling::debase");
      return output;
    }


    template <typename I>
    inline
    mln_concrete(I)
    debase(const Image<I>& input, unsigned gap)
    {
      return debase(input, gap, literal::zero);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::subsampling

} // end of namespace mln


#endif // ! MLN_SUBSAMPLING_DEBASE_HH
