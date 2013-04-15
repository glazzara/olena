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

#ifndef SCRIBO_SUBSAMPLING_INTEGRAL_HH
# define SCRIBO_SUBSAMPLING_INTEGRAL_HH

/// \file
///
/// Both subsampling and integral image computation.

#include <mln/core/concept/image.hh>
#include <mln/metal/equal.hh>
#include <mln/extension/fill.hh>
#include <mln/debug/println.hh>
#include <mln/debug/println_with_border.hh>



namespace scribo
{

  namespace subsampling
  {
    using namespace mln;


    /*! \brief Subsample an image and compute tow integral images.

      \param[in] input An image of Scalar.
      \param[in] scale The scale factor.
      \param[in] integral_sum Integral image of mean values.
      \param[in] integral_sum_2 Integral image of squared mean values.
      \param[in] output_domain The domain of the subscaled image.
      \param[in] border_thickness Border of the integral and subscaled images.

      \p integral_sum, \p integral_sum_2 and output image have the same domain.
      The output domain is set with \p output_domain.

     */
    template <typename I, typename J>
    inline
    mln_concrete(I)
    integral(const Image<I>& input, unsigned scale,
	     Image<J>& integral_sum, Image<J>& integral_sum_2,
	     const mln_domain(I)& output_domain, unsigned border_thickness);

    /*! \overload

      The output domain is defined as follows :

      for_each(dimension)
        input.domain()[dimension] = (N + scale - 1) / scale

      where N is the number of elements in one run in a given dimension.

      The border thickness is set to mln::border::thickness.
     */
    template <typename I, typename J>
    inline
    mln_concrete(I)
    integral(const Image<I>& input, unsigned scale,
	     Image<J>& integral_sum, Image<J>& integral_sum_2);



# ifndef MLN_INCLUDE_ONLY


    // Implementation.

    namespace impl
    {

      template <typename I, typename J>
      inline
      mln_concrete(I)
      integral_3(const Image<I>& input_,
		 Image<J>& integral_sum_, Image<J>& integral_sum_2_,
		 const mln_domain(I)& output_domain,
		 unsigned border_thickness)
      {
	mln_trace("subsampling::impl::integral_3");

	const unsigned
	  scale = 3,
	  area = scale * scale;

	const I& input = exact(input_);
	J& integral_sum = exact(integral_sum_);
	J& integral_sum_2 = exact(integral_sum_2_);

	mln_precondition(input.is_valid());
	mln_precondition(input.domain().pmin() == literal::origin);
	mln_precondition(scale > 1);

	typedef mln_value(I) V;
	typedef mln_value(J) S;
	typedef mln_site(I) P;
// 	mlc_is(mln_value(J), S)::check();

	mln_concrete(I) sub(output_domain, border_thickness);
	V* p_sub = sub.buffer();

	integral_sum.init_(output_domain, border_thickness);
	S* p_isum = integral_sum.buffer();

	integral_sum_2.init_(output_domain, border_thickness);
	S* p_isum_2 = integral_sum_2.buffer();

	const unsigned up = sub.delta_offset(dpoint2d(-1, 0));

	const unsigned nrows = 3 * output_domain.nrows();
	const unsigned ncols = 3 * output_domain.ncols();

	unsigned row = 0;

	unsigned b_offset = sub.delta_offset(dpoint2d(border_thickness,
						     border_thickness));
	p_sub += b_offset;
	p_isum += b_offset;
	p_isum_2 += b_offset;
	{
	  S h_sum = 0, h_sum_2 = 0;
	  const V* ptr1 = & input.at_(row, 0);
	  const V* ptr2 = & input.at_(row + 1, 0);
	  const V* ptr3 = & input.at_(row + 2, 0);
	  for (unsigned col = 0; col < ncols; col += scale)
	  {
	    S sum;
	    sum  = *ptr1 + *(ptr1 + 1) + *(ptr1 + 2);
	    sum += *ptr2 + *(ptr2 + 1) + *(ptr2 + 2);
	    sum += *ptr3 + *(ptr3 + 1) + *(ptr3 + 2);
	    ptr1 += 3;
	    ptr2 += 3;
	    ptr3 += 3;

	    *p_sub++ = sum / area;

	    h_sum   += sum;
	    h_sum_2 += sum * sum;

	    // exception
	    *p_isum = h_sum;
	    *p_isum_2 = h_sum_2;

	    p_isum += 1;
	    p_isum_2 += 1;
	  }
	}

 	unsigned b_next = 2 * border_thickness;

	p_sub += b_next;
	p_isum += b_next;
	p_isum_2 += b_next;

	for (row += scale; row < nrows; row += scale)
	{
	  S h_sum = 0, h_sum_2 = 0;
	  const V* ptr1 = & input.at_(row, 0);
	  const V* ptr2 = & input.at_(row + 1, 0);
	  const V* ptr3 = & input.at_(row + 2, 0);
	  for (unsigned col = 0; col < ncols; col += scale)
	  {
	    S sum;
	    sum  = *ptr1 + *(ptr1 + 1) + *(ptr1 + 2);
	    sum += *ptr2 + *(ptr2 + 1) + *(ptr2 + 2);
	    sum += *ptr3 + *(ptr3 + 1) + *(ptr3 + 2);
	    ptr1 += 3;
	    ptr2 += 3;
	    ptr3 += 3;

	    *p_sub++ = sum / area;

	    h_sum   += sum;
	    h_sum_2 += sum * sum;

	    *p_isum = h_sum + *(p_isum + up);
	    *p_isum_2 = h_sum_2 + *(p_isum_2 + up);

	    p_isum += 1;
	    p_isum_2 += 1;
	  }

	  p_sub += b_next;
	  p_isum += b_next;
	  p_isum_2 += b_next;
	}

	return sub;
      }


