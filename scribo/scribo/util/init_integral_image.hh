// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
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

#ifndef SCRIBO_UTIL_INIT_INTEGRAL_IMAGE_HH
# define SCRIBO_UTIL_INIT_INTEGRAL_IMAGE_HH

/// \file
///
///

# include <mln/core/image/image2d.hh>

namespace scribo
{

  namespace util
  {

    using namespace mln;


    template <typename I, typename F>
    mln_ch_value(I, mln_result(F))
    init_integral_image(const Image<I>& input, unsigned scale, F& func);

    template <typename I, typename F>
    mln_ch_value(I, mln_result(F))
    init_integral_image(const Image<I>& input, unsigned scale, F& func,
			unsigned border);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {

	template <typename I, typename F>
	mln_ch_value(I, mln_result(F))
	init_integral_image(const Image<I>& input, unsigned scale, F& f,
			    const mln_box(I)& output_domain, unsigned border)
	{
	  trace::entering("scribo::util::impl::generic::init_integral_image");
	  typedef mln_ch_value(I, mln_result(F)) J;

	  // Not implemented
	  mlc_abort(I)::check();

	  trace::exiting("scribo::util::impl::generic::init_integral_image");
	  return J();
	}

      } // end of namespace scribo::util::impl::generic


      template <typename I, typename F>
      mln_ch_value(I, mln_result(F))
      init_integral_image_fastest(const Image<I>& input_, unsigned scale, F& f,
				  const mln_box(I)& output_domain, unsigned border)
      {
	trace::entering("scribo::util::impl::init_integral_image_fastest");

	const I& input = exact(input_);
	typedef mln_ch_value(I, mln_result(F)) J;

	mln_precondition(input.is_valid());
	mln_precondition(input.domain().pmin() == literal::origin);

	typedef mln_value(I) V;
	typedef mln_sum(V) S;
	typedef mln_value(J) V2;
	typedef mln_site(I) P;

	J integral_sum_sum_2(output_domain, border);
	V2* p_integ = integral_sum_sum_2.buffer();

	const int up = integral_sum_sum_2.delta_index(dpoint2d(-1, 0));

	const unsigned nrows = scale * integral_sum_sum_2.nrows();
	const unsigned ncols = scale * integral_sum_sum_2.ncols();

	unsigned row = 0;

	unsigned b_offset = integral_sum_sum_2.delta_index(dpoint2d(border,
								    border));

	// First row (special case)
	p_integ += b_offset;
	{
	  f.begin_of_first_row(); // <---- begin_of_first_row()

	  mln::util::array<const V*> ptr(scale);
	  for (unsigned s = 0; s < scale; ++s)
	    ptr[s] = & input.at_(row + s, 0);

	  for (unsigned col = 0; col < ncols; col += scale)
	  {
	    f.begin_of_col(); // <---- begin_of_col()

	    for (unsigned s = 0; s < scale; ++s)
	      for (unsigned i = 0; i < scale; ++i)
	    	f.take(*(ptr[s]++)); // <---- take()

	    // exception
	    *p_integ = f.to_result_first_row(); // <---- to_result_first_row()

	    f.end_of_col(); // <---- end_of_col()
	    ++p_integ;
	  }
	  f.end_of_row(); // <---- end_of_row()
	}

	unsigned b_next = 2 * border;

	// Other rows (general case)
	p_integ += b_next;
	for (row += scale; row < nrows; row += scale)
	{
	  f.begin_of_row(); // <---- begin_of_row()

	  mln::util::array<const V*> ptr(scale);
	  for (unsigned s = 0; s < scale; ++s)
	    ptr[s] = & input.at_(row + s, 0);

	  for (unsigned col = 0; col < ncols; col += scale)
	  {
	    f.begin_of_col(); // <---- begin_of_col()

	    for (unsigned s = 0; s < scale; ++s)
	      for (unsigned i = 0; i < scale; ++i)
	    	f.take(*(ptr[s]++)); // <---- take()

	    *p_integ = f.to_result(*(p_integ + up)); // <---- to_result()

	    f.end_of_col(); // <---- end_of_col()
	    ++p_integ;
	  }
	  f.end_of_row(); // <---- end_of_row()
	  p_integ += b_next;
	}

	trace::exiting("scribo::util::impl::init_integral_fastest_image");
	return integral_sum_sum_2;
      }

    } // end of namespace scribo::util::impl


    // Dispatch

    namespace internal
    {

      template <typename I, typename F>
      mln_ch_value(I,mln_result(F))
      init_integral_image_dispatch(mln::trait::image::speed::fastest,
				   const Image<I>& input, unsigned scale,
				   F& func, const mln_box(I)& output_domain,
				   unsigned border)
      {
	return impl::init_integral_image_fastest(input, scale, func,
						 output_domain, border);
      }


      template <typename I, typename F>
      mln_ch_value(I,mln_result(F))
      init_integral_image_dispatch(mln::trait::image::speed::any,
				   const Image<I>& input, unsigned scale,
				   F& func, const mln_box(I)& output_domain,
				   unsigned border)
      {
	return impl::generic::init_integral_image(input, scale, func,
						  output_domain, border);
      }


      template <typename I, typename F>
      mln_ch_value(I,mln_result(F))
      init_integral_image_dispatch(const Image<I>& input, unsigned scale, F& func,
				   const mln_box(I)& output_domain, unsigned border)
      {
	return init_integral_image_dispatch(mln_trait_image_speed(I)(),
					    input, scale, func, output_domain, border);
      }

    } // end of namespace scribo::util::internal


    // Facade

    template <typename I, typename F>
    mln_ch_value(I,mln_result(F))
    init_integral_image(const Image<I>& input_, unsigned scale, F& func,
			const mln_box(I)& output_domain, unsigned border)
    {
      trace::entering("scribo::util::init_integral_image");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());
      mln_precondition(input.domain().pmin() == literal::origin);

      mln_ch_value(I,mln_result(F))
	output = internal::init_integral_image_dispatch(input, scale, func,
							output_domain, border);

      trace::exiting("scribo::util::init_integral_image");
      return output;
    }

    template <typename I, typename F>
    mln_ch_value(I,mln_result(F))
    init_integral_image(const Image<I>& input_, unsigned scale, F& func)
    {
      trace::entering("scribo::util::init_integral_image");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());
      mln_precondition(input.domain().pmin() == literal::origin);

      box<mln_site(I)>
	output_domain = mln::make::box2d((input.nrows() + scale - 1) / scale,
					 (input.ncols() + scale - 1) / scale);

      mln_ch_value(I,mln_result(F))
	output = init_integral_image(input, scale, func,
				 output_domain, exact(input).border());

      trace::exiting("scribo::util::init_integral_image");
      return output;
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::util

} // end of namespace scribo

#endif // ! SCRIBO_UTIL_INIT_INTEGRAL_IMAGE_HH
