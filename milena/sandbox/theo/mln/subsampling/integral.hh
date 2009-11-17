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

#ifndef MLN_SUBSAMPLING_INTEGRAL_HH
# define MLN_SUBSAMPLING_INTEGRAL_HH

/// \file
///
/// Both subsampling and integral image computation.

#include <mln/core/concept/image.hh>
#include <mln/debug/println.hh>



namespace mln
{

  namespace subsampling
  {

    /// FIXME: Doc.

    template <typename I>
    mln_concrete(I)
    integral(const Image<I>& input, unsigned scale);



# ifndef MLN_INCLUDE_ONLY


    // Implementation.

    namespace impl
    {


      template <unsigned scale, typename I>
      inline
      mln_concrete(I)
      integral__why_is_it_slow(const Image<I>& input_)
      {
	trace::entering("subsampling::impl::integral__why_is_it_slow");

	const I& input = exact(input_);
	const unsigned scale2 = scale * scale, round_it = scale2 / 2;

	typedef mln_value(I) V;
	typedef mln_sum(V) S;
	typedef mln_site(I) P;

	box<P> b = make::box2d(input.nrows() / scale, input.ncols() / scale);
	const unsigned nrows = b.nrows(), ncols = b.ncols();

	typedef mln_concrete(I) O;
	O output(b, 0); // no external border in 'output'
	V* po = output.buffer();

	unsigned
	  tile_cr = input.delta_index(dpoint2d(scale, - ncols * scale)),
	  cr      = input.delta_index(dpoint2d(1, - (int)scale));

	const V* pi = & input.at_(0, 0); // first point of tiles

	// (row, col) are tile coordinates.
	for (unsigned row = 0; row < nrows; ++row)
	  {
	    S hsum = 0, hsum2 = 0; // horizontal integral
	    for (unsigned col = 0; col < ncols; ++col)
	      {
		S sum = 0, sum2 = 0;
		const V* ptr = pi;

		for (unsigned r = 0; r < scale; ++r)
		  {
		    for (unsigned c = 0; c < scale; ++c)
		      {
			sum += *ptr;
			++ptr;
		      }
		    ptr += cr;
		  }
		// ...
		pi += scale;
		*po++ = (round_it + sum) / scale2;
	      }
	    pi += tile_cr;
	  }

	trace::exiting("subsampling::impl::integral__why_is_it_slow");
	return output;
      }



      template <unsigned scale, typename I>
      inline
      mln_concrete(I)
      integral_(const Image<I>& input_)
      {
	trace::entering("subsampling::impl::integral_");

	const I& input = exact(input_);
	const unsigned scale2 = scale * scale, round_it = scale2 / 2;

	typedef mln_value(I) V;
	typedef mln_sum(V) S;
	typedef mln_site(I) P;

	box<P> b = make::box2d((input.nrows() + scale - 1) / scale,
			       (input.ncols() + scale - 1) / scale);
	typedef mln_concrete(I) O;
	O output(b, 0);
	V* po = output.buffer();

	const unsigned nrows = input.nrows();
	const unsigned ncols = input.ncols();

	typedef const V* Ptr; 
	Ptr ptr[scale];
	for (unsigned row = 0; row < nrows; row += scale)
	  {
	    for (unsigned r = 0; r < scale; ++r)
	      ptr[r] = & input.at_(row + r, 0);

	    for (unsigned col = 0; col < ncols; col += scale)
	      {
		S sum = 0;
		for (unsigned r = 0; r < scale; ++r)
		  for (unsigned c = 0; c < scale; ++c)
		    // g++ does *not* un-roll this inner loop!!!
		    // icc does...
		    sum += *ptr[r]++;

		*po++ = (round_it + sum) / scale2;
	      }
	  }

	trace::exiting("subsampling::impl::integral_");
	return output;
      }




      template <typename I>
      inline
      mln_concrete(I)
      integral_3(const Image<I>& input_)
      {
	trace::entering("subsampling::impl::integral_3");

	const unsigned scale = 3;

	const I& input = exact(input_);
	const unsigned area = scale * scale;

	typedef mln_value(I) V;
	typedef mln_sum(V) S;
	typedef mln_site(I) P;

	box<P> b = make::box2d((input.nrows() + scale - 1) / scale,
			       (input.ncols() + scale - 1) / scale);
	const unsigned up = b.ncols();

	mln_concrete(I) sub(b, 0);
	V* p_sub = sub.buffer();

	mln_ch_value(I, S) integral_sum(b, 0);
	S* p_isum = integral_sum.buffer();

	mln_ch_value(I, S) integral_sum_2(b, 0);
	S* p_isum_2 = integral_sum_2.buffer();

	const unsigned nrows = input.nrows();
	const unsigned ncols = input.ncols();

	for (unsigned row = 0; row < nrows; row += scale)
	  {
	    S h_sum = 0, h_sum_2 = 0;
	    const V* ptr1 = & input.at_(row, 0);
	    const V* ptr2 = & input.at_(row + 1, 0);
	    const V* ptr3 = & input.at_(row + 2, 0);
	    for (unsigned col = 0; col < ncols; col += scale)
	      {
		S sum;
		sum  = *ptr1++ + *ptr1++ + *ptr1++;
		sum += *ptr2++ + *ptr2++ + *ptr2++;
		sum += *ptr3++ + *ptr3++ + *ptr3++;

		S val = sum / area;
		*p_sub++ = val;

		h_sum   += val;
		h_sum_2 += val * val;

		if (row != 0)
		  {
		    *p_isum = h_sum + *(p_isum - up);
		    *p_isum_2 = h_sum_2 + *(p_isum_2 - up);
		  }
		else
		  {
		    // exception
		    *p_isum = h_sum;
		    *p_isum_2 = h_sum_2;
		  }

		p_isum += 1;
		p_isum_2 += 1;
	      }
	  }

	debug::println(sub);
	debug::println(integral_sum);
	debug::println(integral_sum_2);

	trace::exiting("subsampling::impl::integral_3");
	return sub;
      }


      template <typename I>
      inline
      mln_concrete(I)
      integral(const Image<I>& input, unsigned scale)
      {
	//       mln_precondition(input.nrows() % scale == 0);
	//       mln_precondition(input.ncols() % scale == 0);
	if (scale == 3)
	  return integral_3(input);
	else
	  std::cerr << "NYI!" << std::endl;
      }


    } // end of namespace mln::subsampling::impl




    // Facades.

    template <typename I>
    inline
    mln_concrete(I)
    integral(const Image<I>& input_, unsigned scale)
    {
      trace::entering("subsampling::integral");

	const I& input = exact(input_);
      
      mln_precondition(input.is_valid());
      mln_precondition(input.domain().pmin() == literal::origin);
      mln_precondition(scale > 1);

      mln_concrete(I) output;
      output = impl::integral(input, scale);

      trace::exiting("subsampling::integral");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::subsampling

} // end of namespace mln


#endif // ! MLN_SUBSAMPLING_INTEGRAL_HH