      template <typename I, typename J>
      inline
      mln_concrete(I)
      integral_2(const Image<I>& input_,
		 Image<J>& integral_sum_, Image<J>& integral_sum_2_,
		 const mln_domain(I)& output_domain,
		 unsigned border_thickness)
      {
	mln_trace("subsampling::impl::integral_2");

	const unsigned
	  scale = 2,
	  area = scale * scale;

	const I& input = exact(input_);
	J& integral_sum = exact(integral_sum_);
	J& integral_sum_2 = exact(integral_sum_2_);

	typedef mln_value(I) V;
	typedef mln_value(J) S;
	typedef mln_site(I) P;

	mlc_bool(P::dim == 2)::check();
	mln_precondition(input.is_valid());
	mln_precondition(input.domain().pmin() == literal::origin);
	mln_precondition(scale > 1);

// 	mlc_is(mln_value(J), S)::check();

	mln_concrete(I) sub(output_domain, border_thickness);
	V* p_sub = sub.buffer();

	integral_sum.init_(output_domain, border_thickness);
	S* p_isum = integral_sum.buffer();

	integral_sum_2.init_(output_domain, border_thickness);
	S* p_isum_2 = integral_sum_2.buffer();

	const unsigned up = sub.delta_offset(dpoint2d(-1, 0));

	const unsigned nrows = 2 * output_domain.nrows();
	const unsigned ncols = 2 * output_domain.ncols();

	extension::fill(sub, 0);

	unsigned b_offset = sub.delta_offset(dpoint2d(border_thickness,
						     border_thickness));
	p_sub += b_offset;
	p_isum += b_offset;
	p_isum_2 += b_offset;

	unsigned row = 0;
	{
	  S h_sum = 0, h_sum_2 = 0;
	  const V* ptr1 = & input.at_(row, 0);
	  const V* ptr2 = & input.at_(row + 1, 0);
	  for (unsigned col = 0; col < ncols; col += scale)
	  {
	    S sum;
	    sum  = *ptr1 + *(ptr1 + 1);
	    sum += *ptr2 + *(ptr2 + 1);
	    ptr1 += 2;
	    ptr2 += 2;

	    *p_sub++ = sum / area;

	    h_sum   += sum;
	    h_sum_2 += sum * sum;

	    // exception
	    *p_isum = h_sum;
	    *p_isum_2 = h_sum_2;

	    p_isum += 1;
	    p_isum_2 += 1;
	  }
	}

 	unsigned b_next = 2 * border_thickness;

	p_sub += b_next;
	p_isum += b_next;
	p_isum_2 += b_next;

	for (row += scale; row < nrows; row += scale)
	{
	  S h_sum = 0, h_sum_2 = 0;
	  const V* ptr1 = & input.at_(row, 0);
	  const V* ptr2 = & input.at_(row + 1, 0);
	  for (unsigned col = 0; col < ncols; col += scale)
	  {
	    S sum;
	    sum  = *ptr1 + *(ptr1 + 1);
	    sum += *ptr2 + *(ptr2 + 1);
	    ptr1 += 2;
	    ptr2 += 2;

	    *p_sub++ = sum / area;

	    h_sum   += sum;
	    h_sum_2 += sum * sum;

	    *p_isum = h_sum + *(p_isum + up);
	    *p_isum_2 = h_sum_2 + *(p_isum_2 + up);

	    p_isum += 1;
	    p_isum_2 += 1;
	  }

	  p_sub += b_next;
	  p_isum += b_next;
	  p_isum_2 += b_next;
	}

	return sub;
      }


      template <typename I, typename J>
      inline
      mln_concrete(I)
      integral(const Image<I>& input, unsigned scale,
	       Image<J>& simple, Image<J>& squared,
	       const mln_domain(I)& output_domain, unsigned border_thickness)
      {
	//       mln_precondition(input.nrows() % scale == 0);
	//       mln_precondition(input.ncols() % scale == 0);
	if (scale == 3)
	  return integral_3(input, simple, squared,
			    output_domain, border_thickness);
	else if (scale == 2)
	  return integral_2(input, simple, squared,
			    output_domain, border_thickness);
	else
	  std::cerr << "NYI!" << std::endl;

	typedef mln_concrete(I) output_t;
	return output_t();
      }


    } // end of namespace mln::subsampling::impl




    // Facades.

    template <typename I, typename J>
    inline
    mln_concrete(I)
    integral(const Image<I>& input_, unsigned scale,
	     Image<J>& integral_sum, Image<J>& integral_sum_2,
	     const mln_domain(I)& output_domain, unsigned border_thickness)
    {
      mln_trace("subsampling::integral");

      const I& input = exact(input_);

      mln_precondition(input.is_valid());
      mln_precondition(input.domain().pmin() == literal::origin);
      mln_precondition(scale > 1);

      mln_concrete(I)
	output = impl::integral(input, scale, integral_sum, integral_sum_2,
				output_domain, border_thickness);

      return output;
    }

    template <typename I, typename J>
    inline
    mln_concrete(I)
    integral(const Image<I>& input_, unsigned scale,
	     Image<J>& integral_sum, Image<J>& integral_sum_2)
    {
      mln_trace("subsampling::integral");

      const I& input = exact(input_);

      mln_precondition(input.is_valid());
      mln_precondition(input.domain().pmin() == literal::origin);
      mln_precondition(scale > 1);

      box<mln_site(I)> b = make::box2d((input.nrows() + scale - 1) / scale,
				       (input.ncols() + scale - 1) / scale);
      mln_concrete(I) output;
      output = integral(input_, scale, integral_sum, integral_sum_2,
			b, mln::border::thickness);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::subsampling

} // end of namespace scribo


#endif // ! SCRIBO_SUBSAMPLING_INTEGRAL_HH
